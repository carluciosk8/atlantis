#include "city.h"

#include "frame.h"
#include "hud.h"
#include "music.h"
#include "sprite_data.h"
#include "sprites.h"
#include "tiles.h"


#define MAP_STATE_IDLE         0
#define MAP_STATE_EXPLODING    1


// 1 while the installation stands.
u8 map_have_installation[MAP_INSTALLATION_COUNT] = { 1, 1, 1, 1, 1, 1, 1 };

// Set by map_init(), before the first wave.
u8 city_left_edge;
u8 city_right_edge;

static u8 map_state = MAP_STATE_IDLE;
static u8 map_explosion_counter = 0;

// Last water/generator animation frame written to VRAM (0xFF forces a redraw).
static u8 map_anim_frame = 0xFF;


static void map_create_installation(u8 index)
{
    VDP_WriteVRAM_16K(map_installations_tiles[index], g_ScreenLayoutLow + map_installations_vram_offset[index], map_installations_size[index]);
    map_have_installation[index] = 1;
    map_anim_frame = 0xFF;   // a rebuilt generator gets its animated tile next frame
}


static void map_put_cloud(u8 x, u8 y, bool big)
{
    if (big)
    {
        TILES_PUT_BUFFER(x, y,     map_cloud_big_top,    sizeof(map_cloud_big_top));
        TILES_PUT_BUFFER(x, y + 1, map_cloud_big_bottom, sizeof(map_cloud_big_bottom));
    }
    else
        TILES_PUT_BUFFER(x, y, map_cloud_small, sizeof(map_cloud_small));
}


// Sky: the plain sky tile, a light-blue gradient on the top rows and a few
// clouds. The clouds keep clear of the wave banner and its frame (columns 10..21,
// rows 4..8), which is restored with plain sky tiles.
static void map_draw_sky(void)
{
    VDP_FillScreen_GM2(MAP_TILE_SKY);
    for (u8 row = 0; row < MAP_SKY_GRADIENT_ROWS; row++)
        TILES_FILL(0, row, map_sky_gradient[row], 32);

    map_put_cloud(3,  5, TRUE);
    map_put_cloud(24, 4, TRUE);
    map_put_cloud(6,  8, TRUE);
    map_put_cloud(7,  4, FALSE);
    map_put_cloud(22, 8, FALSE);
    map_put_cloud(15, 10, FALSE);
}


void map_init(void)
{
    map_anim_frame = 0xFF;
    city_left_edge = MAP_LEFTMOST_ID;
    city_right_edge = MAP_RIGHTMOST_ID;

    VDP_EnableDisplay(FALSE);
    VDP_SetColor2(COLOR_CYAN, COLOR_BLACK);   // border; the text colour is unused in Screen 2
    map_draw_sky();
    TILES_PUT_BUFFER(0, 12, map_background, sizeof(map_background));
    TILES_PUT_BUFFER(0, HUD_ROW, hud, sizeof(hud));

    map_create_installation(MAP_ACROPOLIS_COMMAND_ID);
    map_create_installation(MAP_AQUA_PLAIN_ID);
    map_create_installation(MAP_DOMED_PALACE_ID);
    map_create_installation(MAP_GENERATOR_AQUA1_ID);
    map_create_installation(MAP_GENERATOR_AIR_ID);
    map_create_installation(MAP_BRIDGED_BAZAAR_ID);
    map_create_installation(MAP_GENERATOR_AQUA2_ID);

    VDP_EnableDisplay(TRUE);
}


void map_destroy_installation(u8 index)
{
    VDP_WriteVRAM_16K(map_installations_background[index], g_ScreenLayoutLow + map_installations_vram_offset[index], map_installations_size[index]);
    map_have_installation[index] = 0;
    sfx_play(SFX_INSTALLATION_LOST, 0, 0);
}


