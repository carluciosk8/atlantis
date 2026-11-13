// ____________________________
//  Atlantis — MSXgl (Step 4b: lanes)
//
//  Sets Screen 2, loads the tileset and sprite patterns, installs the vblank
//  H_TIMI hook, and runs a Halt()-synchronized frame loop. A temporary test
//  starts a wave and runs the lane state machines, forcing the explosions on a
//  timer (collision is stubbed, so nothing can kill a gorgon yet).
//  Music, input and RNG are ported in later steps.
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"

#include "city.h"
#include "hud.h"
#include "lane.h"
#include "sprites.h"
#include "tiles.h"
#include "wave.h"

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
// TEMPORARY LANE TEST — Step 4b only, remove in Step 4d. TODO(step 4d: remove)
//=============================================================================
// Starts a wave and runs the frame loop (Halt, sprites_flush, map_update,
// lane_update_all). Without collision nothing can kill a gorgon, so the test
// forces the explosions on a timer: a small one (fighter/bomber) on lane 0 and a
// big one (is_big = 1, bandit: x2 magnification + border flash) on lane 1.
// TODO(step 4d: remove)
static void TestLanesStep4b(void) // TODO(step 4d: remove)
{
	lane_init_all();               // TODO(step 4d: remove)
	wave_init();                   // TODO(step 4d: remove)
	wave_start();                  // TODO(step 4d: remove)

	for (;;)                       // TODO(step 4d: remove)
	{
		Halt();                    // TODO(step 4d: remove)
		sprites_flush();           // TODO(step 4d: remove)
		map_update();              // TODO(step 4d: remove)
		lane_update_all();         // TODO(step 4d: remove)

		// Force the explosion animation without collision. TODO(step 4d: remove)
		if ((g_FrameCounter % 128) == 0)  lane_start_state_exploding(0, 0, 0); // small, lane 0  TODO(step 4d: remove)
		if ((g_FrameCounter % 128) == 64) lane_start_state_exploding(0, 1, 1); // big,   lane 1  TODO(step 4d: remove)
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

	TestLanesStep4b();   // TODO(step 4d: remove) — wave + frame loop + forced explosions
}
