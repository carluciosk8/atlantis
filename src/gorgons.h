#pragma once

#include "ubox.h"


typedef struct gorgon
{
    uint8_t pattern1;
    uint8_t pattern2;
    uint8_t attr1;
    uint8_t attr2;
} gorgon_t;


#define GORGON_ID_FIGHTER_RIGHT 0
#define GORGON_ID_FIGHTER_LEFT  1
#define GORGON_ID_BOMBER_RIGHT  2
#define GORGON_ID_BOMBER_LEFT   3
#define GORGON_ID_BANDIT_RIGHT  4
#define GORGON_ID_BANDIT_LEFT   5
#define GORGON_ID_TOTAL         6


extern const gorgon_t g_gorgon_list[GORGON_ID_TOTAL];
