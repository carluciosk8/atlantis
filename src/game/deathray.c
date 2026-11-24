#include "deathray.h"

#include "city.h"
#include "frame.h"
#include "music.h"
#include "score.h"
#include "sprites.h"
#include "wave.h"


// 1 from the moment a gorgon enters the last lane until the ray destroys an
// installation.
static u8 deathray_armed = 0;


// End the wave at once when nothing is left to defend or to rebuild.
#define END_WAVE_IF_CITY_LOST() \
    if (!city_is_standing() && score_credits == 0) g_wave_size = 0


void deathray_arm(void)
{
    deathray_armed = 1;
}


void deathray_hide(void)
{
    sprites_attributes[SPRITE_PLANE_DEATHRAY + 0].Color = COLOR_TRANSPARENT;
    sprites_attributes[SPRITE_PLANE_DEATHRAY + 1].Color = COLOR_TRANSPARENT;
    sprites_attributes[SPRITE_PLANE_DEATHRAY + 2].Color = COLOR_TRANSPARENT;
    sprites_attributes[SPRITE_PLANE_DEATHRAY + 3].Color = COLOR_TRANSPARENT;
    sprites_attributes[SPRITE_PLANE_DEATHRAY + 4].Color = COLOR_TRANSPARENT;
}


// Move the 5 deathray sprites to x and show animation frame 0..3 (each frame
// has its own pattern and colour, from COLOR_MEDIUM_RED up).
static void deathray_show(u8 x, u8 frame)
{
    sprites_attributes[SPRITE_PLANE_DEATHRAY + 0].X = x;
    sprites_attributes[SPRITE_PLANE_DEATHRAY + 1].X = x;
    sprites_attributes[SPRITE_PLANE_DEATHRAY + 2].X = x;
    sprites_attributes[SPRITE_PLANE_DEATHRAY + 3].X = x;
    sprites_attributes[SPRITE_PLANE_DEATHRAY + 4].X = x;

    sprites_attributes[SPRITE_PLANE_DEATHRAY + 0].Pattern = SPRITE_PATTERN_DEATHRAY + frame*4;
    sprites_attributes[SPRITE_PLANE_DEATHRAY + 1].Pattern = SPRITE_PATTERN_DEATHRAY + frame*4;
    sprites_attributes[SPRITE_PLANE_DEATHRAY + 2].Pattern = SPRITE_PATTERN_DEATHRAY + frame*4;
    sprites_attributes[SPRITE_PLANE_DEATHRAY + 3].Pattern = SPRITE_PATTERN_DEATHRAY + frame*4;
    sprites_attributes[SPRITE_PLANE_DEATHRAY + 4].Pattern = SPRITE_PATTERN_DEATHRAY + frame*4;

    sprites_attributes[SPRITE_PLANE_DEATHRAY + 0].Color = COLOR_MEDIUM_RED + frame;
    sprites_attributes[SPRITE_PLANE_DEATHRAY + 1].Color = COLOR_MEDIUM_RED + frame;
    sprites_attributes[SPRITE_PLANE_DEATHRAY + 2].Color = COLOR_MEDIUM_RED + frame;
    sprites_attributes[SPRITE_PLANE_DEATHRAY + 3].Color = COLOR_MEDIUM_RED + frame;
    sprites_attributes[SPRITE_PLANE_DEATHRAY + 4].Color = COLOR_MEDIUM_RED + frame;
}


// Destroy installation `index` with the ray, then disarm and hide the ray.
static void deathray_destroy(u8 index)
{
    map_destroy_installation(index);
    deathray_armed = 0;
    deathray_hide();
}


// The deathray first destroys the acropolis, once the ray crosses it in its
// direction of travel. After that it destroys the installation at the city
// edge the gorgon is flying from.
static void deathray_hit_city(u8 ray_x, i8 velocity)
{
    if (map_have_installation[MAP_ACROPOLIS_COMMAND_ID])
    {
        if ((MAP_ACROPOLIS_X <= ray_x && velocity > 0) || (ray_x <= MAP_ACROPOLIS_X && velocity < 0))
        {
            map_start_explosion(MAP_ACROPOLIS_X, MAP_ACROPOLIS_EXPLOSION_Y);
            deathray_destroy(MAP_ACROPOLIS_COMMAND_ID);
            END_WAVE_IF_CITY_LOST();
        }
    }
    else if (city_left_edge <= city_right_edge)
    {
        if (velocity > 0 && map_installations_x[city_left_edge] < ray_x)
        {
            map_start_explosion(map_installations_x[city_left_edge], map_installations_y[city_left_edge]);
            deathray_destroy(city_left_edge);
            city_left_edge++;
            END_WAVE_IF_CITY_LOST();
        }
        else if (velocity < 0 && ray_x < map_installations_x[city_right_edge])
        {
            map_start_explosion(map_installations_x[city_right_edge], map_installations_y[city_right_edge]);
            deathray_destroy(city_right_edge);
            city_right_edge--;
            END_WAVE_IF_CITY_LOST();
        }
    }
}


void deathray_update(u8 x, i8 velocity)
{
    if (!deathray_armed) return;

    sfx_play(SFX_DEATHRAY, 0, 0);
    deathray_show(x, g_FrameCounter & 3);
    deathray_hit_city(x, velocity);
}
