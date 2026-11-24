#include "screens.h"

#include "city.h"
#include "text_data.h"
#include "tiles.h"
#include "wave.h"


// Gold plaque frame tiles (one tile thick, rounded outer corners).
#define FRAME_TILE_TL   0xA1
#define FRAME_TILE_T    0xA2
#define FRAME_TILE_TR   0xA3
#define FRAME_TILE_L    0xA4
#define FRAME_TILE_R    0xA5
#define FRAME_TILE_BL   0xA6
#define FRAME_TILE_B    0xA7
#define FRAME_TILE_BR   0xA8

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
        TILES_FILL(10, row, MAP_TILE_SKY, sizeof(wave_text) + 2);
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
    VDP_EnableDisplay(TRUE);
}


void screens_blink_title(void)
{
    // 10 cycles of 7 frames off / 7 frames on (140 vblanks, ~2.33 s at 60 Hz,
    // 2.8 s at 50 Hz).
    for (u8 blink = 0; blink < 10; blink++)
    {
        for (u8 frame = 0; frame < 7; frame++)
        {
            Halt();
            TILES_PUT_BUFFER_CENTERED(13, title_blank);
        }
        for (u8 frame = 0; frame < 7; frame++)
        {
            Halt();
            TILES_PUT_BUFFER_CENTERED(13, title_press_start);
        }
    }
}
