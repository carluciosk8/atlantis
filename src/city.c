#include "city.h"

#include "hud.h"
#include "lane.h"
#include "music.h"
#include "sprites.h"
#include "tiles.h"


#define MAP_TILE_SKY    0x23


const u8 map_background[] =
{
    0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x7B,0x7C,
    0x77,0x78,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x2C,0x2D,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x2C,0x2D,
    0x22,0x3A,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x3D,0x22,0x22,0x3A,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x3D,0x22,
    0x22,0x3B,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x7E,0x22,0x22,0x3C,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x3E,0x22,
    0x22,0x22,0x5C,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x7E,0x22,0x22,0x22,0x22,0x5E,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x7E,0x7D,0x22,0x22,
    0x22,0x22,0x5B,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x3E,0x22,0x22,0x22,0x22,0x5B,0x21,0x5D,0x21,0x21,0x21,0x21,0x5D,0x21,0x5F,0x22,0x22,0x22,0x22,
    0x22,0x22,0x3C,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x3F,0x22,0x22,0x22,0x22,0x22,0x7D,0x22,0x5C,0x21,0x21,0x21,0x3F,0x22,0x22,0x22,0x22,0x22,0x22,
    0x22,0x22,0x3B,0x21,0x21,0x21,0x21,0x21,0x3E,0x22,0x22,0x22,0x7D,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x5B,0x21,0x21,0x21,0x3E,0x22,0x22,0x22,0x22,0x22,0x22,
    0x22,0x22,0x22,0x5E,0x21,0x21,0x21,0x5F,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x3B,0x21,0x21,0x21,0x3F,0x22,0x22,0x22,0x22,0x22,0x22,
    0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x3C,0x21,0x21,0x21,0x3F,0x22,0x22,0x22,0x22,0x22,0x22,
    0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x5C,0x21,0x21,0x3E,0x22,0x22,0x22,0x22,0x22,0x22,
};

const u8 map_water_anim[4][12] =
{
    {0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80},
    {0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81},
    {0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82},
    {0x83,0x83,0x83,0x83,0x83,0x83,0x83,0x83,0x83,0x83,0x83,0x83},
};

const u8 map_acropolis_command[] = {0x79,0x7A};
const u8 map_generator_aqua[] =    {0x84};
const u8 map_generator_air[] =     {0x88};
const u8 map_aqua_plain[] =        {0x1D,0x1E,0x1F};
const u8 map_domed_palace[] =      {0x28,0x29,0x2A,0x2B};
const u8 map_bridged_bazaar[] =    {0x24,0x25,0x26,0x27};

// Installation tables, indexed by MAP_*_ID.
const u8* map_installations_tiles[] =    { map_acropolis_command,  map_aqua_plain,  map_domed_palace,  map_generator_aqua,  map_generator_air,  map_bridged_bazaar,  map_generator_aqua };
const u8  map_installations_size[] =     { 2,                      3,               4,                 1,                   1,                  4,                   1                  };
const u8  map_installations_x[] =        { 120,                    36,              72,                100,                 130,                178,                 220                };
const u8  map_installations_y[] =        { 84,                     153,             138,               114,                 100,                127,                 114                };
      u8  map_have_installation[] =      { 1,                      1,               1,                 1,                   1,                  1,                   1                  };

// The background tiles under each installation, written back when it is destroyed.
const u8* map_installations_background[] =
{
    TILES_MAKE_ADDRESS(15, 0, map_background),
    TILES_MAKE_ADDRESS(4,  8, map_background),
    TILES_MAKE_ADDRESS(8,  6, map_background),
    TILES_MAKE_ADDRESS(13, 3, map_background),
    TILES_MAKE_ADDRESS(17, 1, map_background),
    TILES_MAKE_ADDRESS(21, 5, map_background),
    TILES_MAKE_ADDRESS(28, 3, map_background)
};

// Position of each installation in the name table (added to g_ScreenLayoutLow
// at the use site).
const u16 map_installations_vram_offset[] =
{
    TILES_MAKE_ADDRESS(15, 12, 0),
    TILES_MAKE_ADDRESS(4,  20, 0),
    TILES_MAKE_ADDRESS(8,  18, 0),
    TILES_MAKE_ADDRESS(13, 15, 0),
    TILES_MAKE_ADDRESS(17, 13, 0),
    TILES_MAKE_ADDRESS(21, 17, 0),
    TILES_MAKE_ADDRESS(28, 15, 0)
};

u8 map_state = MAP_STATE_IDLE;
u8 map_explosion_counter = 0;

// Last water/generator animation frame written to VRAM (0xFF forces a redraw).
static u8 map_anim_frame = 0xFF;


void map_init(void)
{
    map_anim_frame = 0xFF;

    VDP_EnableDisplay(FALSE);
    VDP_SetColor2(COLOR_CYAN, COLOR_BLACK);   // border; the text colour is unused in Screen 2
    VDP_FillScreen_GM2(MAP_TILE_SKY);
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


void map_create_installation(u8 index)
{
    VDP_WriteVRAM_16K(map_installations_tiles[index], g_ScreenLayoutLow + map_installations_vram_offset[index], map_installations_size[index]);
    map_have_installation[index] = 1;
    map_anim_frame = 0xFF;   // a rebuilt generator gets its animated tile next frame
}


void map_destroy_installation(u8 index)
{
    VDP_WriteVRAM_16K(map_installations_background[index], g_ScreenLayoutLow + map_installations_vram_offset[index], map_installations_size[index]);
    map_have_installation[index] = 0;
    g_is_attacking = 0;
    sprites_disable_deathray();
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
