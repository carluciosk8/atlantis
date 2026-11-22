#include "collision.h"

#include "city.h"
#include "gorgons.h"
#include "lane.h"
#include "score.h"
#include "tiles.h"
#include "torpedo.h"
#include "wave.h"


u8 left_index = 1;
u8 right_index = 6;


void collision_init()
{
    left_index = 1;
    right_index = 6;
}


// lo <= d <= hi for an 8-bit difference d, as a single unsigned compare
// (exact with 8-bit wraparound: values below lo wrap above hi - lo).
#define IN_RANGE(d, lo, hi)  ((u8)((u8)(d) - (u8)(lo)) <= (u8)((hi) - (lo)))


// Test one (active) torpedo against the lanes 3, 2, 1, 0 (legacy order).
static void collision_handle_torpedo(u8 torpedo)
{
    torpedo_t* t = &torpedos[torpedo];
    u8 tx = t->sprite->X;
    u8 ty = t->sprite->Y;
    lane_t* lane = &g_lanes[3];
    for (u8 n = 3; n != 0xFF; n--, lane--)
    {
        if (lane->state != LANE_STATE_MOVING) continue;

        u8 dx = tx - lane->sprite1->X;
        u8 dy = ty - lane->sprite1->Y;
        u8 id = lane->gorgon_id;
        if (id == GORGON_ID_BANDIT_LEFT || id == GORGON_ID_BANDIT_RIGHT)
        {
            if (IN_RANGE(dx, 2, 12) && IN_RANGE(dy, 6, 14))
            {
                if (g_lanes[0].state == LANE_STATE_MOVING) lane_start_state_exploding(torpedo, 0, 1);
                if (g_lanes[1].state == LANE_STATE_MOVING) lane_start_state_exploding(torpedo, 1, 1);
                if (g_lanes[2].state == LANE_STATE_MOVING) lane_start_state_exploding(torpedo, 2, 1);
                if (g_lanes[3].state == LANE_STATE_MOVING) lane_start_state_exploding(torpedo, 3, 1);

                score_add((t->dx == 0) ? 10 : 20);
                TILES_PUT_BUFFER(9, 23, score, 6);
                return;   // the torpedo is now parked at Y=192: no further hit possible
            }
        }
        else if (IN_RANGE(dx, -2, 15) && IN_RANGE(dy, 4, 15))
        {
            lane_start_state_exploding(torpedo, n, 0);

            score_add((t->dx == 0) ? 1 : 2);
            TILES_PUT_BUFFER(9, 23, score, 6);
            return;       // the torpedo is now parked at Y=192: no further hit possible
        }
    }
}


// An inactive torpedo is parked at Y=192 and can never overlap a lane (lane
// Y <= 72), so skipping it gives the same result as testing it. The test is
// done here, with constant indices, so it costs a single absolute load.
void collision_handle_torpedos()
{
    if (torpedos[0].is_active) collision_handle_torpedo(0);
    if (torpedos[1].is_active) collision_handle_torpedo(1);
}


void collision_handle_deathray(u8 x, i8 v)
{
    if (map_have_installation[0])
    {
        if ((120 <= x && v > 0) || (x <= 120 && v < 0))
        {
            map_start_explosion(120, 92);
            map_destroy_instalation(0);
            if ( left_index > right_index && !map_have_installation[0] && score_credits == 0) g_wave_size = 0;
        }
    }
    else if (left_index <= right_index)
    {
        if (v > 0 && map_installations_x[left_index] < x)
        {
            map_start_explosion(map_installations_x[left_index], map_installations_y[left_index]);
            map_destroy_instalation(left_index);
            left_index++;
            if ( left_index > right_index && !map_have_installation[0] && score_credits == 0) g_wave_size = 0;
        }
        else if (v < 0 && x < map_installations_x[right_index])
        {
            map_start_explosion(map_installations_x[right_index], map_installations_y[right_index]);
            map_destroy_instalation(right_index);
            right_index--;
            if ( left_index > right_index && !map_have_installation[0] && score_credits == 0) g_wave_size = 0;
        }
    }
}
