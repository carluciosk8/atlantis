// ____________________________
//  Atlantis — MSXgl (Step 3: frame loop + interrupts + tick)
//
//  Sets Screen 2, loads the tileset, installs the vblank H_TIMI hook that
//  increments the frame counter, and runs a Halt()-synchronized frame loop that
//  animates the map (water + generators). Sprites, music, input and RNG are
//  ported in later steps.
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"

#include "city.h"
#include "hud.h"
#include "tiles.h"

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

//-----------------------------------------------------------------------------
/// Program entry point
void main(void)
{
	// Screen 2 (Graphic 2), 256×192
	VDP_SetMode(VDP_MODE_SCREEN2);
	VDP_ClearVRAM();

	init_tiles();

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

	// Frame loop: wait for vblank, then update the map animation (water +
	// generators). map_update() reads g_FrameCounter, so it advances in real
	// time regardless of how long a frame body takes.
	for (;;)
	{
		Halt();
		map_update();
	}
}
