#include "tiles.h"

#include "tileset.h"


void tiles_init(void)
{
    VDP_EnableDisplay(FALSE);
    VDP_LoadPattern_GM2(tileset_Patterns, TILES_COUNT, 0);
    VDP_LoadColor_GM2(tileset_Colors, TILES_COUNT, 0);
    VDP_FillScreen_GM2(' ');
    VDP_EnableDisplay(TRUE);
}


void tiles_set_border(u8 border)
{
    VDP_SetColor2(border, COLOR_BLACK);
}
