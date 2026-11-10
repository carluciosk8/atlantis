// ____________________________
//  Atlantis — MSXgl (Step 2: display layer)
//
//  Sets Screen 2, loads the tileset, and draws the title screen / map / HUD so
//  they can be checked in the emulator. Frame pacing, sprites, music, input and
//  RNG are ported in later steps.
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"

#include "city.h"
#include "hud.h"
#include "tiles.h"

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
/// Program entry point
void main(void)
{
	// Screen 2 (Graphic 2), 256×192
	VDP_SetMode(VDP_MODE_SCREEN2);
	VDP_ClearVRAM();

	init_tiles();

	map_init();          // map + HUD (cyan border)
	draw_title_screen(); // title screen (black border), drawn last

	for (;;)
	{
		Halt();
	}
}
