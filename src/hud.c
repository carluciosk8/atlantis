#include "hud.h"

#include "city.h"
#include "collision.h"
#include "controls.h"
#include "lane.h"
#include "sprites.h"
#include "tiles.h"
#include "torpedo.h"


// Title logo, drawn with tileset tiles.
const u8 title_logo_row1[] = { 0, 1 };
const u8 title_logo_row2[] = { 2,3,4,5,6,7,8,32,32,2,3,4,9,6,4,5,6,4,5,6,7,5,6 };
const u8 title_logo_row3[] = { 11,12,13,32,32,13,15,16,17,11,12,13,15,18,13,32,32,13,32,19,32,20,21,13 };
const u8 title_logo_row4[] = { 22,23,24,32,32,24,26,27,24,22,23,24,26,28,24,32,32,24,26,27,24,26,27,24 };

const u8 title_press_start[] = { 'H','I','T',' ','T','R','I','G','G','E','R',' ','T','O',' ','S','T','A','R','T' };
const u8 title_copyright[]   = { '@',' ','2','0','2','1',' ','C','A','R','L','U','C','I','O',' ','S',' ','C','O','R','D','E','I','R','O' };
const u8 title_blank[]       = { ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };

u8 hud[] = { ' ',' ',' ','S','C','O','R','E',' ','0','0','0','0','0','0',' ',' ',' ',' ',' ','H','I',' ','0','0','0','2','7','0',' ',' ',' ' };


void hud_draw_hi_score(void)
{
    static const u8 text1[]  = { ' ','C','O','N','G','R','A','T','U','L','A','T','I','O','N','S',' ','.',' ' };
    static const u8 text2[]  = { ' ',' ','Y','O','U',' ','C','O','N','Q','U','E','R',' ','T','H','E',' ',' ' };
    static const u8 text3[]  = { ' ',' ',' ',' ','H','I',' ','S','C','O','R','E',' ','.','.','.',' ',' ',' ' };
    TILES_FILL(6, 4, ' ', sizeof(text1));
    TILES_PUT_BUFFER(6, 5, text1, sizeof(text1));
    TILES_PUT_BUFFER(6, 6, text2, sizeof(text2));
    TILES_PUT_BUFFER(6, 7, text3, sizeof(text3));
    TILES_FILL(6, 8, ' ', sizeof(text1));

    // Wait ~480 frames (8.0 s at 60 Hz, 9.6 s at 50 Hz).
    for (u16 frame = 0; frame < 480; frame++)
    {
        Halt();
        sprites_flush();
        map_update();
    }
}


void hud_draw_game_over(void)
{
    static const u8 text[]   = { ' ','G','A','M','E',' ','O','V','E','R',' ' };
    TILES_FILL(11, 5, ' ', sizeof(text));
    TILES_PUT_BUFFER(11, 6, text, sizeof(text));
    TILES_FILL(11, 7, ' ', sizeof(text));

    // Keep the game running for ~270 frames (4.5 s at 60 Hz, 5.4 s at 50 Hz).
    for (u16 frame = 0; frame < 270; frame++)
    {
        Halt();
        sprites_flush();
        map_update();
        lane_update_all();
        torpedo_update_all();
        collision_handle_torpedoes();
    }

    for (u8 i = 0; i < SPRITE_COUNT; i++) sprites_attributes[i].Color = COLOR_TRANSPARENT;
    sprites_flush();
}


static void hud_draw_title(void)
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
    TILES_PUT_BUFFER_CENTERED(19, title_copyright);
    VDP_EnableDisplay(TRUE);
}


void hud_show_title_screen(void)
{
    hud_draw_title();

    controls_wait_fire();

    // Blink "HIT TRIGGER TO START": 10 cycles of 7 frames off / 7 frames on
    // (140 vblanks, ~2.33 s at 60 Hz, 2.8 s at 50 Hz).
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
