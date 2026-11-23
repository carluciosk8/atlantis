#pragma once

#include "msxgl.h"

// The outermost installation still standing on each side of the city (the
// deathray's next target from that side). Everything but the acropolis is
// gone once city_left_edge > city_right_edge.
extern u8 city_left_edge;
extern u8 city_right_edge;

void collision_init(void);
void collision_handle_torpedoes(void);
void collision_handle_deathray(u8 ray_x, i8 velocity);
