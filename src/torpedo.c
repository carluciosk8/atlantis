#include "torpedo.h"

#include "sprites.h"

#include <mplayer.h>


torpedo_t torpedos[2];

uint8_t torpedo_current = 0;
uint8_t torpedo_is_fired = 0;


void torpedo_init(uint8_t n)
{
    torpedos[n].sprite = &sprites_attributes[8 + n];
    torpedos[n].sprite->pattern = 12*4;
    torpedos[n].is_active = 0;
    torpedos[n].sprite->x = 0;
    torpedos[n].sprite->y = 192;
    torpedos[n].sprite->attr = 1;
    torpedos[n].dx = 0;
    torpedos[n].dy = 0;
}


void torpedo_init_all()
{
    torpedo_init(0);
    torpedo_init(1);
}


void torpedo_update(uint8_t n)
{
    if (torpedos[n].is_active)
    {
        torpedos[n].sprite->x += torpedos[n].dx;
        torpedos[n].sprite->y += torpedos[n].dy;
        if (torpedos[n].sprite->y > 191)
        {
            torpedos[n].is_active = 0;
            torpedos[n].sprite->y = 192;
        }
    }
}


void torpedo_update_all()
{
    torpedo_update(0);
    torpedo_update(1);
}


void torpedo_fire(uint8_t pattern, uint8_t x, uint8_t y, int8_t dx, int8_t dy)
{
    mplayer_play_effect(1, 2, 0);
    torpedos[torpedo_current].sprite->pattern = pattern;
    torpedos[torpedo_current].is_active = 1;
    torpedos[torpedo_current].sprite->x = x;
    torpedos[torpedo_current].sprite->y = y;
    torpedos[torpedo_current].dx = dx;
    torpedos[torpedo_current].dy = dy;
    torpedo_current = (torpedo_current + 1) % 2;
    torpedo_is_fired = 0;
}