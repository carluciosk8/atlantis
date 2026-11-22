#include "tiles.h"

#include "tileset.h"


void init_tiles()
{
    VDP_EnableDisplay(FALSE);
    VDP_LoadPattern_GM2(tileset_Patterns, TILES_COUNT, 0);
    VDP_LoadColor_GM2(tileset_Colors, TILES_COUNT, 0);
    VDP_FillScreen_GM2(32);
    VDP_EnableDisplay(TRUE);
}


void set_border(u8 border)
{
    VDP_SetColor2(border, 1);
}
