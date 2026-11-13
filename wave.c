#include "wave.h"

#include "city.h"
#include "difficulty.h"
#include "gorgons.h"
#include "lane.h"
#include "score.h"
#include "sprites.h"
#include "tiles.h"


u8 wave_gorgons[WAVE_MAX_SIZE];
i8 wave_velocities[WAVE_MAX_SIZE];
i8 g_wave_size;

u8 wave_text[] = { ' ','W','A','V','E',' ','0','0','0',' ' };


void wave_init()
{
    wave_text[6] = '0';
    wave_text[7] = '0';
    wave_text[8] = '1';

    g_wave_size = 4;
}


void wave_draw_hud()
{
    const u8 border[] = { ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };
    TILES_PUT_BUFFER(11, 5, border, sizeof(border));
    TILES_PUT_BUFFER(11, 6, wave_text, sizeof(wave_text));
    TILES_PUT_BUFFER(11, 7, border, sizeof(border));

    // Wait ~120 frames (2.0 s at 60 Hz, 2.4 s at 50 Hz).
    for (u16 frame = 0; frame < 120; frame++)
    {
        Halt();
        sprites_flush();
        map_update();
        // TODO(step 4c): torpedo_update_all();
    }

    // Restore the map tiles that the "WAVE" text overwrote (same as legacy: it
    // rewrites the first 10 bytes of map_background over the 3 text rows).
    VDP_WriteVRAM_16K(map_background, g_ScreenLayoutLow + 5*32 + 11, 10);
    VDP_WriteVRAM_16K(map_background, g_ScreenLayoutLow + 6*32 + 11, 10);
    VDP_WriteVRAM_16K(map_background, g_ScreenLayoutLow + 7*32 + 11, 10);
}


void wave_start()
{
    // TODO(step 7): srand(ubox_tick)  →  Math_SetRandomSeed16(g_FrameCounter).
    difficulty_adjust(&wave_text[6]);
    for (i8 i = 0; i < g_wave_size; i++)
    {
        wave_gorgons[i] = difficulty_get_random_gorgon();
        u8 vel = difficulty_get_random_speed();
        if (wave_gorgons[i] == GORGON_ID_BANDIT_LEFT || wave_gorgons[i] == GORGON_ID_BANDIT_RIGHT) vel++;
        wave_velocities[i] = (wave_gorgons[i] % 2) ? -vel : vel;
        queue_push(&(g_lanes[0]).queue, i);
    }
}


void wave_finish()
{
    for (u8 i = 0; i < 7; i++) if (map_have_installation[i]) score_increase_hundred(5);
    SCORE_DRAW();

    if ((map_have_installation[0] == 0) && (score_credits > 0))
    {
        map_create_instalation(0);
        score_credits--;
    }

    // TODO(step 4c): rebuild installations at the current wave edges using
    //   collision.c's left_index / right_index:
    //   while (score_credits > 0 && left_index > 1 && right_index < 6)
    //   {
    //       if (score_credits > 0 && left_index > 1)  { left_index--;  map_create_instalation(left_index);  score_credits--; }
    //       if (score_credits > 0 && right_index < 6) { right_index++; map_create_instalation(right_index); score_credits--; }
    //   }

    wave_text[8]++;
    if (wave_text[8] > '9')
    {
        wave_text[8] = '0';
        wave_text[7]++;
    }
    if (wave_text[7] > '9')
    {
        wave_text[7] = '0';
        wave_text[6]++;
    }
    if (wave_text[6] > '9')
    {
        wave_text[6] = '0';
        wave_text[8]++;
    }
}
