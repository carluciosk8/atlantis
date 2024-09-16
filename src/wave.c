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

#include <stdlib.h>


uint8_t wave_gorgons[WAVE_MAX_SIZE];
int8_t  wave_velocities[WAVE_MAX_SIZE];
int8_t g_wave_size;

uint8_t wave_text[] =   { ' ','W','A','V','E',' ','0','0','0',' ' };


void wave_init()
{
    wave_text[6] = '0';
    wave_text[7] = '0';
    wave_text[8] = '1';

    g_wave_size = 4;
}


void wave_draw_hud()
{
    const uint8_t border[] = { ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };
    TILES_PUT_BUFFER(11, 5, border, sizeof(border));
    TILES_PUT_BUFFER(11, 6, wave_text, sizeof(wave_text));
    TILES_PUT_BUFFER(11, 7, border, sizeof(border));

    int frame_counter = 0;
    while (frame_counter < 120)
    {
        ubox_wait_vsync();
        ubox_write_vm((uint8_t*)0x1b00, 68, (uint8_t*)sprites_attributes);
        map_update();
        torpedo_update_all();
        frame_counter++;
    }
    
    ubox_write_vm((uint8_t*)0x1800 + 5*32 + 11, 10, map_background);
    ubox_write_vm((uint8_t*)0x1800 + 6*32 + 11, 10, map_background);
    ubox_write_vm((uint8_t*)0x1800 + 7*32 + 11, 10, map_background);
}


void wave_start()
{
    srand(ubox_tick);
    difficulty_adjust(&wave_text[6]);
    for (int8_t i=0 ; i<g_wave_size ; i++)
    {
        wave_gorgons[i] = difficulty_get_random_gorgon();
        uint8_t vel = difficulty_get_random_speed();
        if (wave_gorgons[i] == GORGON_ID_BANDIT_LEFT || wave_gorgons[i] == GORGON_ID_BANDIT_RIGHT) vel++;
        wave_velocities[i] = (wave_gorgons[i] % 2) ? -vel : vel;
        queue_push(&(g_lanes[0]).queue, i);
    }
}


void wave_finish()
{
    for (uint8_t i=0 ; i<7 ; i++) if (map_have_installation[i]) score_increase_hundred(5);
    SCORE_DRAW();

    if ( (map_have_installation[0] == 0) && (score_credits > 0) )
    {
        map_create_instalation(0);
        score_credits--;
    }

    while ( score_credits > 0 && left_index > 1 && right_index < 6)
    {
        if (score_credits > 0 && left_index > 1)
        {
            left_index--;
            map_create_instalation(left_index);
            score_credits--;
        }

        if (score_credits > 0 && right_index < 6)
        {
            right_index++;
            map_create_instalation(right_index);
            score_credits--;
        }
    }

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

// void ios_inc(uint8_t* ios, uint8_t s, uint8_t e)
// {
//     // ios[s]++
//     // for (int i=s ; i<e ; --i)
//     // {
//     //     if (ios[s] > '9')
//     //     {
//     //         ios[s] = '0';
//     //         ios[s-1]++;
//     //     }
//     // }
// }