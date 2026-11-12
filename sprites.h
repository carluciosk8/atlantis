#pragma once

#include "msxgl.h"


// Sprite attribute table (17 sprites, 68 bytes), flushed to VRAM at
// g_SpriteAttributeLow (0x1B00 in Screen 2) by sprites_flush().
//
// Plane order (lower plane = higher priority, per TMS9918A):
//   0-7   gorgons (2 per lane)
//   8-9   torpedoes
//   10-14 deathray (5 stacked 16x16 sprites)
//   15-16 building explosion (2-color overlay)
extern struct VDP_Sprite sprites_attributes[17];

// Explosion animation pattern tables (8x8 pattern indices, 16x16 = multiple of 4).
extern const u8 sprites_explosion_anim1[];
extern const u8 sprites_explosion_anim2[];


void init_sprites();           // set 16x16 mode + load all sprite patterns
void sprites_flush();          // write sprites_attributes to the SAT
void sprites_disable_deathray();
void sprites_update_deathray(u8 x, u8 frame);
