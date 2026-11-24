#pragma once

#include "msxgl.h"

#include "city_data.h"


extern u8 map_have_installation[];

// The outermost installation still standing on each side of the city (the
// deathray's next target from that side). Everything but the acropolis is gone
// once city_left_edge > city_right_edge. Reset by map_init().
extern u8 city_left_edge;
extern u8 city_right_edge;

// The city is alive while any installation is left.
#define city_is_standing() \
    (city_left_edge <= city_right_edge || map_have_installation[MAP_ACROPOLIS_COMMAND_ID])


void map_init(void);
void map_destroy_installation(u8 index);
void map_start_explosion(u8 x, u8 y);
void map_update(void);

// Spend credits rebuilding the city: the acropolis first, then the installations
// just outside the current edges, alternating sides. Returns the unspent credits.
u8 city_rebuild(u8 credits);
