#include "sky.h"

#include "city_data.h"   // map_background
#include "frame.h"
#include "tiles.h"


u8 sky_current = SKY_DAY;

// Moving objects of the current sky.
static const sky_object_t* sky_objects;
static u8 sky_object_count;
static u8 sky_x[SKY_MAX_OBJECTS];       // X in pixels
static u8 sky_timer[SKY_MAX_OBJECTS];   // frames until the next 1 px step

// Night spotlights: step in sky_beam_order, frames until the next one.
static u8 sky_beam;
static u8 sky_beam_timer;

// Night star twinkle: phase in sky_twinkle, frames until the next one.
static u8 sky_twinkle_phase;
static u8 sky_twinkle_timer;

// Tiles rewritten per frame at most by sky_update().
#define SKY_STEP_BUDGET     8

// One tile row of shifted patterns, tile-major (up to 4 tiles).
static u8 sky_buffer[4 * 8];


static void sky_put(u8 x, u8 y, u8 tile)
{
    VDP_Poke_16K(tile, g_ScreenLayoutLow + y * 32 + x);
}


// Draw the decorations of the current sky (the evening sun and streaks).
static void sky_draw_strips(void)
{
    const u8* strip = sky_evening_strips[0];
    for (u8 i = sky_strip_count[sky_current]; i > 0; i--, strip += 4)
        for (u8 k = 0; k < strip[3]; k++)
            sky_put(strip[0] + k, strip[1], strip[2] + k);
}


// Draw the night sky with the spotlights of the current sweep step.
static void sky_draw_beam(void)
{
    u8 layout = sky_beam_order[sky_beam];
    VDP_WriteVRAM_16K(sky_beam_sky[layout], g_ScreenLayoutLow, SKY_ROWS * 32);
    VDP_WriteVRAM_16K(sky_beam_row12[layout], g_ScreenLayoutLow + SKY_ROWS * 32, 2);
}


// Load the beam tiles into the pool slots of the top and middle banks.
static void sky_load_beam_tiles(void)
{
    for (u8 bank = TILES_BANK_TOP; bank <= TILES_BANK_MIDDLE; bank++)
    {
        u16 pattern = g_ScreenPatternLow + bank * TILES_BANK_SIZE;
        u16 color = g_ScreenColorLow + bank * TILES_BANK_SIZE;
        VDP_WriteVRAM_16K(sky_beam_tiles, pattern + SKY_BEAM_SLOTS_LOW * 8, SKY_BEAM_SLOTS_LOW_COUNT * 8);
        VDP_WriteVRAM_16K(sky_beam_tiles + SKY_BEAM_SLOTS_LOW_COUNT * 8, pattern + SKY_BEAM_SLOTS_HIGH * 8, (SKY_BEAM_TILES - SKY_BEAM_SLOTS_LOW_COUNT) * 8);
        VDP_FillVRAM_16K(SKY_BEAM_COLOR, color + SKY_BEAM_SLOTS_LOW * 8, SKY_BEAM_SLOTS_LOW_COUNT * 8);
        VDP_FillVRAM_16K(SKY_BEAM_COLOR, color + SKY_BEAM_SLOTS_HIGH * 8, (SKY_BEAM_TILES - SKY_BEAM_SLOTS_LOW_COUNT - SKY_BEAM_STAR_TILES) * 8);
        VDP_WriteVRAM_16K(sky_beam_star_colors, color + (SKY_BEAM_SLOTS_HIGH + SKY_BEAM_TILES - SKY_BEAM_SLOTS_LOW_COUNT - SKY_BEAM_STAR_TILES) * 8, SKY_BEAM_STAR_TILES * 8);
    }
}


void sky_set_star_colors(u8 phase, u8 banks)
{
    u8 buffer[SKY_STAR_COUNT * 8];
    const u8* colors = sky_twinkle[phase];
    u8* out = buffer;
    for (u8 k = SKY_STAR_COUNT; k > 0; k--, colors++)
        for (u8 i = 8; i > 0; i--)
            *out++ = *colors;
    u16 dest = g_ScreenColorLow + SKY_STAR_FIRST * 8;
    for (; banks > 0; banks--, dest += TILES_BANK_SIZE)
        VDP_WriteVRAM_16K(buffer, dest, sizeof(buffer));
}


// Shift one tile row of art (8 lines of width-1 bytes) right by `shift` pixels
// into sky_buffer, as `width` tiles.
static void sky_shift(const u8* src, u8 width, u8 shift)
{
    for (u8 line = 0; line < 8; line++)
    {
        u8 prev = 0;
        u8* out = &sky_buffer[line];
        for (u8 k = 1; k <= width; k++, out += 8)
        {
            u8 cur = (k < width) ? *src++ : 0;
            *out = (u8)((((u16)prev << 8) | cur) >> shift);
            prev = cur;
        }
    }
}


// Write the patterns and name-table cells of object `index` at its position.
static void sky_draw_object(u8 index)
{
    const sky_object_t* obj = &sky_objects[index];
    const sky_kind_t* kind = &sky_kinds[obj->kind];
    u8 width = kind->width;
    u8 row = obj->row;
    u8 tile = obj->slot;
    u8 x = sky_x[index];
    const u8* src = kind->art;

    if (obj->kind == SKY_KIND_BIRD)
    {
        // Flap every 8 frames, bob 1 px every 32; each bird has its own phase.
        u8 phase = (u8)(g_FrameCounter >> 3) + index;
        if (phase & 1) src += SKY_BIRD_LINES;
        if (!(phase & 4)) src++;
    }

    for (u8 ty = kind->height; ty > 0; ty--, row++)
    {
        sky_shift(src, width, x & 7);
        src += 8 * (width - 1);
        VDP_WriteVRAM_16K(sky_buffer, g_ScreenPatternLow + (row >> 3) * TILES_BANK_SIZE + tile * 8, width * 8);
        for (u8 k = 0; k < width; k++, tile++)
            sky_put(((x >> 3) + k) & 31, row, tile);
    }
}


