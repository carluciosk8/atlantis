#include "hud.h"

#include "tiles.h"


const u8 title_tiles_line1[] = { 0, 1 };
const u8 title_tiles_line2[] = { 2,3,4,5,6,7,8,32,32,2,3,4,9,6,4,5,6,4,5,6,7,5,6 };
const u8 title_tiles_line3[] = { 11,12,13,32,32,13,15,16,17,11,12,13,15,18,13,32,32,13,32,19,32,20,21,13 };
const u8 title_tiles_line4[] = { 22,23,24,32,32,24,26,27,24,22,23,24,26,28,24,32,32,24,26,27,24,26,27,24 };
const u8 title_tiles_line5[] = { 'H', 'I', 'T', ' ', 'T', 'R', 'I', 'G', 'G', 'E', 'R', ' ', 'T', 'O', ' ', 'S', 'T', 'A', 'R', 'T' };
const u8 title_tiles_line6[] = { '@', ' ', '2', '0', '2', '1', ' ', 'C', 'A', 'R', 'L', 'U', 'C', 'I', 'O', ' ', 'S', ' ', 'C', 'O', 'R', 'D', 'E', 'I', 'R', 'O' };
const u8 title_tiles_line7[] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };

u8 hud[] = { 0x20,0x20,0x20,0x53,0x43,0x4F,0x52,0x45,0x20,0x30,0x30,0x30,0x30,0x30,0x30,0x20,0x20,0x20,0x20,0x20,0x48,0x49,0x20,0x30,0x32,0x37,0x30,0x30,0x30,0x20,0x20,0x20,};


void hud_draw_hi_score()
{
    const u8 border[] = { ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };
    const u8 text1[] =  { ' ','C','O','N','G','R','A','T','U','L','A','T','I','O','N','S',' ','.',' ' };
    const u8 text2[] =  { ' ',' ','Y','O','U',' ','C','O','N','Q','U','E','R',' ','T','H','E',' ',' ' };
    const u8 text3[] =  { ' ',' ',' ',' ','H','I',' ','S','C','O','R','E',' ','.','.','.',' ',' ',' ' };
    TILES_PUT_BUFFER(6, 4, border, sizeof(border));
    TILES_PUT_BUFFER(6, 5, text1, sizeof(text1));
    TILES_PUT_BUFFER(6, 6, text2, sizeof(text2));
    TILES_PUT_BUFFER(6, 7, text3, sizeof(text3));
    TILES_PUT_BUFFER(6, 8, border, sizeof(border));

    // TODO(step 3): wait ~480 frames (ubox_wait_vsync loop with a frame counter).
    // TODO(step 4): each frame, flush the sprite attribute table (68 B at 0x1B00).
    // TODO(step 4): map_update(); (also needs the frame counter, Step 3)
}


void hud_draw_game_over()
{
    const u8 border[] = { ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };
    const u8 text[] =   { ' ','G','A','M','E',' ','O','V','E','R',' ' };
    TILES_PUT_BUFFER(11, 5, border, sizeof(border));
    TILES_PUT_BUFFER(11, 6, text, sizeof(text));
    TILES_PUT_BUFFER(11, 7, border, sizeof(border));

    // TODO(step 3): wait ~270 frames (ubox_wait_vsync loop with a frame counter).
    // TODO(step 4): each frame flush SAT + map_update() + lane_update_all() +
    //   torpedo_update_all() + collision_handle_torpedos().
    // TODO(step 4): clear all 17 sprite attributes (attr = 0) then flush SAT.
}


void draw_title_screen()
{
    VDP_EnableDisplay(FALSE);
    VDP_SetColor2(1, 1);   // border 1 (black); text 1 unused in Screen 2
    VDP_FillScreen_GM2(32);

    TILES_PUT_BUFFER(10, 4 , title_tiles_line1, 2);
    TILES_PUT_BUFFER(25, 4 , title_tiles_line1, 2);
    TILES_PUT_BUFFER( 5, 5 , title_tiles_line2, 23);
    TILES_PUT_BUFFER( 4, 6 , title_tiles_line3, 24);
    TILES_PUT_BUFFER( 4, 7 , title_tiles_line4, 24);
    TILES_PUT_BUFFER_CENTERED( 13, title_tiles_line5);
    TILES_PUT_BUFFER_CENTERED( 19, title_tiles_line6);
    VDP_EnableDisplay(TRUE);
}


void hud_show_title_screen()
{
    draw_title_screen();
    // TODO(step 6): wait_fire_button();   (input, ubox_read_ctl cursor + FIRE1)
    // TODO(step 3): "HIT TRIGGER TO START" blink loop (10 cycles of 7 frames
    //   off / 7 frames on), using ubox_wait_vsync() and
    //   TILES_PUT_BUFFER_CENTERED(13, title_tiles_line7 / title_tiles_line5).
}
