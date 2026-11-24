#pragma once

#include "msxgl.h"


// Fixed screen text and the title logo: const tables only, no code. Text is kept
// as brace lists of characters, so it lives in ROM and is never built on the stack.

// Title logo, tileset tile indices, one array per row (drawn by ui/screens.c).
extern const u8 title_logo_row1[2];
extern const u8 title_logo_row2[23];
extern const u8 title_logo_row3[24];
extern const u8 title_logo_row4[24];

// Title text.
extern const u8 title_press_start[20];
extern const u8 title_copyright[26];
extern const u8 title_blank[20];

// Hi-score screen, 3 lines.
extern const u8 text_hi_score_1[19];
extern const u8 text_hi_score_2[19];
extern const u8 text_hi_score_3[19];

// Game-over screen.
extern const u8 text_game_over[11];
