#pragma once

#include "msxgl.h"


extern u8 torpedo_current;
extern u8 torpedo_is_fired;

// Torpedo launch constants. The *_PATT values name the LAUNCHER side, not the
// pointing direction: TORPEDO_LEFT_PATT (13*4) is launched from the left
// launcher and is drawn pointing right (pattern "torpedo_right"), and
// TORPEDO_RIGHT_PATT (14*4) from the right launcher pointing left. Copied
// verbatim from the legacy — do not rename or "fix". Pattern values are already
// in 8x8 units (16x16 pattern index * 4), matching the sprite attribute layout.
#define TORPEDO_LEFT_PATT   13*4
#define TORPEDO_LEFT_X      14
#define TORPEDO_LEFT_Y      101
#define TORPEDO_LEFT_DX     4
#define TORPEDO_LEFT_DY     -2

#define TORPEDO_CENTER_PATT 12*4
#define TORPEDO_CENTER_X    126
#define TORPEDO_CENTER_Y    93
#define TORPEDO_CENTER_DX   0
#define TORPEDO_CENTER_DY   -3

#define TORPEDO_RIGHT_PATT  14*4
#define TORPEDO_RIGHT_X     238
#define TORPEDO_RIGHT_Y     94
#define TORPEDO_RIGHT_DX    -4
#define TORPEDO_RIGHT_DY    -2

typedef struct
{
    struct VDP_Sprite* sprite;
    u8 sprite_number;
    u8 is_active;
    i8 dx;
    i8 dy;
} torpedo_t;

extern torpedo_t torpedos[2];

void torpedo_init_all();
void torpedo_update_all();
void torpedo_fire(u8 pattern, u8 x, u8 y, i8 dx, i8 dy);
