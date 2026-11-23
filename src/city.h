#pragma once

#include "msxgl.h"


#define MAP_STATE_IDLE         0
#define MAP_STATE_EXPLODING    1

// Installation indices for the map_installations_* tables. The acropolis
// command post stands in the centre; 1..6 run from left to right, and the
// deathray destroys them from both edges inward.
#define MAP_ACROPOLIS_COMMAND_ID    0
#define MAP_AQUA_PLAIN_ID           1
#define MAP_DOMED_PALACE_ID         2
#define MAP_GENERATOR_AQUA1_ID      3
#define MAP_GENERATOR_AIR_ID        4
#define MAP_BRIDGED_BAZAAR_ID       5
#define MAP_GENERATOR_AQUA2_ID      6
#define MAP_INSTALLATION_COUNT      7

#define MAP_LEFTMOST_ID             MAP_AQUA_PLAIN_ID
#define MAP_RIGHTMOST_ID            MAP_GENERATOR_AQUA2_ID

#define MAP_ACROPOLIS_X             120
#define MAP_ACROPOLIS_EXPLOSION_Y   92


extern const u8 map_background[];
extern const u8 map_water_anim[4][12];

extern const u8 map_acropolis_command[];
extern const u8 map_generator_aqua[];
extern const u8 map_generator_air[];
extern const u8 map_aqua_plain[];
extern const u8 map_domed_palace[];
extern const u8 map_bridged_bazaar[];

extern const u8* map_installations_tiles[];
extern const u8* map_installations_background[];
extern const u16 map_installations_vram_offset[];
extern const u8  map_installations_size[];
extern const u8  map_installations_x[];
extern const u8  map_installations_y[];

extern u8 map_have_installation[];
extern u8 map_state;
extern u8 map_explosion_counter;


// Frame counter, incremented once per vblank by the H_TIMI hook (atlantis.c).
// 8-bit, wraps every 256 frames. Drives the water, generator and deathray
// animations and seeds the RNG at the start of each wave.
extern volatile u8 g_FrameCounter;


void map_init(void);
void map_create_installation(u8 index);
void map_destroy_installation(u8 index);
void map_start_explosion(u8 x, u8 y);
void map_update(void);
