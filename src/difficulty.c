#include "difficulty.h"

#include "gorgons.h"
#include "wave.h"


// Cumulative percentages for difficulty_get_random_speed(): a roll below
// speed2_limit gives speed 2, below speed3_limit speed 3, otherwise speed 4.
u8 difficulty_speed2_limit;
u8 difficulty_speed3_limit;
u8 difficulty_bandit_chance;    // percent


// Set the wave size and the speed/bandit odds from the 3 ASCII digits of the
// wave number.
// NOTE: the "> '4'", "[1] > '1'" and "[1] > '2'" branches are unreachable,
// because an earlier, wider test always matches first.
void difficulty_adjust(u8* wave_digits)
{
    if (wave_digits[0] == '0' && wave_digits[1] == '0' && wave_digits[2] == '1')
    {
        difficulty_speed2_limit = 100;
        difficulty_speed3_limit = 100;
        g_wave_size = 10;
        difficulty_bandit_chance = 0;
    }
    else if (wave_digits[0] == '0' && wave_digits[1] == '0' && wave_digits[2] > '1')
    {
        difficulty_speed2_limit = 50;
        difficulty_speed3_limit = 100;
        g_wave_size = 12;
        difficulty_bandit_chance = 15;
    }
    else if (wave_digits[0] == '0' && wave_digits[1] == '0' && wave_digits[2] > '4')
    {
        difficulty_speed2_limit = 33;
        difficulty_speed3_limit = 66;
        g_wave_size = 15;
        difficulty_bandit_chance = 20;
    }
    else if (wave_digits[0] == '0' && wave_digits[1] > '0')
    {
        difficulty_speed2_limit = 0;
        difficulty_speed3_limit = 50;
        g_wave_size = 20;
    }
    else if (wave_digits[0] == '0' && wave_digits[1] > '1')
    {
        difficulty_speed2_limit = 0;
        difficulty_speed3_limit = 33;
        g_wave_size = 25;
    }
    else if (wave_digits[0] == '0' && wave_digits[1] > '2')
    {
        difficulty_speed2_limit = 0;
        difficulty_speed3_limit = 0;
        g_wave_size = 30;
    }
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
