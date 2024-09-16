#include "collision.h"

#include "city.h"
#include "gorgons.h"
#include "lane.h"
#include "score.h"
#include "tiles.h"
#include "torpedo.h"
#include "wave.h"


uint8_t left_index = 1;
uint8_t right_index = 6;


void collision_init()
{
    left_index = 1;
    right_index = 6;
}


void collision_handle_torpedo(uint8_t torpedo, uint8_t lane)
{
    int8_t dx = torpedos[torpedo].sprite->x - g_lanes[lane].sprite1->x;
    int8_t dy = torpedos[torpedo].sprite->y - g_lanes[lane].sprite1->y;
    if (g_lanes[lane].state == LANE_STATE_MOVING)
    {
        if (g_lanes[lane].gorgon_id == GORGON_ID_BANDIT_LEFT || g_lanes[lane].gorgon_id == GORGON_ID_BANDIT_RIGHT)
        {
            if (2 <= dx && dx <= 12 && 6 <= dy && dy <= 14)
            {
                if (g_lanes[0].state == LANE_STATE_MOVING) lane_start_state_exploding(torpedo, 0, 1);
                if (g_lanes[1].state == LANE_STATE_MOVING) lane_start_state_exploding(torpedo, 1, 1);
                if (g_lanes[2].state == LANE_STATE_MOVING) lane_start_state_exploding(torpedo, 2, 1);
                if (g_lanes[3].state == LANE_STATE_MOVING) lane_start_state_exploding(torpedo, 3, 1);

                if (torpedos[torpedo].dx == 0)
                    score_increase_thousand(1);
                else
                    score_increase_thousand(2);
                TILES_PUT_BUFFER(9, 23, score, 6);
            }
        }
        else
        {
            if (-2 <= dx && dx <= 15 && 4 <= dy && dy <= 15)
            {
                lane_start_state_exploding(torpedo, lane, 0);

                if (torpedos[torpedo].dx == 0)
                    score_increase_hundred(1);
                else
                    score_increase_hundred(2);
                TILES_PUT_BUFFER(9, 23, score, 6);
            }
        }
    }
}


void collision_handle_torpedos()
{
    collision_handle_torpedo(0, 3);
    collision_handle_torpedo(0, 2);
    collision_handle_torpedo(0, 1);
    collision_handle_torpedo(0, 0);

    collision_handle_torpedo(1, 3);
    collision_handle_torpedo(1, 2);
    collision_handle_torpedo(1, 1);
    collision_handle_torpedo(1, 0);
}


void collision_handle_deathray(uint8_t x, int8_t v)
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
