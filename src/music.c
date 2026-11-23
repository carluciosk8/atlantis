#include "music.h"

#include "arkos/akm_player.h"

#include "song.h"     // const u8 g_SongData[] (1108 B, exported for 0xD000)
#include "effects.h"  // const u8 g_SFXData[]  ( 283 B, exported for 0xE000)


// Fixed RAM addresses of the Arkos data (see the memory map in CLAUDE.md). The
// binaries are position-dependent: their internal pointers assume the data
// sits at the export address, so the copies must land exactly here and
// nothing else may overlap these regions.
#define SONG_ADDR 0xD000   // song: 1108 B -> 0xD000..0xD454
#define SFX_ADDR  0xE000   // SFX:   283 B -> 0xE000..0xE11B


// Copy the song and SFX to their fixed addresses, initialise the SFX bank and
// start the game music. Called once at boot, before the H_TIMI hook that calls
// AKM_Update() is installed, so no interrupt protection is needed here.
void music_init(void)
{
    Mem_Copy(g_SongData, (void*)SONG_ADDR, sizeof(g_SongData));
    Mem_Copy(g_SFXData, (void*)SFX_ADDR, sizeof(g_SFXData));
    AKM_InitSFX((void*)SFX_ADDR);
    AKM_Play(MUSIC_GAME, (void*)SONG_ADDR);
}


// Switch the music sub-song (MUSIC_*). Runs on the main loop while the H_TIMI
// hook calls AKM_Update(), so the player state change is protected from it.
void music_play(u8 song)
{
    DisableInterrupt();
    AKM_Play(song, (void*)SONG_ADDR);
    EnableInterrupt();
}


// Trigger a sound effect (SFX_*) on channel 0-2. Protected from the hook for
// the same reason as music_play().
void sfx_play(u8 sfx, u8 channel, u8 inverted_volume)
{
    DisableInterrupt();
    AKM_PlaySFX(sfx, channel, inverted_volume);
    EnableInterrupt();
}
