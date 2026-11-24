#pragma once

#include "msxgl.h"


// The deathray: fired by the gorgon flying in the last lane. It is armed when
// that gorgon enters the lane, follows it across the screen (5 stacked 16x16
// sprites) and destroys at most one installation per pass.

// Arm the ray: a gorgon has just entered the last lane.
void deathray_arm(void);

// Hide the ray sprites (the gorgon was shot down or left the screen).
void deathray_hide(void);

// Called every frame by the last lane while its gorgon is moving, with the
// gorgon's X and velocity. Moves and animates the ray, plays its sound and
// tests it against the city. Does nothing once the ray has hit something.
void deathray_update(u8 x, i8 velocity);
