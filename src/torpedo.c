#include "torpedo.h"

#include "music.h"
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


static void torpedo_update(torpedo_t* t)
{
    if (t->is_active)
    {
        struct VDP_Sprite* sprite = t->sprite;
        u8 y = sprite->Y + t->dy;
        sprite->X += t->dx;
        if (y > 191)
        {
            t->is_active = 0;
            y = 192;
        }
        sprite->Y = y;
    }
}


void torpedo_update_all()
{
    torpedo_update(&torpedos[0]);
    torpedo_update(&torpedos[1]);
}


void torpedo_fire(u8 pattern, u8 x, u8 y, i8 dx, i8 dy)
{
    torpedo_t* t = &torpedos[torpedo_current];
    struct VDP_Sprite* sprite = t->sprite;

    sfx_play(1, 2, 0);
    sprite->Pattern = pattern;
    t->is_active = 1;
    sprite->X = x;
    sprite->Y = y;
    t->dx = dx;
    t->dy = dy;
    torpedo_current ^= 1;   // 0 <-> 1
    torpedo_is_fired = 0;
}
