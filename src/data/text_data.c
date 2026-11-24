#include "text_data.h"


// Title logo, drawn with tileset tiles.
const u8 title_logo_row1[] = { 0, 1 };
const u8 title_logo_row2[] = { 2,3,4,5,6,7,8,32,32,2,3,4,9,6,4,5,6,4,5,6,7,5,6 };
const u8 title_logo_row3[] = { 11,12,13,32,32,13,15,16,17,11,12,13,15,18,13,32,32,13,32,19,32,20,21,13 };
const u8 title_logo_row4[] = { 22,23,24,32,32,24,26,27,24,22,23,24,26,28,24,32,32,24,26,27,24,26,27,24 };

const u8 title_press_start[] = { 'H','I','T',' ','T','R','I','G','G','E','R',' ','T','O',' ','S','T','A','R','T' };
const u8 title_copyright[]   = { '@',' ','2','0','2','1',' ','C','A','R','L','U','C','I','O',' ','S',' ','C','O','R','D','E','I','R','O' };
const u8 title_blank[]       = { ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };


// Hi-score screen, 3 lines of 19 characters.
const u8 text_hi_score_1[] = { ' ','C','O','N','G','R','A','T','U','L','A','T','I','O','N','S',' ','.',' ' };
const u8 text_hi_score_2[] = { ' ',' ','Y','O','U',' ','C','O','N','Q','U','E','R',' ','T','H','E',' ',' ' };
const u8 text_hi_score_3[] = { ' ',' ',' ',' ','H','I',' ','S','C','O','R','E',' ','.','.','.',' ',' ',' ' };
// Game-over screen.
const u8 text_game_over[] = { ' ','G','A','M','E',' ','O','V','E','R',' ' };
