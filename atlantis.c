// ____________________________
//  Atlantis — MSXgl (Step 4a: sprites)
//
//  Sets Screen 2, loads the tileset and sprite patterns, installs the vblank
//  H_TIMI hook, and runs a Halt()-synchronized frame loop that animates the map
//  (water + generators) while a temporary test displays every sprite pattern.
//  Music, input and RNG are ported in later steps.
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"

#include "city.h"
#include "hud.h"
#include "sprites.h"
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

//=============================================================================
// TEMPORARY SPRITE TEST — Step 4a only, remove in Step 4b. TODO(step 4b: remove)
//=============================================================================
// Shows all 29 sprite patterns on screen so the pattern data and the horizontal
// flips can be checked in the emulator. It uses its own 29-entry attribute
// array (the game's sprites_attributes has only 17 entries) and flushes it
// straight to the SAT at g_SpriteAttributeLow. Rows are >= 16 px apart and hold
// at most 4 sprites each (the MSX1 scanline limit). Each flipped pattern sits
// next to its unflipped sibling; 2-color overlays are two sprites at the same
// position with the legacy colours. TODO(step 4b: remove)
static void TestSpritesStep4a(void) // TODO(step 4b: remove)
{
	static const struct VDP_Sprite attrs[29] = // TODO(step 4b: remove)
	{
		// Row 1 (Y=8) — gorgon fighter, colours 1 & 12: right then flipped left. TODO(step 4b: remove)
		{  8,  0,  0,  1 }, // fighter1            TODO(step 4b: remove)
		{  8,  0,  4, 12 }, // fighter2            TODO(step 4b: remove)
		{  8, 32,  8,  1 }, // fighter1 flipped    TODO(step 4b: remove)
		{  8, 32, 12, 12 }, // fighter2 flipped    TODO(step 4b: remove)
		// Row 2 (Y=32) — gorgon bomber, colours 1 & 6. TODO(step 4b: remove)
		{ 32,  0, 16,  1 }, // bomber1             TODO(step 4b: remove)
		{ 32,  0, 20,  6 }, // bomber2             TODO(step 4b: remove)
		{ 32, 32, 24,  1 }, // bomber1 flipped     TODO(step 4b: remove)
		{ 32, 32, 28,  6 }, // bomber2 flipped     TODO(step 4b: remove)
		// Row 3 (Y=56) — gorgon bandit, colours 1 & 4. TODO(step 4b: remove)
		{ 56,  0, 32,  1 }, // bandit1             TODO(step 4b: remove)
		{ 56,  0, 36,  4 }, // bandit2             TODO(step 4b: remove)
		{ 56, 32, 40,  1 }, // bandit1 flipped     TODO(step 4b: remove)
		{ 56, 32, 44,  4 }, // bandit2 flipped     TODO(step 4b: remove)
		// Row 4 (Y=80) — torpedoes, colour 1. TODO(step 4b: remove)
		{ 80,  0, 48,  1 }, // torpedo center      TODO(step 4b: remove)
		{ 80, 40, 52,  1 }, // torpedo right       TODO(step 4b: remove)
		{ 80, 80, 56,  1 }, // torpedo left        TODO(step 4b: remove)
		// Row 5 (Y=104) — explosion frames 0-1, colours 9 & 11. TODO(step 4b: remove)
		{104,  0, 60,  9 }, // explosion 0 color 9  TODO(step 4b: remove)
		{104,  0, 64, 11 }, // explosion 0 color 11 TODO(step 4b: remove)
		{104, 40, 68,  9 }, // explosion 1 color 9  TODO(step 4b: remove)
		{104, 40, 72, 11 }, // explosion 1 color 11 TODO(step 4b: remove)
		// Row 6 (Y=128) — explosion frames 2-3. TODO(step 4b: remove)
		{128,  0, 76,  9 }, // explosion 2 color 9  TODO(step 4b: remove)
		{128,  0, 80, 11 }, // explosion 2 color 11 TODO(step 4b: remove)
		{128, 40, 84,  9 }, // explosion 3 color 9  TODO(step 4b: remove)
		{128, 40, 88, 11 }, // explosion 3 color 11 TODO(step 4b: remove)
		// Row 7 (Y=152) — explosion frame 4 + deathray frames 0-1. TODO(step 4b: remove)
		{152,  0, 92,  9 }, // explosion 4 color 9  TODO(step 4b: remove)
		{152,  0, 96, 11 }, // explosion 4 color 11 TODO(step 4b: remove)
		{152, 40, 100, 8 }, // deathray frame 0     TODO(step 4b: remove)
		{152, 80, 104, 9 }, // deathray frame 1     TODO(step 4b: remove)
		// Row 8 (Y=176) — deathray frames 2-3. TODO(step 4b: remove)
		{176,  0, 108, 10 }, // deathray frame 2    TODO(step 4b: remove)
		{176, 40, 112, 11 }, // deathray frame 3    TODO(step 4b: remove)
	}; // TODO(step 4b: remove)
	VDP_WriteVRAM_16K((const u8*)attrs, g_SpriteAttributeLow, sizeof(attrs)); // TODO(step 4b: remove)
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

	TestSpritesStep4a(); // TODO(step 4b: remove) — show every sprite pattern

	// Frame loop: wait for vblank, then update the map animation (water +
	// generators). map_update() reads g_FrameCounter, so it advances in real
	// time regardless of how long a frame body takes.
	for (;;)
	{
		Halt();
		map_update();
	}
}
