#pragma once

#include "msxgl.h"


// Sprite attribute table, flushed to the SAT (g_SpriteAttributeLow, 0x1B00 in
// Screen 2) by sprites_flush(). Lower plane = higher priority (TMS9918A).
#define SPRITE_COUNT            17
#define SPRITE_PLANE_GORGON     0    // 0-7   gorgons (2 per lane)
#define SPRITE_PLANE_TORPEDO    8    // 8-9   torpedoes
#define SPRITE_PLANE_DEATHRAY   10   // 10-14 deathray (5 stacked 16x16 sprites)
#define SPRITE_PLANE_EXPLOSION  15   // 15-16 building explosion (2-colour overlay)

// A sprite parked at this Y is below the visible area.
#define SPRITE_Y_HIDDEN         192

// First deathray pattern, in 8x8 units (16x16 pattern index * 4).
#define SPRITE_PATTERN_DEATHRAY (25*4)

extern struct VDP_Sprite sprites_attributes[SPRITE_COUNT];


void sprites_init(void);                    // set 16x16 mode + load all sprite patterns
void sprites_hide_all(void);                // make every sprite transparent (not flushed)
void sprites_flush(void);                   // write sprites_attributes to the SAT
void sprites_set_magnification(bool on);    // sprite size x1 (false) / x2 (true), keeps 16x16
