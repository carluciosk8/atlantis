#pragma once

#include "msxgl.h"


// The HUD is one 32-character text row at the bottom of the screen. The score
// and the hi-score live inside it as 6 ASCII digits each (see score.c).
#define HUD_ROW             23
#define HUD_WIDTH           32
#define HUD_SCORE_COL       9
#define HUD_HISCORE_COL     23
#define HUD_DIGITS          6

extern u8 hud[HUD_WIDTH];

