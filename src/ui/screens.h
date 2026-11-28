#pragma once

#include "msxgl.h"


// Screens only draw text and tiles. The game flow (flow.c) sequences them and
// runs the frames in between, except for the title blink below.

void screens_draw_title(void);
void screens_animate_title(void);       // waves, generators and stars; call every frame
void screens_blink_title(u8 cycles);   // blink "HIT TRIGGER TO START" (14 frames per cycle)

void screens_draw_wave_banner(void);    // " WAVE nnn " from wave_text
void screens_draw_pause_banner(void);   // "  PAUSED  ", same place and size
void screens_clear_banner(void);        // restore the sky behind either banner

void screens_draw_game_over(void);
void screens_draw_hi_score(void);
