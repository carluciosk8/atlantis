#pragma once

#include <ubox.h>

#define TILES_MAKE_ADDRESS(X, Y, BASE)          ( (X) + (Y)*32 + (BASE) )
#define TILES_PUT_BUFFER(X, Y, BUFFER, SIZE)    ubox_write_vm((uint8_t*)0x1800 + (Y)*32 + (X), (SIZE), (BUFFER));
#define TILES_PUT_BUFFER_CENTERED(Y, BUFFER)    TILES_PUT_BUFFER(16-sizeof(BUFFER)/2, Y , BUFFER, sizeof(BUFFER));

void init_tiles();
