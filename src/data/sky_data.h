#pragma once

#include "msxgl.h"


// Sky data: const tables only, no code. The sky changes with the wave
// (day, evening, night, then day again) and covers name-table rows 0..11.

#define SKY_DAY                0
#define SKY_EVENING            1
#define SKY_NIGHT              2
#define SKY_COUNT              3

#define SKY_ROWS               12

// The plain tile of each sky row (gradient or band). The banner is cleared with
// these, so no decoration may sit in the banner area (columns 10..21, rows 4..8).
extern const u8 sky_rows[SKY_COUNT][SKY_ROWS];

// Border colour of each sky.
extern const u8 sky_border[SKY_COUNT];

// The colour that replaces cyan in the tiles with sky pixels (sky_recolour),
// in the top and in the middle screen third.
extern const u8 sky_cyan[SKY_COUNT][2];

// Tiles with cyan sky pixels, recoloured per sky: { first tile, count }.
// The ranges also cover a few tiles without cyan, which come out unchanged.
#define SKY_RECOLOUR_COUNT     6
extern const u8 sky_recolour[SKY_RECOLOUR_COUNT][2];

// Decorations, tile strips: { x, y, first tile, count }. The evening has the
// setting sun and cloud streaks; the day has none, and the night stars are
// part of the sky_beam_sky frames. Only the evening has strips: sky_draw_strips
// walks sky_evening_strips for sky_strip_count[sky] entries.
#define SKY_EVENING_STRIPS     5
extern const u8 sky_evening_strips[SKY_EVENING_STRIPS][4];
extern const u8 sky_strip_count[SKY_COUNT];


// Night spotlights: 3 beam layouts from above the side launchers (tiles
// 0x60..0x76, light yellow on black), swept 1 -> 2 -> 3 -> 2 every
// SKY_BEAM_FRAMES. Each layout is a whole night sky (rows 0..11, stars
// included), written in one block so no cell goes black between layouts, plus
// row 12 columns 0..1: the left beam reaches row 12 above the left launcher;
// the rest of row 12 (the acropolis) is never written. The right beam ends on
// row 11.
#define SKY_BEAM_FRAMES        15     // 0.25 s at 60 Hz, 0.3 s at 50 Hz
#define SKY_BEAM_LAYOUTS       3
#define SKY_BEAM_STEPS         4
extern const u8 sky_beam_sky[SKY_BEAM_LAYOUTS][SKY_ROWS*32];
extern const u8 sky_beam_row12[SKY_BEAM_LAYOUTS][2];
extern const u8 sky_beam_order[SKY_BEAM_STEPS];   // layout of each sweep step


// Moving objects (clouds and birds), drawn into reserved tile slots whose
// patterns are written at run time, shifted 1 px at a time. Objects that share
// a row move at the same speed, so they never overlap.
#define SKY_KIND_CLOUD_BIG     0    // 24x16 source in 4x2 tiles, drifts right
#define SKY_KIND_CLOUD_SMALL   1    // 16x8 source in 3x1 tiles, drifts right
#define SKY_KIND_BIRD          2    // 8x8 source in 2x1 tiles, flies left

typedef struct
{
    u8 kind;
    u8 x;       // start X in pixels
    u8 row;     // top name-table row
    u8 speed;   // frames per pixel
    u8 slot;    // first reserved tile (width * height tiles, row-major)
} sky_object_t;

#define SKY_DAY_OBJECTS        5
#define SKY_EVENING_OBJECTS    3
#define SKY_MAX_OBJECTS        5
extern const sky_object_t sky_day_objects[SKY_DAY_OBJECTS];
extern const sky_object_t sky_evening_objects[SKY_EVENING_OBJECTS];
extern const sky_object_t* const sky_objects_of[SKY_COUNT];
extern const u8 sky_object_count_of[SKY_COUNT];

// Source art, one row of bytes per pixel line (width - 1 bytes).
extern const u8 sky_cloud_big[16*3];
extern const u8 sky_cloud_small[8*2];
// Bird flap frames: 9 lines, a blank line first, so that starting one line
// later moves the bird up 1 px.
#define SKY_BIRD_LINES         9
extern const u8 sky_bird[2*SKY_BIRD_LINES];

// Colour of each pixel line (white top, grey underside; black birds on light red).
extern const u8 sky_cloud_big_colors[16];
extern const u8 sky_cloud_small_colors[8];
extern const u8 sky_bird_colors[8];

// Geometry and art of each SKY_KIND_*.
typedef struct
{
    u8 width;           // in tiles (the art is width - 1 bytes wide)
    u8 height;          // in tile rows
    u8 tiles;           // width * height: the cost of one step (sky_update)
    const u8* art;
    const u8* colors;   // one colour byte per pixel line
} sky_kind_t;

extern const sky_kind_t sky_kinds[3];
