// ____________________________
//  Atlantis — MSXgl (Step 4c: torpedoes + collision)
//
//  Sets Screen 2, loads the tileset and sprite patterns, installs the vblank
//  H_TIMI hook, and runs a Halt()-synchronized frame loop. A temporary test
//  starts a wave and runs the full per-frame gameplay logic (lanes, torpedoes,
//  collisions, score, building explosions, deathray). The fire button is
//  replaced by a temporary auto-fire stand-in (TEST_AUTOFIRE), removed in
//  Step 6. Music, input and RNG are ported in later steps.
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"

#include "city.h"
#include "collision.h"
#include "hud.h"
#include "lane.h"
#include "score.h"
#include "sprites.h"
#include "tiles.h"
#include "torpedo.h"
#include "wave.h"

// Temporary auto-fire toggle (Step 4c/4d only). 1 = fire torpedoes on a timer
// (exercises collisions, score and the deathray); 0 = nothing fires, so the
// gorgons reach the last lane and the deathray can be observed on its own.
// Removed with handle_input() in Step 6.
#define TEST_AUTOFIRE 1

//=============================================================================
// FRAME LOOP
//=============================================================================

// Frame counter, incremented once per vblank by the H_TIMI hook below.
// 8-bit (wraps every 256 frames), like the legacy ubox_tick. Also zeroed
// explicitly in main() (legacy ubox_reset_tick()).
volatile u8 g_FrameCounter;

//-----------------------------------------------------------------------------
// H_TIMI hook: called by the BIOS KEYINT ISR once per vblank. Keep it short and
// do not touch the VDP here (the BIOS reads VDP status before calling it).
static void FrameCounterHook(void)
{
	g_FrameCounter++;
}

//=============================================================================
// TEMPORARY AUTO-FIRE STAND-IN — Step 4c/4d only, remove in Step 6.
//=============================================================================
// Fires a torpedo every 32 frames, alternating the left and right launchers,
// so collisions, the score and the deathray can be exercised without reading
// the fire button. Every line is TODO(step 6: remove); Step 6 ports
// handle_input(), which reads the real fire button.
#if TEST_AUTOFIRE
static void TestAutoFire(void) // TODO(step 6: remove)
{
    if ((g_FrameCounter % 32) == 0 && !torpedos[torpedo_current].is_active) // TODO(step 6: remove)
    {
        if (torpedo_current) // TODO(step 6: remove) — alternate left/right launcher
            torpedo_fire(TORPEDO_RIGHT_PATT, TORPEDO_RIGHT_X, TORPEDO_RIGHT_Y, TORPEDO_RIGHT_DX, TORPEDO_RIGHT_DY); // TODO(step 6: remove)
        else // TODO(step 6: remove)
            torpedo_fire(TORPEDO_LEFT_PATT,  TORPEDO_LEFT_X,  TORPEDO_LEFT_Y,  TORPEDO_LEFT_DX,  TORPEDO_LEFT_DY);  // TODO(step 6: remove)
    } // TODO(step 6: remove)
} // TODO(step 6: remove)
#endif

//=============================================================================
// TEMPORARY LANE TEST — Step 4b/4c, remove in Step 4d. TODO(step 4d: remove)
//=============================================================================
// Starts a wave and runs the frame loop (Halt, sprites_flush, map_update,
// lane_update_all, torpedo_update_all, collision_handle_torpedos), as the
// legacy main loop does. Collisions now kill gorgons and score hits; the death
// ray destroys installations when a gorgon reaches the last lane.
// TODO(step 4d: remove)
static void TestLanesStep4b(void) // TODO(step 4d: remove)
{
    collision_init();              // TODO(step 4d: remove)
    lane_init_all();               // TODO(step 4d: remove)
    score_init();                  // TODO(step 4d: remove)
    torpedo_init_all();            // TODO(step 4d: remove)
    wave_init();                   // TODO(step 4d: remove)
    wave_start();                  // TODO(step 4d: remove)

    for (;;)                       // TODO(step 4d: remove)
    {
#if TEST_AUTOFIRE
        TestAutoFire();            // TODO(step 4d: remove)
#endif
        Halt();                    // TODO(step 4d: remove)
        sprites_flush();           // TODO(step 4d: remove)
        map_update();              // TODO(step 4d: remove)
        lane_update_all();         // TODO(step 4d: remove)
        torpedo_update_all();      // TODO(step 4d: remove)
        collision_handle_torpedos(); // TODO(step 4d: remove)
    }
}

//-----------------------------------------------------------------------------
/// Program entry point
void main(void)
{
	// Screen 2 (Graphic 2), 256×192
	VDP_SetMode(VDP_MODE_SCREEN2);
	VDP_ClearVRAM();

	init_tiles();
	init_sprites();      // 16x16 mode + sprite patterns (after ClearVRAM)

	// Enable the vblank interrupt, then install the H_TIMI hook that increments
	// the frame counter. The 4-byte hook write is done with interrupts disabled
	// so a vblank cannot run a half-written hook (BIOS_SetHookCallback /
	// BIOS_SetHookInterSlotCallback do not DI/EI themselves).
	g_FrameCounter = 0;
	VDP_EnableVBlank(TRUE);
	DisableInterrupt();
	BIOS_SetHookCallback(H_TIMI, FrameCounterHook);
	EnableInterrupt();

	map_init();          // map + HUD (cyan border)

	TestLanesStep4b();   // TODO(step 4d: remove) — wave + frame loop + torpedoes + collisions
}
