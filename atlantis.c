// ____________________________
//  Atlantis — MSXgl
//
//  Boot (Screen 2, tiles, sprite patterns, music, vblank hook), then hands
//  over to the game flow (flow.c).
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"

#include "flow.h"
#include "frame.h"
#include "music.h"
#include "sprites.h"
#include "tiles.h"

//-----------------------------------------------------------------------------
/// Program entry point
void main(void)
{
    // Screen 2 (Graphic 2), 256×192
    VDP_SetMode(VDP_MODE_SCREEN2);
    VDP_ClearVRAM();
    BIOS_SetKeyClick(FALSE);

    tiles_init();
    sprites_init();      // after VDP_ClearVRAM, which wipes the pattern table

    // The hook is installed only after the player is initialised.
    VDP_EnableVBlank(TRUE);
    music_init();
    frame_install_hook();

    flow_run();
}
