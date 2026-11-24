#pragma once

#include "msxgl.h"


extern u8 torpedo_next;

// Launchers, the argument of torpedo_fire(). The centre gun needs the acropolis.
#define TORPEDO_LAUNCHER_LEFT   0
#define TORPEDO_LAUNCHER_CENTER 1
#define TORPEDO_LAUNCHER_RIGHT  2

typedef struct
{
    struct VDP_Sprite* sprite;
    u8 sprite_number;
    u8 is_active;
    i8 dx;
    i8 dy;
} torpedo_t;

extern torpedo_t torpedoes[2];

void torpedo_init_all(void);
void torpedo_update_all(void);
void torpedo_fire(u8 launcher);     // TORPEDO_LAUNCHER_*
void torpedo_hide(u8 index);        // deactivate torpedo `index` and park it off screen
