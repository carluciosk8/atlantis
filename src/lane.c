#include "lane.h"

#include "city.h"      // g_FrameCounter (deathray animation)
#include "collision.h" // collision_handle_deathray
#include "gorgons.h"
#include "music.h"
#include "sprites.h"
#include "tiles.h"     // tiles_set_border
#include "torpedo.h"   // torpedoes[] (deactivate a torpedo on hit)
#include "wave.h"


u8 g_is_attacking = 0;
lane_t g_lanes[LANE_COUNT];

// Big-explosion border colour, 1 + counter % 15, for explosion_counter 0..20
// (the explosion ends once counter >> 2 exceeds 4, so 20 is the maximum).
static const u8 lane_big_border[21] =
{
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    1, 2, 3, 4, 5, 6,
};


void lane_init(u8 lane_index)
{
    g_lanes[lane_index].sprite1 = &sprites_attributes[2*lane_index];
    g_lanes[lane_index].sprite2 = &sprites_attributes[2*lane_index + 1];
    g_lanes[lane_index].sprite1->Y = lane_index*LANE_HEIGHT;
    g_lanes[lane_index].sprite2->Y = lane_index*LANE_HEIGHT;
    g_lanes[lane_index].state = LANE_STATE_WAITING;
    g_lanes[lane_index].wave_index = 0;
    g_lanes[lane_index].next = (lane_index < LANE_LAST) ? &(g_lanes[(lane_index + 1) % LANE_COUNT]) : 0;
    queue_init(g_lanes[lane_index].queue);
}


void lane_init_all(void)
{
    lane_init(0);
    lane_init(1);
    lane_init(2);
    lane_init(3);
}


void lane_update(lane_t* lane)
{
    u8 state = lane->state;
    if      (state == LANE_STATE_WAITING)       lane_execute_state_waiting(lane);
    else if (state == LANE_STATE_MOVING)        lane_execute_state_moving(lane);
    else if (state == LANE_STATE_EXPLODING)     lane_execute_state_exploding(lane, 0);
    else if (state == LANE_STATE_EXPLODING_BIG) lane_execute_state_exploding(lane, 1);
}


void lane_update_all(void)
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

        lane->sprite1->X = GORGON_MOVES_LEFT(gorgon_id) ? LANE_RIGHT_LIMIT : LANE_LEFT_LIMIT;
        lane->sprite2->X = lane->sprite1->X;
        lane->sprite1->Pattern = g_gorgon_list[gorgon_id].pattern1;
        lane->sprite2->Pattern = g_gorgon_list[gorgon_id].pattern2;
        lane->sprite1->Color = g_gorgon_list[gorgon_id].color1;
        lane->sprite2->Color = g_gorgon_list[gorgon_id].color2;

        lane->state = LANE_STATE_MOVING;
        if (!lane->next) g_is_attacking = 1;
    }
}


void lane_execute_state_moving(lane_t* lane)
{
    struct VDP_Sprite* sprite1 = lane->sprite1;
    i8 velocity = wave_velocities[lane->wave_index];
    u8 x = sprite1->X + velocity;
    sprite1->X = x;
    lane->sprite2->X = x;

    // A gorgon in the last lane fires the deathray while it crosses the city.
    if (g_is_attacking && !lane->next)
    {
        sfx_play(SFX_DEATHRAY, 0, 0);
        sprites_update_deathray(x, g_FrameCounter & 3);
        collision_handle_deathray(x, velocity);   // touches neither lane sprite
    }

    // Off either edge (flying left wraps the 8-bit X past the limit): hand the
    // gorgon to the next lane, or end its run if this was the last lane.
    if (x > LANE_RIGHT_LIMIT)
    {
        lane->sprite1->Color = COLOR_TRANSPARENT;
        lane->sprite2->Color = COLOR_TRANSPARENT;
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
            tiles_set_border(COLOR_CYAN);
        }
        lane->state = LANE_STATE_WAITING;
        lane->sprite1->Color = COLOR_TRANSPARENT;
        lane->sprite2->Color = COLOR_TRANSPARENT;
        g_wave_size--;
    }
    else
    {
        lane->sprite1->Pattern = sprites_explosion_anim1[lane->explosion_sprite_id];
        lane->sprite2->Pattern = sprites_explosion_anim2[lane->explosion_sprite_id];
        lane->explosion_counter++;
        lane->explosion_sprite_id = lane->explosion_counter >> 2;
        if (is_big) tiles_set_border(lane_big_border[lane->explosion_counter]);
    }
}


void lane_start_state_exploding(u8 torpedo_index, u8 lane_index, u8 is_big)
{
    g_lanes[lane_index].explosion_counter = 0;
    g_lanes[lane_index].explosion_sprite_id = 0;
    g_lanes[lane_index].sprite1->Color = COLOR_LIGHT_RED;
    g_lanes[lane_index].sprite2->Color = COLOR_LIGHT_YELLOW;
    g_lanes[lane_index].state = LANE_STATE_EXPLODING;
    if (lane_index == LANE_LAST) sprites_disable_deathray();

    torpedoes[torpedo_index].is_active = 0;
    torpedoes[torpedo_index].sprite->X = 0;
    torpedoes[torpedo_index].sprite->Y = SPRITE_Y_HIDDEN;

    if (is_big)
    {
        sprites_set_magnification(true);
        g_lanes[lane_index].state = LANE_STATE_EXPLODING_BIG;
        sfx_play(SFX_BIG_EXPLOSION, 1, 0);
    }
    else
    {
        sfx_play(SFX_EXPLOSION, 1, 0);
    }
}
