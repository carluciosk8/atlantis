// ____________________________
//  Atlantis — MSXgl (Step 5: music + SFX)
//
//  Sets Screen 2, loads the tileset and sprite patterns, installs the vblank
//  H_TIMI hook (which increments the frame counter and advances the Arkos AKM
//  music player), and runs the full legacy game flow: title screen, wave loop
//  (lanes, torpedoes, collisions, score, building explosions, deathray), game
//  over and hi-score screens, looping forever. Input is a stub (Step 6) and the
//  RNG seed is still pending (Step 7).
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"

#include "arkos/akm_player.h"   // AKM_Update (called by the H_TIMI hook)

#include "city.h"
#include "collision.h"
#include "hud.h"
#include "input.h"
#include "lane.h"
#include "music.h"
#include "score.h"
#include "sprites.h"
#include "tiles.h"
#include "torpedo.h"
#include "wave.h"

// Temporary test aid (Step 4d only). 1 = force the hi-score branch at game
// over by setting the hi-score (hud[23..28]) to spaces at the start of each
// game, so the score "000000" always beats it (spaces sort below every ASCII
// digit). Lets the hi-score screen be reached even with TEST_AUTOFIRE 0 (the
// score stays at 0). TODO(step 6: remove).
#define TEST_FORCE_HISCORE 0

//=============================================================================
// FRAME LOOP
//=============================================================================

// Frame counter, incremented once per vblank by the H_TIMI hook below.
// 8-bit (wraps every 256 frames), like the legacy ubox_tick. Also zeroed
// explicitly in main() (legacy ubox_reset_tick()).
volatile u8 g_FrameCounter;

//-----------------------------------------------------------------------------
// H_TIMI hook: called by the BIOS KEYINT ISR once per vblank. Increments the
// frame counter, then advances the music player. The BIOS KEYINT has already
// saved all registers around the hook, but expects A (a copy of VDP status
// register S#0) to be preserved for STATFL, so this __naked wrapper pushes AF
// around its work (matching the legacy ubox_isr's push af / pop af). It must
// not touch the VDP.
static void MusicFrameHook(void) __naked
{
	__asm
		push	af
		ld		hl, #_g_FrameCounter
		inc		(hl)
		call	_AKM_Update
		pop		af
		ret
	__endasm;
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

	// Enable the vblank interrupt, then initialise the music player (copy the
	// song/SFX to 0xD000/0xE000, start the game sub-song) and finally install
	// the merged H_TIMI hook that increments the frame counter and advances the
	// music. The hook is installed only after the player is initialised, and the
	// 4-byte hook write is done with interrupts disabled so a vblank cannot run
	// a half-written hook (BIOS_SetHookCallback / BIOS_SetHookInterSlotCallback
	// do not DI/EI themselves).
	g_FrameCounter = 0;
	VDP_EnableVBlank(TRUE);
	music_init();
	DisableInterrupt();
	BIOS_SetHookCallback(H_TIMI, MusicFrameHook);
	EnableInterrupt();

	for (;;)
	{
		music_play(1);
		hud_show_title_screen();

		music_play(0);

		collision_init();
		lane_init_all();
		map_init();
		score_init();
		torpedo_init_all();
		wave_init();

#if TEST_FORCE_HISCORE
		// TODO(step 6: remove) — test aid: set the hi-score (hud[23..28]) to
		// TODO(step 6: remove) — spaces so the score "000000" beats it (spaces
		// TODO(step 6: remove) — < '0' in ASCII) and score_check_hi() returns 1
		// TODO(step 6: remove) — even with TEST_AUTOFIRE 0 (score stays at 0).
		for (u8 i = 0; i < 6; i++) hud[23 + i] = ' ';  // TODO(step 6: remove)
#endif

		while (left_index <= right_index || map_have_installation[0])
		{
			wave_start();
			wave_draw_hud();

			while (g_wave_size > 0)
			{
				handle_input();
				Halt();                    // wait for vblank
				sprites_flush();           // SAT flush right after vblank (intentional)
				map_update();
				lane_update_all();
				torpedo_update_all();
				collision_handle_torpedos();
			}

			wave_finish();
		}

		music_play(2);
		hud_draw_game_over();

		if (score_check_hi())
		{
			music_play(3);
			score_promote_hi();
			hud_draw_hi_score();
		}
	}
}
