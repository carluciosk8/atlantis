#pragma once

#include "msxgl.h"


// Temporary auto-fire toggle (Step 4d only). 1 = fire torpedoes on a timer
// (exercises collisions, score and the deathray); 0 = nothing fires, so the
// gorgons reach the last lane and the deathray can be observed on its own.
// Defined here (shared header) so both input.c and atlantis.c can see it.
// Removed with handle_input() in Step 6.
#define TEST_AUTOFIRE 1  // TODO(step 6: remove)


void handle_input();
void wait_fire_button();
