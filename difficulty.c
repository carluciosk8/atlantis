#include "difficulty.h"

#include "gorgons.h"
#include "wave.h"

// The embedded SDCC's <stdlib.h> is not on the default include path (the build
// tool adds -I tools/ but not -I tools/sdcc/include), so declare the prototype
// here instead. rand() links from the SDCC standard library (z80.lib).
// TODO(step 7): replace rand() with Math_GetRandom8() / Math_GetRandomMax8().
int rand(void);


u8 difficulty_t1;
u8 difficulty_t2;
u8 difficulty_bandit_prob;


void difficulty_adjust(u8* p_wave)
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


u8 difficulty_get_random_speed()
{
    u8 rnd = rand() % 100;
    
    if (rnd < difficulty_t1) return 2;
    else if (rnd < difficulty_t2) return 3;
    else return 4;
}


u8 difficulty_get_random_gorgon()
{
    u8 rnd = rand() % 100;

    if (rnd < difficulty_bandit_prob)
    {
        return GORGON_ID_BANDIT_RIGHT + rand() % 2;
    }
    return rand() % GORGON_ID_BANDIT_RIGHT;
}
