#include "tiles.h"

#include "tileset.h"


void tiles_replace_color(u8 first, u8 count, u8 bank, u8 from, u8 to)
{
    u8 buffer[8];
    const u8* color = &tileset_Colors[first * 8];
    u16 dest = g_ScreenColorLow + bank * TILES_BANK_SIZE + first * 8;

    for (; count > 0; count--, dest += 8)
    {
        for (u8 i = 0; i < 8; i++, color++)
        {
            u8 value = *color;
            if ((value >> 4) == from)   value = (value & 0x0F) | (to << 4);
            if ((value & 0x0F) == from) value = (value & 0xF0) | to;
            buffer[i] = value;
        }
        VDP_WriteVRAM_16K(buffer, dest, 8);
    }
}


void tiles_init(void)
{
    VDP_EnableDisplay(FALSE);
    VDP_LoadPattern_GM2(tileset_Patterns, TILES_COUNT, 0);
    VDP_LoadColor_GM2(tileset_Colors, TILES_COUNT, 0);
    tiles_replace_color(TILES_WAVE_FIRST, TILES_WAVE_COUNT, TILES_BANK_BOTTOM, COLOR_CYAN, COLOR_BLACK);   // the title's night waves
    VDP_FillScreen_GM2(' ');
    VDP_EnableDisplay(TRUE);
}


void tiles_set_border(u8 border)
{
    VDP_SetColor2(border, COLOR_BLACK);
}
