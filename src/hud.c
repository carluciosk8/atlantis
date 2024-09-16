#include "hud.h"

#include "city.h"
#include "collision.h"
#include "lane.h"
#include "sprites.h"
#include "tiles.h"
#include "torpedo.h"


const uint8_t title_tiles_line1[] = { 0, 1 };
const uint8_t title_tiles_line2[] = { 2,3,4,5,6,7,8,32,32,2,3,4,9,6,4,5,6,4,5,6,7,5,6 };
const uint8_t title_tiles_line3[] = { 11,12,13,32,32,13,15,16,17,11,12,13,15,18,13,32,32,13,32,19,32,20,21,13 };
const uint8_t title_tiles_line4[] = { 22,23,24,32,32,24,26,27,24,22,23,24,26,28,24,32,32,24,26,27,24,26,27,24 };
const uint8_t title_tiles_line5[] = { 'H', 'I', 'T', ' ', 'T', 'R', 'I', 'G', 'G', 'E', 'R', ' ', 'T', 'O', ' ', 'S', 'T', 'A', 'R', 'T' };
const uint8_t title_tiles_line6[] = { '@', ' ', '2', '0', '2', '1', ' ', 'C', 'A', 'R', 'L', 'U', 'C', 'I', 'O', ' ', 'S', ' ', 'C', 'O', 'R', 'D', 'E', 'I', 'R', 'O' };
const uint8_t title_tiles_line7[] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };

uint8_t hud[] = { 0x20,0x20,0x20,0x53,0x43,0x4F,0x52,0x45,0x20,0x30,0x30,0x30,0x30,0x30,0x30,0x20,0x20,0x20,0x20,0x20,0x48,0x49,0x20,0x30,0x32,0x37,0x30,0x30,0x30,0x20,0x20,0x20,};


void hud_draw_hi_score()
{
    const uint8_t border[] = { ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };
    const uint8_t text1[] =  { ' ','C','O','N','G','R','A','T','U','L','A','T','I','O','N','S',' ','.',' ' };
    const uint8_t text2[] =  { ' ',' ','Y','O','U',' ','C','O','N','Q','U','E','R',' ','T','H','E',' ',' ' };
    const uint8_t text3[] =  { ' ',' ',' ',' ','H','I',' ','S','C','O','R','E',' ','.','.','.',' ',' ',' ' };
    TILES_PUT_BUFFER(6, 4, border, sizeof(border));
    TILES_PUT_BUFFER(6, 5, text1, sizeof(text1));
    TILES_PUT_BUFFER(6, 6, text2, sizeof(text2));
    TILES_PUT_BUFFER(6, 7, text3, sizeof(text3));
    TILES_PUT_BUFFER(6, 8, border, sizeof(border));

    int frame_counter = 0;
    while (frame_counter < 480)
    {
        ubox_wait_vsync();
        ubox_write_vm((uint8_t*)0x1b00, 68, (uint8_t*)sprites_attributes);
        map_update();
        frame_counter++;
    }
}


void hud_draw_game_over()
{
    const uint8_t border[] = { ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };
    const uint8_t text[] =   { ' ','G','A','M','E',' ','O','V','E','R',' ' };
    TILES_PUT_BUFFER(11, 5, border, sizeof(border));
    TILES_PUT_BUFFER(11, 6, text, sizeof(text));
    TILES_PUT_BUFFER(11, 7, border, sizeof(border));

    int frame_counter = 0;
    while (frame_counter < 270)
    {
        ubox_wait_vsync();
        ubox_write_vm((uint8_t*)0x1b00, 68, (uint8_t*)sprites_attributes);
        map_update();
        lane_update_all();
        torpedo_update_all();
        collision_handle_torpedos();
        frame_counter++;
    }

    for (uint8_t i=0 ; i<17 ; i++) sprites_attributes[i].attr = 0;
    ubox_write_vm((uint8_t*)0x1b00, 68, (uint8_t*)sprites_attributes);
}


void draw_title_screen()
{
    ubox_disable_screen();
    ubox_set_colors(1,1,1);
    ubox_fill_screen(32);

    TILES_PUT_BUFFER(10, 4 , title_tiles_line1, 2);
    TILES_PUT_BUFFER(25, 4 , title_tiles_line1, 2);
    TILES_PUT_BUFFER( 5, 5 , title_tiles_line2, 23);
    TILES_PUT_BUFFER( 4, 6 , title_tiles_line3, 24);
    TILES_PUT_BUFFER( 4, 7 , title_tiles_line4, 24);
    TILES_PUT_BUFFER_CENTERED( 13, title_tiles_line5);
    TILES_PUT_BUFFER_CENTERED( 19, title_tiles_line6);
    ubox_enable_screen();
}


void wait_fire_button()
{
    while (!(ubox_read_ctl(UBOX_MSX_CTL_CURSOR) & UBOX_MSX_CTL_FIRE1)) { }
    while ((ubox_read_ctl(UBOX_MSX_CTL_CURSOR) & UBOX_MSX_CTL_FIRE1)) { }
}


void hud_show_title_screen()
{
    draw_title_screen();
    wait_fire_button();
    for (uint8_t f=0 ; f<10 ; f++) 
    {
        for (uint8_t i=0 ; i<7 ; i++)
        { 
            ubox_wait_vsync();
            TILES_PUT_BUFFER_CENTERED( 13, title_tiles_line7);
        }
        for (uint8_t j=0 ; j<7 ; j++)
        { 
            ubox_wait_vsync();
            TILES_PUT_BUFFER_CENTERED( 13, title_tiles_line5);
        } 
    }
}
