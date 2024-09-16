#pragma once

#include "ubox.h"


extern uint8_t torpedo_current;
extern uint8_t torpedo_is_fired;

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
    struct sprite_attr* sprite;
    uint8_t sprite_number;
    uint8_t is_active;
    int8_t dx;
    int8_t dy;
} torpedo_t;

extern torpedo_t torpedos[2];

void torpedo_init_all();
void torpedo_update_all();
void torpedo_fire(uint8_t pattern, uint8_t x, uint8_t y, int8_t dx, int8_t dy);
