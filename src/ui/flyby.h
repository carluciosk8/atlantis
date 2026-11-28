#pragma once

#include "msxgl.h"


// Gorgons crossing the title screen: at random times one gorgon, or sometimes
// two from opposite sides, flies between the logo and the start text. Uses the
// gorgon sprite planes; the caller flushes the sprite table after each vblank.

void flyby_init(void);      // hide every sprite and schedule the first crossing
void flyby_update(void);    // move the gorgons; call once per frame
