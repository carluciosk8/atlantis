#include "wave.h"

#include "city.h"
#include "collision.h"
#include "difficulty.h"
#include "gorgons.h"
#include "lane.h"
#include "score.h"
#include "sprites.h"
#include "tiles.h"
#include "torpedo.h"


// Positions of the 3 ASCII digits of the wave number inside wave_text.
#define WAVE_HUNDREDS   6
#define WAVE_TENS       7
#define WAVE_ONES       8


u8 wave_gorgons[WAVE_MAX_SIZE];
i8 wave_velocities[WAVE_MAX_SIZE];
i8 g_wave_size;

u8 wave_text[] = { ' ','W','A','V','E',' ','0','0','0',' ' };


void wave_init(void)
{
    wave_text[WAVE_HUNDREDS] = '0';
    wave_text[WAVE_TENS]     = '0';
    wave_text[WAVE_ONES]     = '1';

    g_wave_size = 4;
}


void wave_draw_hud(void)
{
    const u8 border[] = { ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };
    TILES_PUT_BUFFER(11, 5, border, sizeof(border));
    TILES_PUT_BUFFER(11, 6, wave_text, sizeof(wave_text));
    TILES_PUT_BUFFER(11, 7, border, sizeof(border));

    // Wait ~120 frames (2.0 s at 60 Hz, 2.4 s at 50 Hz).
    for (u8 frame = 0; frame < 120; frame++)
    {
        Halt();
        sprites_flush();
        map_update();
        torpedo_update_all();
    }

    // Restore the sky behind the text: the first 10 tiles of map_background
    // are plain sky.
    TILES_PUT_BUFFER(11, 5, map_background, 10);
    TILES_PUT_BUFFER(11, 6, map_background, 10);
    TILES_PUT_BUFFER(11, 7, map_background, 10);
}


void wave_start(void)
{
    Math_SetRandomSeed16(g_FrameCounter);
    difficulty_adjust(&wave_text[WAVE_HUNDREDS]);

    for (i8 i = 0; i < g_wave_size; i++)
    {
        wave_gorgons[i] = difficulty_get_random_gorgon();

        u8 speed = difficulty_get_random_speed();
        if (GORGON_IS_BANDIT(wave_gorgons[i])) speed++;
        wave_velocities[i] = GORGON_MOVES_LEFT(wave_gorgons[i]) ? -speed : speed;

        queue_push(&(g_lanes[0]).queue, i);
    }
}


void wave_finish(void)
{
    // Bonus for every installation still standing.
    for (u8 i = 0; i < MAP_INSTALLATION_COUNT; i++) if (map_have_installation[i]) score_add(5);
    SCORE_DRAW();

    // Spend the score credits rebuilding the city: the acropolis first, then
    // the installations just outside the current edges, alternating sides.
    if ((map_have_installation[MAP_ACROPOLIS_COMMAND_ID] == 0) && (score_credits > 0))
    {
        map_create_installation(MAP_ACROPOLIS_COMMAND_ID);
        score_credits--;
    }

    while (score_credits > 0 && (city_left_edge > MAP_LEFTMOST_ID || city_right_edge < MAP_RIGHTMOST_ID))
    {
        if (score_credits > 0 && city_left_edge > MAP_LEFTMOST_ID)
        {
            city_left_edge--;
            map_create_installation(city_left_edge);
            score_credits--;
        }
        if (score_credits > 0 && city_right_edge < MAP_RIGHTMOST_ID)
        {
            city_right_edge++;
            map_create_installation(city_right_edge);
            score_credits--;
        }
    }

    // Advance the 3-digit wave number (999 wraps to 001).
    wave_text[WAVE_ONES]++;
    if (wave_text[WAVE_ONES] > '9')
    {
        wave_text[WAVE_ONES] = '0';
        wave_text[WAVE_TENS]++;
    }
    if (wave_text[WAVE_TENS] > '9')
    {
        wave_text[WAVE_TENS] = '0';
        wave_text[WAVE_HUNDREDS]++;
    }
    if (wave_text[WAVE_HUNDREDS] > '9')
    {
        wave_text[WAVE_HUNDREDS] = '0';
        wave_text[WAVE_ONES]++;
    }
}
