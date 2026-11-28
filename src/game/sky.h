#pragma once

#include "msxgl.h"

#include "sky_data.h"


// The sky (name-table rows 0..11) changes with the wave: day, evening, night.
// Each sky recolours the tiles with sky pixels (buildings, launchers, waves,
// plaque frame corners), draws its rows and decorations, and animates its
// clouds or birds in reserved tile slots, or (at night) sweeps the spotlights,
// where the black gorgons can be seen.

extern u8 sky_current;     // SKY_*

#define sky_get_border()   (sky_border[sky_current])

// Screen thirds with stars in game.
#define SKY_STAR_BANKS     2

// Draw `sky` with the display as it is (map_init calls it with the display off).
void sky_draw(u8 sky);

// Switch to `sky` between waves, with the display off; nothing if already shown.
void sky_set(u8 sky);

// Redraw the clouds, birds or spotlights over a cleared banner.
void sky_refresh(void);

// Move the clouds or birds, or sweep the night spotlights and twinkle the stars; call once per in-wave frame. Not called while a
// banner or plaque is shown, so the animation never draws over them, and the
// next step of each object rewrites its name-table cells.
void sky_update(void);

// Colour the bright star tiles with twinkle phase `phase` (sky_twinkle) in the
// first `banks` screen thirds, from the top. In game the stars are in the top and
// middle thirds (SKY_STAR_BANKS); the title screen has stars in all three.
void sky_set_star_colors(u8 phase, u8 banks);
