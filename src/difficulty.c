#include "difficulty.h"

#include <stdlib.h>

#include "gorgons.h"
#include "wave.h"


uint8_t difficulty_t1;
uint8_t difficulty_t2;
uint8_t difficulty_bandit_prob;


void difficulty_adjust(uint8_t* p_wave)
{
    if (p_wave[0] == '0' && p_wave[1] == '0' && p_wave[2] == '1')
    {
        difficulty_t1 = 100;
        difficulty_t2 = 100;
        g_wave_size = 10;
        difficulty_bandit_prob = 0;
        return;
    }
    else if (p_wave[0] == '0' && p_wave[1] == '0' && p_wave[2] > '1')
    {
        difficulty_t1 = 50;
        difficulty_t2 = 100;
        g_wave_size = 12;
        difficulty_bandit_prob = 15;
        return;
    }
    else if (p_wave[0] == '0' && p_wave[1] == '0' && p_wave[2] > '4')
    {
        difficulty_t1 = 33;
        difficulty_t2 = 66;
        g_wave_size = 15;
        difficulty_bandit_prob = 20;
        return;
    }
    else if (p_wave[0] == '0' && p_wave[1] > '0')
    {
        difficulty_t1 = 0;
        difficulty_t2 = 50;
        g_wave_size = 20;
        return;
    }
    else if (p_wave[0] == '0' && p_wave[1] > '1')
    {
        difficulty_t1 = 0;
        difficulty_t2 = 33;
        g_wave_size = 25;
        return;
    }
    else if (p_wave[0] == '0' && p_wave[1] > '2')
    {
        difficulty_t1 = 0;
        difficulty_t2 = 0;
        g_wave_size = 30;
    }
}


uint8_t difficulty_get_random_speed()
{
    uint8_t rnd = rand() % 100;
    
    if (rnd < difficulty_t1) return 2;
    else if (rnd < difficulty_t2) return 3;
    else return 4;
}


uint8_t difficulty_get_random_gorgon()
{
    uint8_t rnd = rand() % 100;

    if (rnd < difficulty_bandit_prob)
    {
        return GORGON_ID_BANDIT_RIGHT + rand() % 2;
    }
    return rand() % GORGON_ID_BANDIT_RIGHT;
}
