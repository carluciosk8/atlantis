#pragma once

#include "ubox.h"

extern uint8_t left_index;
extern uint8_t right_index;

void collision_init();
void collision_handle_torpedos();
void collision_handle_deathray(uint8_t x, int8_t v);