// Advance object `index` by 1 px; put back the sky in the cell column it leaves.
static void sky_step_object(u8 index)
{
    const sky_object_t* obj = &sky_objects[index];
    const sky_kind_t* kind = &sky_kinds[obj->kind];
    u8 x = sky_x[index];
    u8 left = 0xFF;   // column to restore, if any

    if (obj->kind == SKY_KIND_BIRD)
    {
        // Flying left: leaving a cell when the shift wraps from 0 to 7.
        if ((x & 7) == 0) left = ((x >> 3) + kind->width - 1) & 31;
        x--;
    }
    else
    {
        // Drifting right: leaving a cell when the shift wraps from 7 to 0.
        x++;
        if ((x & 7) == 0) left = ((x >> 3) - 1) & 31;
    }
    sky_x[index] = x;

    if (left != 0xFF)
    {
        u8 row = obj->row;
        for (u8 ty = kind->height; ty > 0; ty--, row++)
            sky_put(left, row, sky_rows[sky_current][row]);
    }

    sky_draw_object(index);
}


void sky_draw(u8 sky)
{
    sky_current = sky;

    // Sky pixels inside the city, waves and plaque frame.
    u8 top = sky_cyan[sky][0];
    u8 middle = sky_cyan[sky][1];
    const u8* range = sky_recolour[0];
    for (u8 i = SKY_RECOLOUR_COUNT; i > 0; i--, range += 2)
    {
        tiles_replace_color(range[0], range[1], TILES_BANK_TOP,    COLOR_CYAN, top);
        tiles_replace_color(range[0], range[1], TILES_BANK_MIDDLE, COLOR_CYAN, middle);
    }
    tiles_set_border(sky_border[sky]);

    const u8* rows = sky_rows[sky];
    u16 dest = g_ScreenLayoutLow;
    for (u8 row = SKY_ROWS; row > 0; row--, dest += 32)
        VDP_FillVRAM_16K(*rows++, dest, 32);
    // Row 12, columns 0..1: the background under the night's left beam.
    VDP_WriteVRAM_16K(map_background, dest, 2);

    sky_draw_strips();

    // Night spotlights, starting at the first layout, and the star twinkle
    // (phase 0 is also the stars' own colour, for the other skies).
    sky_beam = 0;
    sky_beam_timer = SKY_BEAM_FRAMES;
    sky_twinkle_phase = 0;
    sky_twinkle_timer = SKY_TWINKLE_FRAMES;
    sky_set_star_colors(0, SKY_STAR_BANKS);
    if (sky == SKY_NIGHT)
    {
        sky_load_beam_tiles();
        sky_draw_beam();
    }

    // Moving objects: line colours of their reserved tiles, then the first frame.
    sky_objects = sky_objects_of[sky];
    sky_object_count = sky_object_count_of[sky];
    for (u8 i = 0; i < sky_object_count; i++)
    {
        const sky_object_t* obj = &sky_objects[i];
        const sky_kind_t* kind = &sky_kinds[obj->kind];
        const u8* colors = kind->colors;
        u8 row = obj->row;
        u16 dest = obj->slot * 8;
        for (u8 ty = kind->height; ty > 0; ty--, row++, colors += 8)
        {
            u16 bank = g_ScreenColorLow + (row >> 3) * TILES_BANK_SIZE;
            for (u8 k = kind->width; k > 0; k--, dest += 8)
                VDP_WriteVRAM_16K(colors, bank + dest, 8);
        }

        sky_x[i] = obj->x;
        sky_timer[i] = obj->speed;
        sky_draw_object(i);
    }
}


void sky_set(u8 sky)
{
    if (sky == sky_current) return;
    VDP_EnableDisplay(FALSE);
    sky_draw(sky);
    VDP_EnableDisplay(TRUE);
}


void sky_refresh(void)
{
    for (u8 i = 0; i < sky_object_count; i++)
        sky_draw_object(i);
    if (sky_current == SKY_NIGHT) sky_draw_beam();
}


void sky_update(void)
{
    // Night: move the spotlights to the next layout every SKY_BEAM_FRAMES.
    if (sky_current == SKY_NIGHT && --sky_beam_timer == 0)
    {
        sky_beam_timer = SKY_BEAM_FRAMES;
        if (++sky_beam == SKY_BEAM_STEPS) sky_beam = 0;
        sky_draw_beam();
    }

    // Night: twinkle the bright stars.
    if (sky_current == SKY_NIGHT && --sky_twinkle_timer == 0)
    {
        sky_twinkle_timer = SKY_TWINKLE_FRAMES;
        sky_twinkle_phase = (sky_twinkle_phase + 1) & (SKY_TWINKLE_PHASES - 1);
        sky_set_star_colors(sky_twinkle_phase, SKY_STAR_BANKS);
    }


    // At most SKY_STEP_BUDGET tiles are rewritten per frame, to bound the frame
    // time (one big cloud, or all the birds); an object that is due but over
    // the budget waits for the next frame.
    u8 budget = SKY_STEP_BUDGET;
    for (u8 i = 0; i < sky_object_count; i++)
    {
        if (sky_timer[i] > 1) sky_timer[i]--;
        else
        {
            u8 cost = sky_kinds[sky_objects[i].kind].tiles;
            if (cost <= budget)
            {
                budget -= cost;
                sky_timer[i] = sky_objects[i].speed;
                sky_step_object(i);
            }
        }
    }
}
