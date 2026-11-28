#include "flyby.h"

#include "gorgons.h"
#include "sprites.h"


#define FLYBY_MAX           2

// Pixels per frame, as the slowest gorgons in game; bandits fly 1 faster.
#define FLYBY_SPEED         2

// Frames from the title draw, or from the end of a crossing, to the next one:
// FLYBY_DELAY_MIN plus a random 0..255 (2.0..6.2 s at 60 Hz).
#define FLYBY_DELAY_MIN     120

// Sprite Y. The empty band between the logo and "HIT TRIGGER TO START" is
// screen lines 64..103, and the gorgons are drawn in lines 8..16 below their
// sprite Y. One gorgon flies in the middle of the band; two fly in the middle of
// its two halves.
#define FLYBY_Y_SINGLE      72
#define FLYBY_Y_TOP         62
#define FLYBY_Y_BOTTOM      82

// A gorgon enters and leaves just outside the screen: X runs from -16 to 256.
#define FLYBY_X_LEFT        (-16)
#define FLYBY_X_RIGHT       256

// Sprite colour bit 7 (early clock) draws the sprite 32 pixels to the left.
#define FLYBY_EARLY_CLOCK   0x80

typedef struct
{
    bool active;
    u8 type;        // 0 = fighter, 1 = bomber, 2 = bandit
    u8 id;          // GORGON_ID_*
    u8 y;
    i16 x;
} flyby_t;

// The sky is black, so the gorgons swap their black for the light shade of their
// own colour, by type. The second sprite keeps its dark shade (g_gorgon_list).
static const u8 flyby_light[GORGON_ID_TOTAL / 2] = { COLOR_LIGHT_GREEN, COLOR_LIGHT_RED, COLOR_LIGHT_BLUE };

static flyby_t flyby_gorgons[FLYBY_MAX];
static u8 flyby_count;      // gorgons crossing now
static u16 flyby_delay;     // frames to the next crossing, while none is


static void flyby_wait(void)
{
    flyby_delay = FLYBY_DELAY_MIN + (u8)Math_GetRandom16();
}


// Write the two overlaid sprites of gorgon `index`.
static void flyby_place(u8 index)
{
    const flyby_t* gorgon = &flyby_gorgons[index];
    const gorgon_t* look = &g_gorgon_list[gorgon->id];
    struct VDP_Sprite* sprite = &sprites_attributes[SPRITE_PLANE_GORGON + 2*index];

    u8 x = (u8)gorgon->x;
    u8 clock = 0;
    if (gorgon->x < 0)
    {
        x = (u8)(gorgon->x + 32);
        clock = FLYBY_EARLY_CLOCK;
    }

    sprite[0].Y = gorgon->y;
    sprite[0].X = x;
    sprite[0].Pattern = look->pattern1;
    sprite[0].Color = flyby_light[gorgon->type] | clock;

    sprite[1].Y = gorgon->y;
    sprite[1].X = x;
    sprite[1].Pattern = look->pattern2;
    sprite[1].Color = look->color2 | clock;
}


// Start gorgon `index`, of a random type, at height `y` from the left or right edge.
static void flyby_start(u8 index, u8 y, u8 from_right)
{
    flyby_t* gorgon = &flyby_gorgons[index];
    gorgon->type = (u8)(Math_GetRandom16() % (GORGON_ID_TOTAL / 2));
    gorgon->id = gorgon->type * 2 + from_right;   // odd ids fly to the left
    gorgon->y = y;
    gorgon->x = from_right ? FLYBY_X_RIGHT : FLYBY_X_LEFT;
    gorgon->active = TRUE;
    flyby_count++;
    // Not placed yet: x may be 256, which the first update moves into range.
}


// Launch one gorgon, or 1 time in 4 two of them from opposite sides.
static void flyby_launch(void)
{
    u8 pair = (Math_GetRandom16() & 3) == 0;
    u8 from_right = (u8)Math_GetRandom16() & 1;

    if (pair)
    {
        flyby_start(0, FLYBY_Y_TOP, from_right);
        flyby_start(1, FLYBY_Y_BOTTOM, from_right ^ 1);
    }
    else
    {
        flyby_start(0, FLYBY_Y_SINGLE, from_right);
    }
}


void flyby_init(void)
{
    sprites_hide_all();
    flyby_gorgons[0].active = FALSE;
    flyby_gorgons[1].active = FALSE;
    flyby_count = 0;
    flyby_wait();
}


void flyby_update(void)
{
    if (flyby_count == 0)
    {
        if (--flyby_delay == 0) flyby_launch();
        return;
    }

    flyby_t* gorgon = flyby_gorgons;
    for (u8 i = 0; i < FLYBY_MAX; i++, gorgon++)
    {
        if (!gorgon->active) continue;

        i8 speed = GORGON_IS_BANDIT(gorgon->id) ? FLYBY_SPEED + 1 : FLYBY_SPEED;
        if (GORGON_MOVES_LEFT(gorgon->id)) gorgon->x -= speed;
        else                               gorgon->x += speed;

        if (gorgon->x <= FLYBY_X_LEFT || gorgon->x >= FLYBY_X_RIGHT)
        {
            gorgon->active = FALSE;
            sprites_attributes[SPRITE_PLANE_GORGON + 2*i].Color = COLOR_TRANSPARENT;
            sprites_attributes[SPRITE_PLANE_GORGON + 2*i + 1].Color = COLOR_TRANSPARENT;
            if (--flyby_count == 0) flyby_wait();
        }
        else
        {
            flyby_place(i);
        }
    }
}
