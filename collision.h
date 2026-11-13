#pragma once

#include "msxgl.h"

// TODO(step 4c): this is a stub header. The real collision.h (collision.c, not
// ported yet) also declares left_index / right_index and the collision_init /
// collision_handle_torpedos functions. lane.c only needs collision_handle_deathray
// for now, and it is stubbed as a no-op in lane.c.

void collision_handle_deathray(u8 x, i8 v);
