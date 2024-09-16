#pragma once

#include "ubox.h"

#include "queue.h"


#define LANE_STATE_WAITING   0
#define LANE_STATE_MOVING    1
#define LANE_STATE_EXPLODING 2

#define LANE_LEFT_LIMIT     0
#define LANE_RIGHT_LIMIT    239

typedef struct lane_s
{
    uint8_t state;
    uint8_t wave_index;
    uint8_t gorgon_id;
    struct sprite_attr* sprite1;
    struct sprite_attr* sprite2;
    queue_t queue;
    uint8_t explosion_counter;
    uint8_t explosion_sprite_id;
    struct lane_s* next;
} lane_t;

extern lane_t g_lanes[4];
extern uint8_t g_is_attacking;

void lane_init_all();
void lane_update_all();

void lane_execute_state_waiting(lane_t* lane);
void lane_execute_state_moving(lane_t* lane);
void lane_execute_state_exploding(lane_t* lane, uint8_t is_big);

void lane_start_state_exploding(uint8_t torpedo, uint8_t lane, uint8_t is_big);
