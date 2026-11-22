#pragma once

#include "tiles.h"


#define SCORE_DRAW()     TILES_PUT_BUFFER(9, 23, score, 6);


extern u8* score;
extern u8 score_credits;

void score_init();
void score_add(u8 points);
u8 score_check_hi();
void score_promote_hi();
