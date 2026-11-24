#pragma once

#include "msxgl.h"


// A gorgon is drawn with two overlaid sprites, one colour each.
typedef struct gorgon
{
    u8 pattern1;
    u8 pattern2;
    u8 color1;
    u8 color2;
} gorgon_t;


#define GORGON_ID_FIGHTER_RIGHT 0
#define GORGON_ID_FIGHTER_LEFT  1
#define GORGON_ID_BOMBER_RIGHT  2
#define GORGON_ID_BOMBER_LEFT   3
#define GORGON_ID_BANDIT_RIGHT  4
#define GORGON_ID_BANDIT_LEFT   5
#define GORGON_ID_TOTAL         6

// Odd ids (*_LEFT) enter at the right edge and fly to the left.
#define GORGON_MOVES_LEFT(id)   ((id) % 2)
#define GORGON_IS_BANDIT(id)    ((id) == GORGON_ID_BANDIT_LEFT || (id) == GORGON_ID_BANDIT_RIGHT)


extern const gorgon_t g_gorgon_list[GORGON_ID_TOTAL];
