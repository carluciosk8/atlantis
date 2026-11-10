#pragma once

#include "tiles.h"


#define SCORE_DRAW()     TILES_PUT_BUFFER(9, 23, score, 6);


extern u8* score;
extern u8 score_credits;

void score_init();
void score_increase_hundred(u8 digit);
void score_increase_thousand(u8 digit);
u8 score_check_hi();
void score_promote_hi();
