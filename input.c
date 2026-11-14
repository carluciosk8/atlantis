#include "input.h"

#include "city.h"
#include "torpedo.h"


// Wait a fixed number of frames (about 3 s at 60 Hz) instead of reading the
// fire button, so the title screen stays visible long enough to inspect.
#define WAIT_FIRE_FRAMES 180  // TODO(step 6): remove — real fire-button wait


void handle_input() // TODO(step 6): stub — real input (cursor + fire) in Step 6
{
#if TEST_AUTOFIRE
    // Auto-fire stand-in, moved here from atlantis.c TestAutoFire() (Step 4c).
    // Fires a torpedo every 32 frames, alternating the left and right
    // launchers, so collisions, the score and the deathray can be exercised
    // without reading the fire button.
    if ((g_FrameCounter % 32) == 0 && !torpedos[torpedo_current].is_active) // TODO(step 6: remove)
    {
        if (torpedo_current) // TODO(step 6: remove) — alternate left/right launcher
            torpedo_fire(TORPEDO_RIGHT_PATT, TORPEDO_RIGHT_X, TORPEDO_RIGHT_Y, TORPEDO_RIGHT_DX, TORPEDO_RIGHT_DY); // TODO(step 6: remove)
        else // TODO(step 6: remove)
            torpedo_fire(TORPEDO_LEFT_PATT,  TORPEDO_LEFT_X,  TORPEDO_LEFT_Y,  TORPEDO_LEFT_DX,  TORPEDO_LEFT_DY);  // TODO(step 6: remove)
    } // TODO(step 6: remove)
#endif
}


void wait_fire_button() // TODO(step 6): stub — real fire-button wait in Step 6
{
    for (u16 frame = 0; frame < WAIT_FIRE_FRAMES; frame++) // TODO(step 6: remove)
    {
        Halt(); // TODO(step 6: remove)
    }
}