void map_start_explosion(u8 x, u8 y)
{
    sprites_attributes[SPRITE_PLANE_EXPLOSION].X = x;
    sprites_attributes[SPRITE_PLANE_EXPLOSION + 1].X = x;
    sprites_attributes[SPRITE_PLANE_EXPLOSION].Y = y;
    sprites_attributes[SPRITE_PLANE_EXPLOSION + 1].Y = y;
    sprites_attributes[SPRITE_PLANE_EXPLOSION].Color = COLOR_LIGHT_RED;
    sprites_attributes[SPRITE_PLANE_EXPLOSION + 1].Color = COLOR_LIGHT_YELLOW;
    sprites_attributes[SPRITE_PLANE_EXPLOSION].Pattern = sprites_explosion_anim1[0];
    sprites_attributes[SPRITE_PLANE_EXPLOSION + 1].Pattern = sprites_explosion_anim2[0];
    map_explosion_counter = 0;
    map_state = MAP_STATE_EXPLODING;
}


void map_update(void)
{
    // Water and generator animation, driven by the 8-bit frame counter.
    // water_frame advances every 4 vblanks (0..3), air_frame every 8 (0..1).
    // The tiles are rewritten only when water_frame differs from the last
    // value written, so a frame overrun that skips a value still redraws.
    u8 water_frame = (u8)(g_FrameCounter >> 2) & 3;

    if (water_frame != map_anim_frame)
    {
        u8 air_frame = water_frame >> 1;
        map_anim_frame = water_frame;

        if (map_have_installation[MAP_GENERATOR_AIR_ID])   VDP_Poke_GM2(17, 13, map_generator_air[0] + air_frame);
        if (map_have_installation[MAP_GENERATOR_AQUA1_ID]) VDP_Poke_GM2(13, 15, map_generator_aqua[0] + water_frame);
        if (map_have_installation[MAP_GENERATOR_AQUA2_ID]) VDP_Poke_GM2(28, 15, map_generator_aqua[0] + water_frame);

        VDP_WriteVRAM_16K(map_water_anim[water_frame], g_ScreenLayoutLow + TILES_MAKE_ADDRESS(2, 14, 0), 12);
        VDP_WriteVRAM_16K(map_water_anim[water_frame], g_ScreenLayoutLow + TILES_MAKE_ADDRESS(18, 14, 0), 12);
    }

    // Building explosion: 5 frames of 4 vblanks each.
    if (map_state == MAP_STATE_EXPLODING)
    {
        u8 explosion_frame = map_explosion_counter >> 2;
        if (explosion_frame > 4)
        {
            map_state = MAP_STATE_IDLE;
            sprites_attributes[SPRITE_PLANE_EXPLOSION].Color = COLOR_TRANSPARENT;
            sprites_attributes[SPRITE_PLANE_EXPLOSION + 1].Color = COLOR_TRANSPARENT;
        }
        else
        {
            sprites_attributes[SPRITE_PLANE_EXPLOSION].Pattern = sprites_explosion_anim1[explosion_frame];
            sprites_attributes[SPRITE_PLANE_EXPLOSION + 1].Pattern = sprites_explosion_anim2[explosion_frame];
            map_explosion_counter++;
        }
    }
}


u8 city_rebuild(u8 credits)
{
    if ((map_have_installation[MAP_ACROPOLIS_COMMAND_ID] == 0) && (credits > 0))
    {
        map_create_installation(MAP_ACROPOLIS_COMMAND_ID);
        credits--;
    }

    while (credits > 0 && (city_left_edge > MAP_LEFTMOST_ID || city_right_edge < MAP_RIGHTMOST_ID))
    {
        if (credits > 0 && city_left_edge > MAP_LEFTMOST_ID)
        {
            city_left_edge--;
            map_create_installation(city_left_edge);
            credits--;
        }
        if (credits > 0 && city_right_edge < MAP_RIGHTMOST_ID)
        {
            city_right_edge++;
            map_create_installation(city_right_edge);
            credits--;
        }
    }

    return credits;
}
