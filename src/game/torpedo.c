#include "torpedo.h"

#include "music.h"
#include "sprites.h"


// Launch constants. The *_PATT values name the LAUNCHER side, not the pointing
// direction: TORPEDO_LEFT_PATT (13*4) is launched from the left launcher and is
// drawn pointing right (pattern "torpedo_right"), and TORPEDO_RIGHT_PATT (14*4)
// from the right launcher pointing left. Do not rename or "fix" them. Pattern
// values are in 8x8 units (16x16 pattern index * 4), matching the sprite
// attribute layout.
#define TORPEDO_LEFT_PATT   13*4
#define TORPEDO_CENTER_PATT 12*4
#define TORPEDO_RIGHT_PATT  14*4

typedef struct
{
    u8 pattern;
    u8 x;
    u8 y;
    i8 dx;
    i8 dy;
} launcher_t;

// Indexed by TORPEDO_LAUNCHER_*.
static const launcher_t torpedo_launchers[3] =
{
//  { pattern,             x,   y,   dx, dy },
    { TORPEDO_LEFT_PATT,   14,  101,  4, -2 },   // left
    { TORPEDO_CENTER_PATT, 126, 93,   0, -3 },   // centre
    { TORPEDO_RIGHT_PATT,  238, 94,  -4, -2 },   // right
};


torpedo_t torpedoes[2];

u8 torpedo_next = 0;        // slot used by the next shot (alternates 0 / 1)


static void torpedo_init(u8 n)
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


void torpedo_hide(u8 index)
{
    torpedoes[index].is_active = 0;
    torpedoes[index].sprite->X = 0;
    torpedoes[index].sprite->Y = SPRITE_Y_HIDDEN;
}


void torpedo_fire(u8 launcher)
{
    const launcher_t* l = &torpedo_launchers[launcher];
    torpedo_t* torpedo = &torpedoes[torpedo_next];
    struct VDP_Sprite* sprite = torpedo->sprite;

    sfx_play(SFX_TORPEDO, 2, 0);
    sprite->Pattern = l->pattern;
    torpedo->is_active = 1;
    sprite->X = l->x;
    sprite->Y = l->y;
    torpedo->dx = l->dx;
    torpedo->dy = l->dy;

    torpedo_next ^= 1;
}
