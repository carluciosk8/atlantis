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

// Give the bright star tiles their own colours back (the title uses them after
// a night wave may have left them mid-twinkle).
void sky_reset_stars(void);
