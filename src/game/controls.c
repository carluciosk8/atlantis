#include "controls.h"

#include "city.h"
#include "frame.h"
#include "torpedo.h"


// Debug auto-fire (compiled only when 1): fires a torpedo every 32 frames,
// alternating the left and right launchers, so collisions, the score and the
// deathray can be exercised without pressing the fire button.
#define TEST_AUTOFIRE 1


// Fire-button state of the previous frame (fires on the rising edge only).
static u8 last_trigger = 0;


// Read the keyboard as a joystick and invert once, so the result is
// active-high: bit 0 = Up, bit 1 = Down, bit 2 = Left, bit 3 = Right,
// bit 4 = Trigger A (space).
// Keyboard_ReadAsJoystick has no DI/EI between the row select (port AAh) and
// the read (port A9h), and the BIOS keyboard scan runs in the vblank
// interrupt, so the read is protected here to avoid reading the wrong row.
static u8 read_controller(void)
{
    u8 controller;
    DisableInterrupt();
    controller = (u8)~Keyboard_ReadAsJoystick();
    EnableInterrupt();
    return controller;
}


void controls_update(void)
{
    u8 controller = read_controller();
    u8 trigger    = controller & JOY_INPUT_TRIGGER_A;

    // Left / right pick a side launcher; the centre gun needs the acropolis.
    if (trigger && !last_trigger && !torpedoes[torpedo_next].is_active)
    {
        if      (controller & JOY_INPUT_DIR_LEFT)                torpedo_fire(TORPEDO_LAUNCHER_LEFT);
        else if (controller & JOY_INPUT_DIR_RIGHT)               torpedo_fire(TORPEDO_LAUNCHER_RIGHT);
        else if (map_have_installation[MAP_ACROPOLIS_COMMAND_ID]) torpedo_fire(TORPEDO_LAUNCHER_CENTER);
    }
    last_trigger = trigger;

#if TEST_AUTOFIRE
    if ((g_FrameCounter % 32) == 0 && !torpedoes[torpedo_next].is_active)
    {
        torpedo_fire(torpedo_next ? TORPEDO_LAUNCHER_RIGHT : TORPEDO_LAUNCHER_LEFT);
    }
#endif
}


bool controls_fire_held(void)
{
    return (read_controller() & JOY_INPUT_TRIGGER_A) != 0;
}
