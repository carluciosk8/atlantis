#pragma once

#include "msxgl.h"

// Arkos Tracker 2 music and sound effects. The song (song.akm) and the SFX bank
// (effects.akx) are position-dependent binaries: music_init() copies them to
// their fixed RAM addresses (see the memory map in CLAUDE.md), and the H_TIMI
// hook installed in atlantis.c advances the player once per vblank.

// Sub-songs of song.akm
#define MUSIC_GAME          0
#define MUSIC_TITLE         1
#define MUSIC_GAME_OVER     2
#define MUSIC_HI_SCORE      3

// Sound effects of effects.akx (numbered from 1)
#define SFX_TORPEDO             1
#define SFX_DEATHRAY            2
#define SFX_EXPLOSION           3
#define SFX_BIG_EXPLOSION       4
#define SFX_INSTALLATION_LOST   5


void music_init(void);
void music_play(u8 song);
void sfx_play(u8 sfx, u8 channel, u8 inverted_volume);
