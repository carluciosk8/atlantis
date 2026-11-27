#pragma once

#include "msxgl.h"


// Sprite pattern data: const tables only, no code. platform/sprites.c copies
// them to VRAM at boot.

// Gorgon patterns, 16x16 = 32 bytes each, for the gorgons flying right
// (GORGON_ID_*_RIGHT). sprites_init() loads their mirrors for the gorgons
// flying left.
extern const u8 sprite_fighter1[32];
extern const u8 sprite_fighter2[32];
extern const u8 sprite_bomber1[32];
extern const u8 sprite_bomber2[32];
extern const u8 sprite_bandit1[32];
extern const u8 sprite_bandit2[32];

// Torpedo (3), explosion (5 x 2 colours) and deathray (4 slots) patterns: 17
// 16x16 patterns of 32 bytes, in the order loaded from 16x16 pattern index 12.
#define SPRITE_FX_COUNT         17
extern const u8 sprite_patterns_fx[SPRITE_FX_COUNT * 32];

// Explosion animation pattern tables (8x8 pattern indices, 16x16 = multiple of 4).
extern const u8 sprites_explosion_anim1[5];
extern const u8 sprites_explosion_anim2[5];
