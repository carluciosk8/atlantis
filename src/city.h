#pragma once

#include "msxgl.h"

#define MAP_STATE_IDLE         0
#define MAP_STATE_EXPLODING    1

#define MAP_ACROPOLIS_COMMAND_ID    0
#define MAP_AUQA_PLAINS_ID          1
#define MAP_DOMED_PALACE_ID         2
#define MAP_GENERATOR_AQUA1_ID      3
#define MAP_GENERATOR_AIR_ID        4
#define MAP_BRIDGED_BAZAAR_ID       5
#define MAP_GENERATOR_AQUA2_ID      6


extern const u8 map_background[];
extern const u8 map_water_anim[4][12];

extern const u8 map_acropolis_command[];
extern const u8 map_generator_aqua[];
extern const u8 map_generator_air[];
extern const u8 map_aqua_plain[];
extern const u8 map_domed_palace[];
extern const u8 map_bridged_bazaar[];

extern const u8* map_installations_tiles[];
extern const u8* map_installations_address[];
extern const u16 map_installations_vaddress[];
extern const u8  map_installations_size[];
extern const u8  map_installations_x[];
extern const u8  map_installations_y[];

extern u8 map_have_installation[];
extern u8 map_state;
extern u8 map_explosion_counter;


// Frame counter (platform/loop layer), incremented once per vblank by the
// H_TIMI hook installed in atlantis.c. 8-bit, wraps every 256 frames (legacy
// ubox_tick). Used by map_update() for the water/generator animation; later
// steps use it for the RNG seed (Step 7) and the deathray animation (Step 4).
extern volatile u8 g_FrameCounter;


void map_init();
void map_create_instalation(u8 index);
void map_destroy_instalation(u8 index);
void map_start_explosion(u8 x, u8 y);
void map_update();
