#pragma once

#include "ubox.h"

#define WAVE_MAX_SIZE    32

extern uint8_t wave_gorgons[WAVE_MAX_SIZE];
extern int8_t  wave_velocities[WAVE_MAX_SIZE];
extern int8_t g_wave_size;

void wave_init();
void wave_draw_hud();
void wave_start();
void wave_finish();
