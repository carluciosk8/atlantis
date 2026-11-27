#pragma once

#include "msxgl.h"


// Fixed screen text and the title logo: const tables only, no code. Text is kept
// as brace lists of characters, so it lives in ROM and is never built on the stack.

// Title logo, tileset tile indices, one array per row (drawn by ui/screens.c).
extern const u8 title_logo_row1[2];
extern const u8 title_logo_row2[23];
extern const u8 title_logo_row3[24];
extern const u8 title_logo_row4[24];

// Title backdrop: stars on the black sky ({ x, y, tile } name-table cells),
// the animated waves at name-table row 19 and the 3-row seabed strip below them,
// with an animated generator near each end.
#define TITLE_STAR_COUNT       34
#define TITLE_WAVE_ROW         19
#define TITLE_SEABED_ROW       20
#define TITLE_SEABED_ROWS      3
#define TITLE_GENERATOR_ROW    21
#define TITLE_GENERATOR1_X     3
#define TITLE_GENERATOR2_X     28
extern const u8 title_stars[TITLE_STAR_COUNT][3];
extern const u8 title_seabed[TITLE_SEABED_ROWS*32];

// Title text.
extern const u8 title_press_start[20];
extern const u8 title_copyright[26];

// Hi-score screen, 3 lines.
extern const u8 text_hi_score_1[19];
extern const u8 text_hi_score_2[19];
extern const u8 text_hi_score_3[19];

// Game-over screen.
extern const u8 text_game_over[11];
