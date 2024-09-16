#pragma once

#include <ubox.h>

#define MAP_STATE_IDLE         0
#define MAP_STATE_EXPLODING    1

#define MAP_ACROPOLIS_COMMAND_ID    0
#define MAP_AUQA_PLAINS_ID          1
#define MAP_DOMED_PALACE_ID         2
#define MAP_GENERATOR_AQUA1_ID      3
#define MAP_GENERATOR_AIR_ID        4
#define MAP_BRIDGED_BAZAAR_ID       5
#define MAP_GENERATOR_AQUA2_ID      6


extern const uint8_t map_background[];
extern const uint8_t map_water_anim[4][12];

extern const uint8_t map_acropolis_command[];
extern const uint8_t map_generator_aqua[];
extern const uint8_t map_generator_air[];
extern const uint8_t map_aqua_plain[];
extern const uint8_t map_domed_palace[];
extern const uint8_t map_bridged_bazaar[];

extern const uint8_t* map_installations_tiles[];
extern const uint8_t* map_installations_address[];
extern const uint8_t* map_installations_vaddress[];
extern const uint8_t  map_installations_size[];
extern const uint8_t  map_installations_x[];
extern const uint8_t  map_installations_y[];

extern uint8_t map_have_installation[];
extern uint8_t map_state;
extern uint8_t map_explosion_counter;


void map_init();
void map_create_instalation(uint8_t index);
void map_destroy_instalation(uint8_t index);
void map_start_explosion(uint8_t x, uint8_t y);
void map_update();
