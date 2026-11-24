#include "wave.h"

#include "city.h"
#include "collision.h"
#include "difficulty.h"
#include "frame.h"
#include "gorgons.h"
#include "lane.h"
#include "score.h"


// Positions of the 3 ASCII digits of the wave number inside wave_text.
#define WAVE_HUNDREDS   6
#define WAVE_TENS       7
#define WAVE_ONES       8


u8 wave_gorgons[WAVE_MAX_SIZE];
i8 wave_velocities[WAVE_MAX_SIZE];
i8 g_wave_size;

u8 wave_text[WAVE_TEXT_SIZE] = { ' ','W','A','V','E',' ','0','0','0',' ' };


void wave_init(void)
{
    wave_text[WAVE_HUNDREDS] = '0';
    wave_text[WAVE_TENS]     = '0';
    wave_text[WAVE_ONES]     = '1';
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

    // Spend the score credits rebuilding the city.
    score_credits = city_rebuild(score_credits);

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
