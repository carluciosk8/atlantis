#include "flow.h"

#include "city.h"
#include "collision.h"
#include "controls.h"
#include "hud.h"
#include "lane.h"
#include "music.h"
#include "score.h"
#include "screens.h"
#include "sky.h"
#include "sprites.h"
#include "torpedo.h"
#include "wave.h"


// Debug test aid (compiled out when 0). 1 = set the hi-score to spaces at the
// start of each game, so any score (even 000000) beats it, because a space
// sorts below every ASCII digit, and the hi-score screen always shows.
#define TEST_FORCE_HISCORE 0

// What flow_play_frames() keeps running besides the city animation.
#define PLAY_LANES      0x01
#define PLAY_TORPEDOES  0x02
#define PLAY_COLLIDE    0x04


// Run `count` frames (count > 0) of the non-interactive game loop.
static void flow_play_frames(u16 count, u8 what)
{
    do
    {
        Halt();                    // wait for vblank
        sprites_flush();           // SAT flush right after vblank (intentional)
        map_update();
        if (what & PLAY_LANES)     lane_update_all();
        if (what & PLAY_TORPEDOES) torpedo_update_all();
        if (what & PLAY_COLLIDE)   collision_handle_torpedoes();
    } while (--count);
}


// Play one wave: until every gorgon has been shot down or has finished its run.
static void flow_play_wave(void)
{
    wave_start();
    sky_set(wave_sky);

    // Wave banner for ~120 frames (2.0 s at 60 Hz, 2.4 s at 50 Hz).
    screens_draw_wave_banner();
    flow_play_frames(120, PLAY_TORPEDOES);
    screens_clear_wave_banner();
    sky_refresh();

    while (g_wave_size > 0)
    {
        controls_update();
        Halt();                    // wait for vblank
        sprites_flush();           // SAT flush right after vblank (intentional)
        map_update();
        sky_update();
        lane_update_all();
        torpedo_update_all();
        collision_handle_torpedoes();
    }

    wave_finish();
}


// Animate the title until the fire button is pressed.
static void flow_wait_start(void)
{
    // First wait until a held button is released, then for a new press.
    while (controls_fire_held())  { Halt(); screens_animate_title(); }
    while (!controls_fire_held()) { Halt(); screens_animate_title(); }
}


void flow_run(void)
{
    for (;;)
    {
        music_play(MUSIC_TITLE);
        screens_draw_title();
        flow_wait_start();
        screens_blink_title();

        music_play(MUSIC_GAME);

        lane_init_all();
        score_init();
        map_init();
        torpedo_init_all();
        wave_init();

#if TEST_FORCE_HISCORE
        for (u8 i = 0; i < HUD_DIGITS; i++) hud[HUD_HISCORE_COL + i] = ' ';
#endif

        // Play waves while any installation is left.
        while (city_is_standing()) flow_play_wave();

        music_play(MUSIC_GAME_OVER);
        screens_draw_game_over();

        // Keep the game running for ~270 frames (4.5 s at 60 Hz, 5.4 s at 50 Hz).
        flow_play_frames(270, PLAY_LANES | PLAY_TORPEDOES | PLAY_COLLIDE);
        sprites_hide_all();
        sprites_flush();

        if (score_check_hi())
        {
            music_play(MUSIC_HI_SCORE);
            score_promote_hi();
            screens_draw_hi_score();

            // Wait ~480 frames (8.0 s at 60 Hz, 9.6 s at 50 Hz).
            flow_play_frames(480, 0);
        }
    }
}
