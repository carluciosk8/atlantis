#include "controls.h"

#include "city.h"
#include "torpedo.h"


// Debug auto-fire (compiled only when 1): fires a torpedo every 32 frames,
// alternating the left and right launchers, so collisions, the score and the
// deathray can be exercised without pressing the fire button. 0 by default so
// the normal build contains no stub code.
#define TEST_AUTOFIRE 0


// Last-frame fire-button state (rising-edge latch), as in the legacy
// src/input.c `last_trigger`.
static u8 last_trigger = 0;


// Read the keyboard as a joystick and invert once, so the result is
// active-high with the legacy bit layout: bit 0 = Up, bit 1 = Down,
// bit 2 = Left, bit 3 = Right, bit 4 = Trigger A (space).
// Keyboard_ReadAsJoystick has no DI/EI around the row select (port AAh) and
// the read (port A9h), and the BIOS keyboard scan runs in the vblank
// interrupt, so the read is wrapped here to avoid reading the wrong row.
static u8 read_controller()
{
    u8 controller;
    DisableInterrupt();
    controller = (u8)~Keyboard_ReadAsJoystick();
    EnableInterrupt();
    return controller;
}


void handle_input()
{
    u8 controller = read_controller();
    u8 trigger    = controller & JOY_INPUT_TRIGGER_A;

    if (trigger && !last_trigger && !torpedos[torpedo_current].is_active)
    {
        if      (controller & JOY_INPUT_DIR_LEFT)  torpedo_fire(TORPEDO_LEFT_PATT,  TORPEDO_LEFT_X,  TORPEDO_LEFT_Y,  TORPEDO_LEFT_DX,  TORPEDO_LEFT_DY);
        else if (controller & JOY_INPUT_DIR_RIGHT) torpedo_fire(TORPEDO_RIGHT_PATT, TORPEDO_RIGHT_X, TORPEDO_RIGHT_Y, TORPEDO_RIGHT_DX, TORPEDO_RIGHT_DY);
        else if (map_have_installation[0])          torpedo_fire(TORPEDO_CENTER_PATT, TORPEDO_CENTER_X, TORPEDO_CENTER_Y, TORPEDO_CENTER_DX, TORPEDO_CENTER_DY);
    }
    last_trigger = trigger;

#if TEST_AUTOFIRE
    if ((g_FrameCounter % 32) == 0 && !torpedos[torpedo_current].is_active)
    {
        if (torpedo_current)
            torpedo_fire(TORPEDO_RIGHT_PATT, TORPEDO_RIGHT_X, TORPEDO_RIGHT_Y, TORPEDO_RIGHT_DX, TORPEDO_RIGHT_DY);
        else
            torpedo_fire(TORPEDO_LEFT_PATT,  TORPEDO_LEFT_X,  TORPEDO_LEFT_Y,  TORPEDO_LEFT_DX,  TORPEDO_LEFT_DY);
    }
#endif
}


void wait_fire_button()
{
    while (read_controller() & JOY_INPUT_TRIGGER_A)    { Halt(); }  // wait for the press
    while (!(read_controller() & JOY_INPUT_TRIGGER_A)) { Halt(); }  // wait for the release
}
