#pragma once

#include "msxgl.h"

// TODO(step 4c): this is a stub header. The real torpedo.h (torpedo.c, not
// ported yet) also declares the TORPEDO_* launch constants and the torpedo_*
// functions. lane.c only needs the torpedo_t type and the torpedos[] array for
// now; the array is stubbed in lane.c.

typedef struct
{
    struct VDP_Sprite* sprite;
    u8 sprite_number;
    u8 is_active;
    i8 dx;
    i8 dy;
} torpedo_t;

extern torpedo_t torpedos[2];
