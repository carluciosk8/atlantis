#pragma once

#include "msxgl.h"

// Arkos Tracker 2 music + SFX helpers (Step 5).
//
// The song (song.akm) and the sound-effects bank (effects.akx) are
// position-dependent binaries exported for fixed RAM addresses (see
// docs/MIGRATION_PLAN.md). music_init() copies them to those addresses and
// starts the game music; the H_TIMI hook installed in atlantis.c advances the
// player with AKM_Update() once per vblank.


void music_init();
void music_play(u8 song);
void sfx_play(u8 no, u8 chan, u8 vol);
