#include "gorgons.h"

// Patterns are in 8x8 units (16x16 pattern index * 4).
const gorgon_t g_gorgon_list[GORGON_ID_TOTAL] =
{
//  { pattern1, pattern2, color1,             color2             },
    {  0,        4,       COLOR_BLACK,        COLOR_DARK_GREEN   },    // GORGON_ID_FIGHTER_RIGHT
    {  8,       12,       COLOR_BLACK,        COLOR_DARK_GREEN   },    // GORGON_ID_FIGHTER_LEFT
    { 16,       20,       COLOR_BLACK,        COLOR_DARK_RED     },    // GORGON_ID_BOMBER_RIGHT
    { 24,       28,       COLOR_BLACK,        COLOR_DARK_RED     },    // GORGON_ID_BOMBER_LEFT
    { 32,       36,       COLOR_BLACK,        COLOR_DARK_BLUE    },    // GORGON_ID_BANDIT_RIGHT
    { 40,       44,       COLOR_BLACK,        COLOR_DARK_BLUE    },    // GORGON_ID_BANDIT_LEFT
};
