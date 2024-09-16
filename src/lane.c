#include "lane.h"

#include "collision.h"
#include "gorgons.h"
#include "sprites.h"
#include "torpedo.h"
#include "wave.h"

#include <mplayer.h>


uint8_t g_is_attacking = 0;
lane_t g_lanes[4];


void lane_init(uint8_t n)
{
    g_lanes[n].sprite1 = &sprites_attributes[2*n];
    g_lanes[n].sprite2 = &sprites_attributes[2*n + 1];
    g_lanes[n].sprite1->y = n*24;
    g_lanes[n].sprite2->y = n*24;
    g_lanes[n].state = LANE_STATE_WAITING;
    g_lanes[n].wave_index = 0;
    g_lanes[n].next = (n<3) ? &(g_lanes[(n+1)%4]) : 0;
    queue_init(g_lanes[n].queue);
}


void lane_init_all()
{
    lane_init(0);
    lane_init(1);
    lane_init(2);
    lane_init(3);
}


void lane_update(lane_t* lane)
{
    uint8_t state = lane->state;
    if      (state == LANE_STATE_WAITING)   lane_execute_state_waiting(lane);
    else if (state == LANE_STATE_MOVING)    lane_execute_state_moving(lane);
    else if (state == LANE_STATE_EXPLODING) lane_execute_state_exploding(lane, 0);
    else if (state == 4*LANE_STATE_EXPLODING) lane_execute_state_exploding(lane, 1);
}


void lane_update_all()
{
    lane_update(&g_lanes[0]);
    lane_update(&g_lanes[1]);
    lane_update(&g_lanes[2]);
    lane_update(&g_lanes[3]);
}


void lane_execute_state_waiting(lane_t* lane)
{
    if (lane->queue.size > 0)
    {
        queue_pop(&(lane->queue), &(lane->wave_index));
        uint8_t gorgon_id = wave_gorgons[lane->wave_index];
        lane->gorgon_id = gorgon_id;
        lane->sprite1->x = (gorgon_id % 2) ? LANE_RIGHT_LIMIT : LANE_LEFT_LIMIT;
        lane->sprite2->x = lane->sprite1->x;
        lane->sprite1->pattern = g_gorgon_list[gorgon_id].pattern1;
        lane->sprite2->pattern = g_gorgon_list[gorgon_id].pattern2;
        lane->sprite1->attr = g_gorgon_list[gorgon_id].attr1;
        lane->sprite2->attr = g_gorgon_list[gorgon_id].attr2;
        lane->state = LANE_STATE_MOVING;
        if (!lane->next) g_is_attacking = 1;
    }
}


void lane_execute_state_moving(lane_t* lane)
{
    lane->sprite1->x += wave_velocities[lane->wave_index];
    lane->sprite2->x = lane->sprite1->x;

    if (g_is_attacking && !lane->next)
    {
        mplayer_play_effect(2, 0, 0);
        sprites_update_deathray(lane->sprite1->x, ubox_tick % 4);
        collision_handle_deathray(lane->sprite1->x, wave_velocities[lane->wave_index]);
    }

    if (LANE_RIGHT_LIMIT < lane->sprite1->x)
    {
        lane->sprite1->attr = 0;
        lane->sprite2->attr = 0;
        lane->state = LANE_STATE_WAITING;
        if (lane->next)
        {
            queue_push(&(lane->next->queue), lane->wave_index);
        }
        else
        {
            g_wave_size--;
            sprites_disable_deathray();
        }
    }
}


void lane_execute_state_exploding(lane_t* lane, uint8_t is_big)
{
    if (lane->explosion_sprite_id > 4)
    {
        if (is_big)
        {
            ubox_wvdp(1, 0xe2);
            ubox_set_colors(1,1,7);
        }
        lane->state = LANE_STATE_WAITING;
        lane->sprite1->attr = 0;
        lane->sprite2->attr = 0;
        g_wave_size--;
    }
    else
    {
        lane->sprite1->pattern = sprites_explosion_anim1[lane->explosion_sprite_id];
        lane->sprite2->pattern = sprites_explosion_anim2[lane->explosion_sprite_id];
        lane->explosion_counter++;
        lane->explosion_sprite_id = lane->explosion_counter >> 2;
        if (is_big) ubox_set_colors(1,1,1 + lane->explosion_counter % 15);
    }
}


void lane_start_state_exploding(uint8_t torpedo, uint8_t lane, uint8_t is_big)
{
    g_lanes[lane].explosion_counter = 0;
    g_lanes[lane].explosion_sprite_id = 0;
    g_lanes[lane].sprite1->attr = 9;
    g_lanes[lane].sprite2->attr = 11;
    g_lanes[lane].state = LANE_STATE_EXPLODING;
    if (lane == 3) sprites_disable_deathray();

    torpedos[torpedo].is_active = 0;
    torpedos[torpedo].sprite->x = 0;
    torpedos[torpedo].sprite->y = 192;

    if (is_big)
    {
        ubox_wvdp(1, 0xe3);
        g_lanes[lane].state = 4*LANE_STATE_EXPLODING;
        mplayer_play_effect_p(4, 1, 0);
    }
    else mplayer_play_effect(3, 1, 0);
}
