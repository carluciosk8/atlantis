#pragma once

#include "tiles.h"

#include <ubox.h>


#define SCORE_DRAW()     TILES_PUT_BUFFER(9, 23, score, 6);


extern uint8_t* score;
extern uint8_t score_credits;

void score_init();
void score_increase_hundred(uint8_t digit);
void score_increase_thousand(uint8_t digit);
uint8_t score_check_hi();
void score_promote_hi();
