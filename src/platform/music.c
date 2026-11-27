#include "music.h"

#include "arkos/akm_player.h"

#include "song.h"     // const u8 g_SongData[] (1811 B, exported for 0xD000)
#include "effects.h"  // const u8 g_SFXData[]  ( 283 B, exported for 0xE000)


// Fixed RAM addresses of the Arkos data (see the memory map in CLAUDE.md). The
// binaries are position-dependent: their internal pointers assume the data
// sits at the export address, so the copies must land exactly here and
// nothing else may overlap these regions.
#define SONG_ADDR 0xD000   // song: 1811 B -> 0xD000..0xD713
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


// Silence the song and play the "game starts" jingle (SFX_START, a rising
// arpeggio that fades out over 45 frames). The jingle plays on channel 0, and
// the same SFX at inverted volume 16 (volume 0) on channels 1 and 2: while an
// SFX is active it owns its channel's PSG registers, so the song is muted on
// all three channels. AKM_Stop() cannot be used instead, because AKM_Update()
// does nothing once the song is stopped, so the SFX would not play either.
// The song comes back when the SFX ends, so call music_stop() before frame 45.
void music_play_start_jingle(void)
{
    DisableInterrupt();
    AKM_PlaySFX(SFX_START, 0, 0);
    AKM_PlaySFX(SFX_START, 1, 16);
    AKM_PlaySFX(SFX_START, 2, 16);
    EnableInterrupt();
}


// Stop the song and any SFX (silences the PSG). music_play() restarts it.
void music_stop(void)
{
    DisableInterrupt();
    AKM_Stop();
    EnableInterrupt();
}
