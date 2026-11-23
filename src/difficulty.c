#include "difficulty.h"

#include "gorgons.h"
#include "wave.h"


// Cumulative percentages for difficulty_get_random_speed(): a roll below
// speed2_limit gives speed 2, below speed3_limit speed 3, otherwise speed 4.
u8 difficulty_speed2_limit;
u8 difficulty_speed3_limit;
u8 difficulty_bandit_chance;    // percent


// Difficulty tiers: { wave size, speed2_limit, speed3_limit, bandit chance }.
static const u8 difficulty_tiers[6][4] =
{
    { 10, 100, 100,  0 },   // wave 1
    { 12,  50, 100, 15 },   // waves 2-4
    { 15,  33,  66, 20 },   // waves 5-9
    { 20,   0,  50, 20 },   // waves 10-19
    { 25,   0,  33, 20 },   // waves 20-29
    { 30,   0,   0, 20 },   // waves 30+
};


// Set the wave size and the speed/bandit odds from the 3 ASCII digits of the
// wave number. The tier is picked from the most significant digit down.
void difficulty_adjust(u8* wave_digits)
{
    u8 tier;

    if      (wave_digits[0] > '0' || wave_digits[1] > '2') tier = 5;
    else if (wave_digits[1] > '1') tier = 4;
    else if (wave_digits[1] > '0') tier = 3;
    else if (wave_digits[2] > '4') tier = 2;
    else if (wave_digits[2] > '1') tier = 1;
    else                           tier = 0;

    const u8* t = difficulty_tiers[tier];
    g_wave_size              = (i8)t[0];
    difficulty_speed2_limit  = t[1];
    difficulty_speed3_limit  = t[2];
    difficulty_bandit_chance = t[3];
}


u8 difficulty_get_random_speed(void)
{
    u8 roll = (u8)(Math_GetRandom16() % 100);

    if (roll < difficulty_speed2_limit) return 2;
    else if (roll < difficulty_speed3_limit) return 3;
    else return 4;
}


u8 difficulty_get_random_gorgon(void)
{
    u8 roll = (u8)(Math_GetRandom16() % 100);

    if (roll < difficulty_bandit_chance)
    {
        return GORGON_ID_BANDIT_RIGHT + (u8)(Math_GetRandom16() % 2);
    }
    return (u8)(Math_GetRandom16() % GORGON_ID_BANDIT_RIGHT);
}
