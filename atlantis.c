// ____________________________
//  Atlantis — MSXgl
//
//  Boot (Screen 2, tiles, sprite patterns, music, vblank hook) and the main
//  game flow: title screen, waves, game over and hi-score, looping forever.
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"

#include "arkos/akm_player.h"   // AKM_Update (called by the H_TIMI hook)

#include "city.h"
#include "collision.h"
#include "hud.h"
#include "controls.h"
#include "lane.h"
#include "music.h"
#include "score.h"
#include "sprites.h"
#include "tiles.h"
#include "torpedo.h"
#include "wave.h"

// Debug test aid (compiled out when 0). 1 = set the hi-score to spaces at the
// start of each game, so any score (even 000000) beats it, because a space
// sorts below every ASCII digit, and the hi-score screen always shows.
#define TEST_FORCE_HISCORE 0

//=============================================================================
// FRAME LOOP
//=============================================================================

// Frame counter, incremented once per vblank by the H_TIMI hook below.
volatile u8 g_FrameCounter;

//-----------------------------------------------------------------------------
// H_TIMI hook: called by the BIOS KEYINT ISR once per vblank. Increments the
// frame counter, then advances the music player. The BIOS KEYINT has already
// saved all registers around the hook, but expects A (a copy of VDP status
// register S#0) to be preserved for STATFL, so this __naked wrapper pushes AF
// around its work. It must not touch the VDP.
static void MusicFrameHook(void) __naked
{
    __asm
        push    af
        ld      hl, #_g_FrameCounter
        inc     (hl)
        call    _AKM_Update
        pop     af
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
    BIOS_SetKeyClick(FALSE);

    tiles_init();
    sprites_init();      // after VDP_ClearVRAM, which wipes the pattern table

    // The hook is installed only after the player is initialised, and with
    // interrupts disabled so a vblank cannot run a half-written hook
    // (BIOS_SetHookCallback does not DI/EI itself).
    g_FrameCounter = 0;
    VDP_EnableVBlank(TRUE);
    music_init();
    DisableInterrupt();
    BIOS_SetHookCallback(H_TIMI, MusicFrameHook);
    EnableInterrupt();

    for (;;)
    {
        music_play(MUSIC_TITLE);
        hud_show_title_screen();

        music_play(MUSIC_GAME);

        collision_init();
        lane_init_all();
        score_init();
        map_init();
        torpedo_init_all();
        wave_init();

#if TEST_FORCE_HISCORE
        for (u8 i = 0; i < HUD_DIGITS; i++) hud[HUD_HISCORE_COL + i] = ' ';
#endif

        // Play waves while any installation is left.
        while (city_left_edge <= city_right_edge || map_have_installation[MAP_ACROPOLIS_COMMAND_ID])
        {
            wave_start();
            wave_draw_hud();

            while (g_wave_size > 0)
            {
                controls_update();
                Halt();                    // wait for vblank
                sprites_flush();           // SAT flush right after vblank (intentional)
                map_update();
                lane_update_all();
                torpedo_update_all();
                collision_handle_torpedoes();
            }

            wave_finish();
        }

        music_play(MUSIC_GAME_OVER);
        hud_draw_game_over();

        if (score_check_hi())
        {
            music_play(MUSIC_HI_SCORE);
            score_promote_hi();
            hud_draw_hi_score();
        }
    }
}
