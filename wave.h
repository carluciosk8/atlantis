#pragma once

#include "msxgl.h"

#define WAVE_MAX_SIZE    32

extern u8 wave_gorgons[WAVE_MAX_SIZE];
extern i8 wave_velocities[WAVE_MAX_SIZE];
extern i8 g_wave_size;

void wave_init();
void wave_draw_hud();
void wave_start();
void wave_finish();
