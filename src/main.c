#include <ubox.h>
#include <mplayer.h>

#include "city.h"
#include "collision.h"
#include "hud.h"
#include "input.h"
#include "lane.h"
#include "sprites.h"
#include "score.h"
#include "tiles.h"
#include "torpedo.h"
#include "wave.h"


// Arkos data
extern uint8_t SONG[];
extern uint8_t EFFECTS[];


void main( void )
{
    ubox_reset_tick();
    ubox_init_isr(1);
    ubox_set_mode(2);

    init_tiles();
    init_sprites();

    mplayer_init(SONG, 0);
    mplayer_init_effects(EFFECTS);
    ubox_set_user_isr(mplayer_play);

    for(;;)
    {
        mplayer_init(SONG, 1);

        hud_show_title_screen();

        mplayer_init(SONG, 0);

        collision_init();
        lane_init_all();
        map_init();
        score_init();
        torpedo_init_all();
        wave_init();

        while ( left_index <= right_index || map_have_installation[0] )
        {
            wave_start();
            wave_draw_hud();

            while (g_wave_size > 0)
            {
                handle_input();
                ubox_wait_vsync();
                ubox_write_vm((uint8_t*)0x1b00, 68, (uint8_t*)sprites_attributes);
                map_update();
                lane_update_all();
                torpedo_update_all();
                collision_handle_torpedos();
            }

            wave_finish();
        }

        mplayer_init(SONG, 2);
        hud_draw_game_over();

        if (score_check_hi())
        {
            mplayer_init(SONG, 3);
            score_promote_hi();
            hud_draw_hi_score();
        }
    }   
}
