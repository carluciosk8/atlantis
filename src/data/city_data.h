#pragma once

#include "msxgl.h"


// City data: const tables only, no code. The installation tables are indexed by
// the MAP_*_ID values below.

// Plain sky tile (the name-table fill under the city and behind the wave banner).
#define MAP_TILE_SKY           0x23

// Sky gradient: tile for each of the top 4 name-table rows (light blue fading
// into the plain sky tile below), and the cloud tiles (the big cloud is 3x2
// tiles in two rows, the small one is 2x1).
#define MAP_SKY_GRADIENT_ROWS  4
extern const u8 map_sky_gradient[MAP_SKY_GRADIENT_ROWS];
extern const u8 map_cloud_big_top[3];
extern const u8 map_cloud_big_bottom[3];
extern const u8 map_cloud_small[2];

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


// Background name-table rows 12..22 (11 rows of 32 tiles).
extern const u8 map_background[11*32];

// Wave crest animation, 16 frames of 12 tiles (written to row 14 at x=2 and
// x=18). A frame alternates two tiles, so the wave flows across the whole row.
#define MAP_WATER_FRAMES       16
extern const u8 map_water_anim[MAP_WATER_FRAMES][12];

// Installation tiles (the generators' first tile is their first animation frame).
extern const u8 map_acropolis_command[2];
extern const u8 map_generator_aqua[1];
extern const u8 map_generator_air[1];
extern const u8 map_aqua_plain[3];
extern const u8 map_domed_palace[4];
extern const u8 map_bridged_bazaar[4];

extern const u8* const map_installations_tiles[MAP_INSTALLATION_COUNT];
extern const u8  map_installations_size[MAP_INSTALLATION_COUNT];
extern const u8  map_installations_x[MAP_INSTALLATION_COUNT];
extern const u8  map_installations_y[MAP_INSTALLATION_COUNT];

// The background tiles under each installation, written back when it is destroyed.
extern const u8* const map_installations_background[MAP_INSTALLATION_COUNT];

// Position of each installation in the name table (added to g_ScreenLayoutLow
// at the use site).
extern const u16 map_installations_vram_offset[MAP_INSTALLATION_COUNT];
