#include "music.h"

#include "arkos/akm_player.h"

#include "song.h"     // const u8 g_SongData[] (1108 B, exported for 0xD000)
#include "effects.h"  // const u8 g_SFXData[]  ( 283 B, exported for 0xE000)


// Fixed RAM addresses for the Arkos data (see docs/MIGRATION_PLAN.md target
// memory map). The binaries are position-dependent: their internal pointers are
// absolute addresses that assume the data sits at the export address, so the
// copies must land exactly here and nothing else may overlap these regions.
#define SONG_ADDR 0xD000   // song: 1108 B -> 0xD000..0xD454
#define SFX_ADDR  0xE000   // SFX:   283 B -> 0xE000..0xE11B


// Copy the song and SFX to their fixed addresses, initialise the SFX bank and
// start the game music (sub-song 0). Called once at boot, before the H_TIMI
// hook is installed, so no interrupt protection is needed here (the hook that
// calls AKM_Update() does not exist yet).
void music_init()
{
	Mem_Copy(g_SongData, (void*)SONG_ADDR, sizeof(g_SongData));
	Mem_Copy(g_SFXData, (void*)SFX_ADDR, sizeof(g_SFXData));
	AKM_InitSFX((void*)SFX_ADDR);
	AKM_Play(0, (void*)SONG_ADDR);
}


// Switch the music sub-song (0 = game, 1 = title, 2 = game over, 3 = hi-score).
// Runs on the main loop while the H_TIMI hook calls AKM_Update(), so the player
// state change is protected against the hook.
void music_play(u8 song)
{
	DisableInterrupt();
	AKM_Play(song, (void*)SONG_ADDR);
	EnableInterrupt();
}


// Trigger a sound effect (1-based number, as in the legacy mplayer). Runs on
// the main loop; protected against the hook for the same reason as music_play().
void sfx_play(u8 no, u8 chan, u8 vol)
{
	DisableInterrupt();
	AKM_PlaySFX(no, chan, vol);
	EnableInterrupt();
}
