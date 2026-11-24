#include "screens.h"

#include "city.h"
#include "text_data.h"
#include "tiles.h"
#include "wave.h"


void screens_draw_hi_score(void)
{
    TILES_FILL(6, 4, ' ', sizeof(text_hi_score_1));
    TILES_PUT_BUFFER(6, 5, text_hi_score_1, sizeof(text_hi_score_1));
    TILES_PUT_BUFFER(6, 6, text_hi_score_2, sizeof(text_hi_score_2));
    TILES_PUT_BUFFER(6, 7, text_hi_score_3, sizeof(text_hi_score_3));
    TILES_FILL(6, 8, ' ', sizeof(text_hi_score_1));
}


void screens_draw_game_over(void)
{
    TILES_FILL(11, 5, ' ', sizeof(text_game_over));
    TILES_PUT_BUFFER(11, 6, text_game_over, sizeof(text_game_over));
    TILES_FILL(11, 7, ' ', sizeof(text_game_over));
}


void screens_draw_wave_banner(void)
{
    TILES_FILL(11, 5, ' ', sizeof(wave_text));
    TILES_PUT_BUFFER(11, 6, wave_text, sizeof(wave_text));
    TILES_FILL(11, 7, ' ', sizeof(wave_text));
}


void screens_clear_wave_banner(void)
{
    TILES_FILL(11, 5, MAP_TILE_SKY, sizeof(wave_text));
    TILES_FILL(11, 6, MAP_TILE_SKY, sizeof(wave_text));
    TILES_FILL(11, 7, MAP_TILE_SKY, sizeof(wave_text));
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
