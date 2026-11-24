#pragma once

#include "hud.h"
#include "tiles.h"


#define SCORE_DRAW()     TILES_PUT_BUFFER(HUD_SCORE_COL, HUD_ROW, score, HUD_DIGITS)


extern u8* score;
extern u8 score_credits;

void score_init(void);
void score_add(u8 points);
u8 score_check_hi(void);
void score_promote_hi(void);
