#include "lane.h"

#include "city.h"      // g_FrameCounter (deathray animation)
#include "collision.h" // collision_handle_deathray
#include "gorgons.h"
#include "sprites.h"
#include "tiles.h"     // set_border
#include "torpedo.h"   // torpedos[] (deactivate a torpedo on hit)
#include "wave.h"


u8 g_is_attacking = 0;
lane_t g_lanes[4];


void lane_init(u8 n)
{
    g_lanes[n].sprite1 = &sprites_attributes[2*n];
    g_lanes[n].sprite2 = &sprites_attributes[2*n + 1];
    g_lanes[n].sprite1->Y = n*24;
    g_lanes[n].sprite2->Y = n*24;
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
    u8 state = lane->state;
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
        u8 gorgon_id = wave_gorgons[lane->wave_index];
        lane->gorgon_id = gorgon_id;
        lane->sprite1->X = (gorgon_id % 2) ? LANE_RIGHT_LIMIT : LANE_LEFT_LIMIT;
        lane->sprite2->X = lane->sprite1->X;
        lane->sprite1->Pattern = g_gorgon_list[gorgon_id].pattern1;
        lane->sprite2->Pattern = g_gorgon_list[gorgon_id].pattern2;
        lane->sprite1->Color = g_gorgon_list[gorgon_id].attr1;
        lane->sprite2->Color = g_gorgon_list[gorgon_id].attr2;
        lane->state = LANE_STATE_MOVING;
        if (!lane->next) g_is_attacking = 1;
    }
}


void lane_execute_state_moving(lane_t* lane)
{
    lane->sprite1->X += wave_velocities[lane->wave_index];
    lane->sprite2->X = lane->sprite1->X;

    if (g_is_attacking && !lane->next)
    {
        // TODO(step 5): mplayer_play_effect(2, 0, 0)  →  AKM_PlaySFX(2, 0, 0)
        sprites_update_deathray(lane->sprite1->X, g_FrameCounter % 4);
        collision_handle_deathray(lane->sprite1->X, wave_velocities[lane->wave_index]);
    }

    if (LANE_RIGHT_LIMIT < lane->sprite1->X)
    {
        lane->sprite1->Color = 0;
        lane->sprite2->Color = 0;
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


void lane_execute_state_exploding(lane_t* lane, u8 is_big)
{
    if (lane->explosion_sprite_id > 4)
    {
        if (is_big)
        {
            sprites_set_magnification(false);
            set_border(7);
        }
        lane->state = LANE_STATE_WAITING;
        lane->sprite1->Color = 0;
        lane->sprite2->Color = 0;
        g_wave_size--;
    }
    else
    {
        lane->sprite1->Pattern = sprites_explosion_anim1[lane->explosion_sprite_id];
        lane->sprite2->Pattern = sprites_explosion_anim2[lane->explosion_sprite_id];
        lane->explosion_counter++;
        lane->explosion_sprite_id = lane->explosion_counter >> 2;
        if (is_big) set_border(1 + lane->explosion_counter % 15);
    }
}


void lane_start_state_exploding(u8 torpedo, u8 lane, u8 is_big)
{
    g_lanes[lane].explosion_counter = 0;
    g_lanes[lane].explosion_sprite_id = 0;
    g_lanes[lane].sprite1->Color = 9;
    g_lanes[lane].sprite2->Color = 11;
    g_lanes[lane].state = LANE_STATE_EXPLODING;
    if (lane == 3) sprites_disable_deathray();

    torpedos[torpedo].is_active = 0;
    torpedos[torpedo].sprite->X = 0;
    torpedos[torpedo].sprite->Y = 192;

    if (is_big)
    {
        sprites_set_magnification(true);
        g_lanes[lane].state = 4*LANE_STATE_EXPLODING;
        // TODO(step 5): mplayer_play_effect_p(4, 1, 0)  →  AKM_PlaySFX(4, 1, 0)
    }
    else
    {
        // TODO(step 5): mplayer_play_effect(3, 1, 0)  →  AKM_PlaySFX(3, 1, 0)
    }
}
