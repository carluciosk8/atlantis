#include "torpedo.h"

#include "sprites.h"


torpedo_t torpedos[2];

u8 torpedo_current = 0;
u8 torpedo_is_fired = 0;


void torpedo_init(u8 n)
{
    torpedos[n].sprite = &sprites_attributes[8 + n];
    torpedos[n].sprite->Pattern = 12*4;
    torpedos[n].is_active = 0;
    torpedos[n].sprite->X = 0;
    torpedos[n].sprite->Y = 192;
    torpedos[n].sprite->Color = 1;
    torpedos[n].dx = 0;
    torpedos[n].dy = 0;
}


void torpedo_init_all()
{
    torpedo_init(0);
    torpedo_init(1);
}


void torpedo_update(u8 n)
{
    if (torpedos[n].is_active)
    {
        torpedos[n].sprite->X += torpedos[n].dx;
        torpedos[n].sprite->Y += torpedos[n].dy;
        if (torpedos[n].sprite->Y > 191)
        {
            torpedos[n].is_active = 0;
            torpedos[n].sprite->Y = 192;
        }
    }
}


void torpedo_update_all()
{
    torpedo_update(0);
    torpedo_update(1);
}


void torpedo_fire(u8 pattern, u8 x, u8 y, i8 dx, i8 dy)
{
    // TODO(step 5): mplayer_play_effect(1, 2, 0)  →  AKM_PlaySFX(1, 2, 0)
    torpedos[torpedo_current].sprite->Pattern = pattern;
    torpedos[torpedo_current].is_active = 1;
    torpedos[torpedo_current].sprite->X = x;
    torpedos[torpedo_current].sprite->Y = y;
    torpedos[torpedo_current].dx = dx;
    torpedos[torpedo_current].dy = dy;
    torpedo_current = (torpedo_current + 1) % 2;
    torpedo_is_fired = 0;
}
