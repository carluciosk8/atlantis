#pragma once

#include "msxgl.h"

extern u8 left_index;
extern u8 right_index;

void collision_init();
void collision_handle_torpedos();
void collision_handle_deathray(u8 x, i8 v);
