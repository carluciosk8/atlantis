#include "sprites.h"

#include "sprite_data.h"


// { Y, X, Pattern, Color }
struct VDP_Sprite sprites_attributes[SPRITE_COUNT] =
{
    {0, 0, 0, 0},       // gorgons
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},       // torpedoes
    {0, 0, 0, 0},
    {88,     0, SPRITE_PATTERN_DEATHRAY, COLOR_TRANSPARENT},    // deathray
    {88+16,  0, SPRITE_PATTERN_DEATHRAY, COLOR_TRANSPARENT},
    {88+32,  0, SPRITE_PATTERN_DEATHRAY, COLOR_TRANSPARENT},
    {88+48,  0, SPRITE_PATTERN_DEATHRAY, COLOR_TRANSPARENT},
    {88+64,  0, SPRITE_PATTERN_DEATHRAY, COLOR_TRANSPARENT},
    {0, 0, 0, 0},       // building explosion
    {0, 0, 0, 0},
};


//-----------------------------------------------------------------------------
// Load the horizontal mirror of a 16x16 pattern at 8x8 index `index`: the left
// and right columns swap and each byte's bits are reversed.
static void sprites_load_mirrored(const u8* src, u8 index)
{
    u8 buffer[32];
    for (u8 i = 0; i < 16; i++)
    {
        buffer[i]      = Math_Flip(src[16 + i]);
        buffer[16 + i] = Math_Flip(src[i]);
    }
    VDP_LoadSpritePattern(buffer, index, 4);
}


//-----------------------------------------------------------------------------
// Set 16x16 sprite mode and load every sprite pattern into VRAM.
// Must run after VDP_SetMode + VDP_ClearVRAM (ClearVRAM wipes the pattern table).
void sprites_init(void)
{
    VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16);

    // Gorgon fighters/bombers/bandits: pattern index p -> 8x8 index p*4, 4 units.
    // The flying-left patterns are mirrors of the flying-right ones.
    VDP_LoadSpritePattern(sprite_fighter1,  0,  4);
    VDP_LoadSpritePattern(sprite_fighter2,  4,  4);
    sprites_load_mirrored(sprite_fighter1,  8);
    sprites_load_mirrored(sprite_fighter2,  12);
    VDP_LoadSpritePattern(sprite_bomber1,   16, 4);
    VDP_LoadSpritePattern(sprite_bomber2,   20, 4);
    sprites_load_mirrored(sprite_bomber1,   24);
    sprites_load_mirrored(sprite_bomber2,   28);
    VDP_LoadSpritePattern(sprite_bandit1,   32, 4);
    VDP_LoadSpritePattern(sprite_bandit2,   36, 4);
    sprites_load_mirrored(sprite_bandit1,   40);
    sprites_load_mirrored(sprite_bandit2,   44);

    // Torpedoes (12-14), explosion frames (15-24) and deathray slots (25-28):
    // 17 16x16 patterns = 544 bytes at 8x8 index 48 (= 16x16 index 12).
    VDP_WriteVRAM_16K(sprite_patterns_fx, g_SpritePatternLow + 12*32, sizeof(sprite_patterns_fx));
}


//-----------------------------------------------------------------------------
// Make every sprite transparent in the shadow table.
void sprites_hide_all(void)
{
    for (u8 i = 0; i < SPRITE_COUNT; i++) sprites_attributes[i].Color = COLOR_TRANSPARENT;
}


//-----------------------------------------------------------------------------
// Write the sprite attribute table (17 x 4 = 68 bytes) to VRAM.
void sprites_flush(void)
{
    VDP_WriteVRAM_16K((const u8*)sprites_attributes, g_SpriteAttributeLow, sizeof(sprites_attributes));
}


//-----------------------------------------------------------------------------
// Set sprite magnification (R#1 bit 0). x2 magnification is global, so the
// bandit big explosion magnifies every sprite on screen. Always keeps the
// 16x16 size bit set.
void sprites_set_magnification(bool on)
{
    VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16 | (on ? VDP_SPRITE_SCALE_2 : VDP_SPRITE_SCALE_1));
}
