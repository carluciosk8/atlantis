#include "tiles.h"

#include "tileset.h"


void init_tiles()
{
    ubox_disable_screen();
    ubox_set_tiles(tileset);
    ubox_set_tiles_colors(tileset_colors);
    ubox_fill_screen(32);
    ubox_enable_screen();
}
