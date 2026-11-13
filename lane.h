#pragma once

#include "msxgl.h"

#include "queue.h"


#define LANE_STATE_WAITING   0
#define LANE_STATE_MOVING    1
#define LANE_STATE_EXPLODING 2

#define LANE_LEFT_LIMIT     0
#define LANE_RIGHT_LIMIT    239

typedef struct lane_s
{
    u8 state;
    u8 wave_index;
    u8 gorgon_id;
    struct VDP_Sprite* sprite1;
    struct VDP_Sprite* sprite2;
    queue_t queue;
    u8 explosion_counter;
    u8 explosion_sprite_id;
    struct lane_s* next;
} lane_t;

extern lane_t g_lanes[4];
extern u8 g_is_attacking;

void lane_init_all();
void lane_update_all();

void lane_execute_state_waiting(lane_t* lane);
void lane_execute_state_moving(lane_t* lane);
void lane_execute_state_exploding(lane_t* lane, u8 is_big);

void lane_start_state_exploding(u8 torpedo, u8 lane, u8 is_big);
