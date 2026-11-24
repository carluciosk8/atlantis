#pragma once

#include "msxgl.h"


// Frame counter, incremented once per vblank by the H_TIMI hook installed by
// frame_install_hook(). 8-bit, wraps every 256 frames. Drives the water,
// generator and deathray animations and seeds the RNG at the start of each wave.
extern volatile u8 g_FrameCounter;


// Reset the frame counter and install the H_TIMI hook that increments it and
// advances the music player. Call it only after music_init(): the hook runs the
// player on the next vblank.
void frame_install_hook(void);
