#include "screens.h"

#include "city.h"
#include "frame.h"
#include "sky.h"
#include "text_data.h"
#include "tiles.h"
#include "wave.h"


// Gold plaque frame tiles (one tile thick, rounded outer corners).
#define FRAME_TILE_TL   0x9B
#define FRAME_TILE_T    0x9C
#define FRAME_TILE_TR   0x9D
#define FRAME_TILE_L    0x9E
#define FRAME_TILE_R    0x9F
#define FRAME_TILE_BL   0xA0
#define FRAME_TILE_B    0xA1
#define FRAME_TILE_BR   0xA2

// Draw the frame on the border cells of the w x h tile rectangle at (x, y);
// the interior is left for the caller.
static void screens_draw_frame(u8 x, u8 y, u8 w, u8 h)
{
    u8 right = x + w - 1;
    u8 bottom = y + h - 1;

    TILES_FILL(x,         y, FRAME_TILE_TL, 1);
    TILES_FILL(x + 1,     y, FRAME_TILE_T,  w - 2);
    TILES_FILL(right,     y, FRAME_TILE_TR, 1);
    for (u8 row = y + 1; row < bottom; row++)
    {
        TILES_FILL(x,     row, FRAME_TILE_L, 1);
        TILES_FILL(right, row, FRAME_TILE_R, 1);
    }
    TILES_FILL(x,         bottom, FRAME_TILE_BL, 1);
    TILES_FILL(x + 1,     bottom, FRAME_TILE_B,  w - 2);
    TILES_FILL(right,     bottom, FRAME_TILE_BR, 1);
}


void screens_draw_hi_score(void)
{
    screens_draw_frame(5, 4, 21, 7);
    TILES_FILL(6, 5, ' ', sizeof(text_hi_score_1));
    TILES_PUT_BUFFER(6, 6, text_hi_score_1, sizeof(text_hi_score_1));
    TILES_PUT_BUFFER(6, 7, text_hi_score_2, sizeof(text_hi_score_2));
    TILES_PUT_BUFFER(6, 8, text_hi_score_3, sizeof(text_hi_score_3));
    TILES_FILL(6, 9, ' ', sizeof(text_hi_score_1));
}


void screens_draw_game_over(void)
{
    screens_draw_frame(10, 4, 13, 5);
    TILES_FILL(11, 5, ' ', sizeof(text_game_over));
    TILES_PUT_BUFFER(11, 6, text_game_over, sizeof(text_game_over));
    TILES_FILL(11, 7, ' ', sizeof(text_game_over));
}


void screens_draw_wave_banner(void)
{
    screens_draw_frame(10, 4, 12, 5);
    TILES_FILL(11, 5, ' ', sizeof(wave_text));
    TILES_PUT_BUFFER(11, 6, wave_text, sizeof(wave_text));
    TILES_FILL(11, 7, ' ', sizeof(wave_text));
}


void screens_clear_wave_banner(void)
{
    // The frame and its interior: 12 x 5 tiles at (10, 4).
    for (u8 row = 4; row < 9; row++)
        TILES_FILL(10, row, sky_rows[sky_current][row], sizeof(wave_text) + 2);
}


// Last title animation frame written to VRAM (0xFF forces a redraw).
static u8 title_anim_frame;


void screens_animate_title(void)
{
    // Same timing as in game: one frame every 4 vblanks.
    u8 wave_frame = (u8)(g_FrameCounter >> 2) & (MAP_WATER_FRAMES - 1);
    if (wave_frame == title_anim_frame) return;
    title_anim_frame = wave_frame;

    // The 32-column row takes 12 + 12 + 8 tiles from three different frames.
    TILES_PUT_BUFFER(0,  TITLE_WAVE_ROW, map_water_anim[wave_frame], 12);
    TILES_PUT_BUFFER(12, TITLE_WAVE_ROW, map_water_anim[wave_frame ^ (MAP_WATER_FRAMES / 2)], 12);
    TILES_PUT_BUFFER(24, TITLE_WAVE_ROW, map_water_anim[(wave_frame + 4) & (MAP_WATER_FRAMES - 1)], 8);

    u8 generator = map_generator_aqua[0] + (wave_frame & 3);
    VDP_Poke_GM2(TITLE_GENERATOR1_X, TITLE_GENERATOR_ROW, generator);
    VDP_Poke_GM2(TITLE_GENERATOR2_X, TITLE_GENERATOR_ROW, generator);
}


void screens_draw_title(void)
{
    VDP_EnableDisplay(FALSE);
    VDP_SetColor2(COLOR_BLACK, COLOR_BLACK);   // border; the text colour is unused in Screen 2
    VDP_FillScreen_GM2(' ');

    TILES_PUT_BUFFER(10, 4, title_logo_row1, 2);
    TILES_PUT_BUFFER(25, 4, title_logo_row1, 2);
    TILES_PUT_BUFFER( 5, 5, title_logo_row2, 23);
    TILES_PUT_BUFFER( 4, 6, title_logo_row3, 24);
    TILES_PUT_BUFFER( 4, 7, title_logo_row4, 24);
    TILES_PUT_BUFFER_CENTERED(13, title_press_start);
    TILES_PUT_BUFFER_CENTERED(16, title_copyright);
    const u8* star = title_stars[0];
    for (u8 i = 0; i < TITLE_STAR_COUNT; i++, star += 3)
        VDP_Poke_GM2(star[0], star[1], star[2]);
    TILES_PUT_BUFFER(0, TITLE_SEABED_ROW, title_seabed, sizeof(title_seabed));
    title_anim_frame = 0xFF;
    screens_animate_title();
    VDP_EnableDisplay(TRUE);
}


void screens_blink_title(u8 cycles)
{
    // `cycles` (> 0) cycles of 7 frames off / 7 frames on.
    do
    {
        for (u8 frame = 0; frame < 7; frame++)
        {
            Halt();
            screens_animate_title();
            TILES_FILL(16 - sizeof(title_press_start)/2, 13, ' ', sizeof(title_press_start));
        }
        for (u8 frame = 0; frame < 7; frame++)
        {
            Halt();
            screens_animate_title();
            TILES_PUT_BUFFER_CENTERED(13, title_press_start);
        }
    } while (--cycles);
}
