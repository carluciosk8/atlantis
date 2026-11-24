#include "torpedo.h"

#include "music.h"
#include "sprites.h"


torpedo_t torpedoes[2];

u8 torpedo_next = 0;        // slot used by the next shot (alternates 0 / 1)


void torpedo_init(u8 n)
{
    torpedoes[n].sprite = &sprites_attributes[SPRITE_PLANE_TORPEDO + n];
    torpedoes[n].sprite->Pattern = TORPEDO_CENTER_PATT;
    torpedoes[n].is_active = 0;
    torpedoes[n].sprite->X = 0;
    torpedoes[n].sprite->Y = SPRITE_Y_HIDDEN;
    torpedoes[n].sprite->Color = COLOR_BLACK;
    torpedoes[n].dx = 0;
    torpedoes[n].dy = 0;
}


void torpedo_init_all(void)
{
    torpedo_init(0);
    torpedo_init(1);
}


static void torpedo_update(torpedo_t* torpedo)
{
    if (torpedo->is_active)
    {
        struct VDP_Sprite* sprite = torpedo->sprite;
        u8 y = sprite->Y + torpedo->dy;
        sprite->X += torpedo->dx;

        // Leaving the top of the screen wraps the 8-bit Y past 191: park it hidden.
        if (y > 191)
        {
            torpedo->is_active = 0;
            y = SPRITE_Y_HIDDEN;
        }
        sprite->Y = y;
    }
}


void torpedo_update_all(void)
{
    torpedo_update(&torpedoes[0]);
    torpedo_update(&torpedoes[1]);
}


void torpedo_fire(u8 pattern, u8 x, u8 y, i8 dx, i8 dy)
{
    torpedo_t* torpedo = &torpedoes[torpedo_next];
    struct VDP_Sprite* sprite = torpedo->sprite;

    sfx_play(SFX_TORPEDO, 2, 0);
    sprite->Pattern = pattern;
    torpedo->is_active = 1;
    sprite->X = x;
    sprite->Y = y;
    torpedo->dx = dx;
    torpedo->dy = dy;

    torpedo_next ^= 1;
}
