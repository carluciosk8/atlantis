#include "gorgons.h"

const gorgon_t g_gorgon_list[GORGON_ID_TOTAL] = 
{
//  { patt1,  patt2,  attr1,  attr2 },  
    {     0,      4,      1,     12 },    // GORGON_ID_FIGHTER_RIGHT
    {     8,     12,      1,     12 },    // GORGON_ID_FIGHTER_LEFT
    {    16,     20,      1,      6 },    // GORGON_ID_BOMBER_RIGHT
    {    24,     28,      1,      6 },    // GORGON_ID_BOMBER_LEFT
    {    32,     36,      1,      4 },    // GORGON_ID_BANDIT_RIGHT
    {    40,     44,      1,      4 },    // GORGON_ID_BANDIT_LEFT
};
