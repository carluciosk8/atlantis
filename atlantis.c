// ____________________________
//  Atlantis — MSXgl (Step 4d: full game flow)
//
//  Sets Screen 2, loads the tileset and sprite patterns, installs the vblank
//  H_TIMI hook, and runs the full legacy game flow: title screen, wave loop
//  (lanes, torpedoes, collisions, score, building explosions, deathray), game
//  over and hi-score screens, looping forever. Input is a stub (Step 6) and
//  music is left as TODO(step 5) comments.
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"

#include "city.h"
#include "collision.h"
#include "hud.h"
#include "input.h"
#include "lane.h"
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

	// Music boot setup (legacy main.c:30-32).
	// TODO(step 5): mplayer_init(SONG, 0);            →  AKM_Play(0, (void*)0xD000);
	// TODO(step 5): mplayer_init_effects(EFFECTS);    →  AKM_InitSFX((void*)0xE000);
	// TODO(step 5): ubox_set_user_isr(mplayer_play);  →  AKM_Update() per frame (Step 5);

	for (;;)
	{
		// TODO(step 5): mplayer_init(SONG, 1);  →  AKM_Play(1, (void*)0xD000);
		hud_show_title_screen();

		// TODO(step 5): mplayer_init(SONG, 0);  →  AKM_Play(0, (void*)0xD000);

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

		// TODO(step 5): mplayer_init(SONG, 2);  →  AKM_Play(2, (void*)0xD000);
		hud_draw_game_over();

		if (score_check_hi())
		{
			// TODO(step 5): mplayer_init(SONG, 3);  →  AKM_Play(3, (void*)0xD000);
			score_promote_hi();
			hud_draw_hi_score();
		}
	}
}
