#pragma once

#include "msxgl.h"

#include "queue.h"


#define LANE_COUNT      4
#define LANE_LAST       (LANE_COUNT - 1)    // the lane whose gorgons attack the city
#define LANE_HEIGHT     24                  // sprite Y of lane n is n * LANE_HEIGHT

#define LANE_STATE_WAITING          0
#define LANE_STATE_MOVING           1
#define LANE_STATE_EXPLODING        2
#define LANE_STATE_EXPLODING_BIG    8       // bandit hit: magnified sprites + border flash

#define LANE_LEFT_LIMIT     0
#define LANE_RIGHT_LIMIT    239

// One horizontal flight lane. Lanes are chained through `next`: a gorgon that
// leaves the screen is queued into the next lane, and from the last lane
// (next == 0) it attacks the city with the deathray.
typedef struct lane_s
{
    u8 state;
    u8 wave_index;                  // index into wave_gorgons[] / wave_velocities[]
    u8 gorgon_id;
    struct VDP_Sprite* sprite1;     // two overlaid sprites, because an MSX1
    struct VDP_Sprite* sprite2;     // sprite has a single colour
    queue_t queue;                  // wave indices of the gorgons waiting to enter
    u8 explosion_counter;
    u8 explosion_sprite_id;
    struct lane_s* next;
} lane_t;

extern lane_t g_lanes[LANE_COUNT];


void lane_init_all(void);
void lane_update_all(void);

// Blow up the gorgon in lane `lane_index`. is_big (bandit hit) also magnifies
// the sprites and flashes the border. The caller hides the torpedo.
void lane_explode(u8 lane_index, u8 is_big);
