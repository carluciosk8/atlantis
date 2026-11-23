#include "collision.h"

#include "city.h"
#include "gorgons.h"
#include "lane.h"
#include "score.h"
#include "tiles.h"
#include "torpedo.h"
#include "wave.h"


u8 city_left_edge = MAP_LEFTMOST_ID;
u8 city_right_edge = MAP_RIGHTMOST_ID;


void collision_init(void)
{
    city_left_edge = MAP_LEFTMOST_ID;
    city_right_edge = MAP_RIGHTMOST_ID;
}


// lo <= d <= hi for an 8-bit difference d, as a single unsigned compare
// (exact with 8-bit wraparound: values below lo wrap above hi - lo).
#define IN_RANGE(d, lo, hi)  ((u8)((u8)(d) - (u8)(lo)) <= (u8)((hi) - (lo)))

// End the wave at once when nothing is left to defend or to rebuild.
#define END_WAVE_IF_CITY_LOST() \
    if (city_left_edge > city_right_edge && !map_have_installation[MAP_ACROPOLIS_COMMAND_ID] && score_credits == 0) g_wave_size = 0


// Test one active torpedo against the lanes, from the bottom one up (3 to 0).
// The first hit wins.
static void collision_handle_torpedo(u8 torpedo_index)
{
    torpedo_t* torpedo = &torpedoes[torpedo_index];
    u8 tx = torpedo->sprite->X;
    u8 ty = torpedo->sprite->Y;
    lane_t* lane = &g_lanes[LANE_LAST];

    for (u8 lane_index = LANE_LAST; lane_index != 0xFF; lane_index--, lane--)
    {
        if (lane->state != LANE_STATE_MOVING) continue;

        u8 dx = tx - lane->sprite1->X;
        u8 dy = ty - lane->sprite1->Y;
        u8 id = lane->gorgon_id;

        if (GORGON_IS_BANDIT(id))
        {
            // A bandit hit blows up every gorgon on screen.
            if (IN_RANGE(dx, 2, 12) && IN_RANGE(dy, 6, 14))
            {
                if (g_lanes[0].state == LANE_STATE_MOVING) lane_start_state_exploding(torpedo_index, 0, 1);
                if (g_lanes[1].state == LANE_STATE_MOVING) lane_start_state_exploding(torpedo_index, 1, 1);
                if (g_lanes[2].state == LANE_STATE_MOVING) lane_start_state_exploding(torpedo_index, 2, 1);
                if (g_lanes[3].state == LANE_STATE_MOVING) lane_start_state_exploding(torpedo_index, 3, 1);

                score_add((torpedo->dx == 0) ? 10 : 20);
                SCORE_DRAW();
                return;   // the torpedo is now hidden: no further hit possible
            }
        }
        else if (IN_RANGE(dx, -2, 15) && IN_RANGE(dy, 4, 15))
        {
            lane_start_state_exploding(torpedo_index, lane_index, 0);

            score_add((torpedo->dx == 0) ? 1 : 2);
            SCORE_DRAW();
            return;       // the torpedo is now hidden: no further hit possible
        }
    }
}


// An inactive torpedo is parked at Y=192 and can never overlap a lane (lane
// Y <= 72), so skipping it gives the same result as testing it. The test is
// done here, with constant indices, so it costs a single absolute load.
void collision_handle_torpedoes(void)
{
    if (torpedoes[0].is_active) collision_handle_torpedo(0);
    if (torpedoes[1].is_active) collision_handle_torpedo(1);
}


// The deathray first destroys the acropolis, once the ray crosses it in its
// direction of travel. After that it destroys the installation at the city
// edge the gorgon is flying from.
void collision_handle_deathray(u8 ray_x, i8 velocity)
{
    if (map_have_installation[MAP_ACROPOLIS_COMMAND_ID])
    {
        if ((MAP_ACROPOLIS_X <= ray_x && velocity > 0) || (ray_x <= MAP_ACROPOLIS_X && velocity < 0))
        {
            map_start_explosion(MAP_ACROPOLIS_X, MAP_ACROPOLIS_EXPLOSION_Y);
            map_destroy_installation(MAP_ACROPOLIS_COMMAND_ID);
            END_WAVE_IF_CITY_LOST();
        }
    }
    else if (city_left_edge <= city_right_edge)
    {
        if (velocity > 0 && map_installations_x[city_left_edge] < ray_x)
        {
            map_start_explosion(map_installations_x[city_left_edge], map_installations_y[city_left_edge]);
            map_destroy_installation(city_left_edge);
            city_left_edge++;
            END_WAVE_IF_CITY_LOST();
        }
        else if (velocity < 0 && ray_x < map_installations_x[city_right_edge])
        {
            map_start_explosion(map_installations_x[city_right_edge], map_installations_y[city_right_edge]);
            map_destroy_installation(city_right_edge);
            city_right_edge--;
            END_WAVE_IF_CITY_LOST();
        }
    }
}
