#include "tiles.h"

#include "tileset.h"


// Offset of the bottom screen third inside the Screen 2 pattern/colour tables.
#define TILES_BOTTOM_BANK   0x1000


// Rewrite the bottom-third colours of the wave tiles with a black sky.
static void tiles_load_night_waves(void)
{
    const u8* color = &tileset_Colors[TILES_WAVE_FIRST * 8];
    u16 dest = g_ScreenColorLow + TILES_BOTTOM_BANK + TILES_WAVE_FIRST * 8;

    for (u16 i = 0; i < TILES_WAVE_COUNT * 8; i++)
    {
        u8 value = color[i];
        if ((value & 0xF0) == (COLOR_CYAN << 4)) value = (value & 0x0F) | (COLOR_BLACK << 4);
        if ((value & 0x0F) == COLOR_CYAN)        value = (value & 0xF0) | COLOR_BLACK;
        VDP_Poke_16K(value, dest + i);
    }
}


void tiles_init(void)
{
    VDP_EnableDisplay(FALSE);
    VDP_LoadPattern_GM2(tileset_Patterns, TILES_COUNT, 0);
    VDP_LoadColor_GM2(tileset_Colors, TILES_COUNT, 0);
    tiles_load_night_waves();
    VDP_FillScreen_GM2(' ');
    VDP_EnableDisplay(TRUE);
}


void tiles_set_border(u8 border)
{
    VDP_SetColor2(border, COLOR_BLACK);
}
