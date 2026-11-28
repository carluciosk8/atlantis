# Music and sound effects

How the song and the sound effects are built, what they contain, and what was learned making them. The songs were rewritten in 6519007; the sound effects are the original ones.

## Current data
| File | Size | RAM copy |
|---|---|---|
| `assets/sfx/song.akm` → `src/data/song.h` (`g_SongData`) | 1811 B | 0xD000..0xD713 |
| `assets/sfx/effects.akx` → `src/data/effects.h` (`g_SFXData`) | 283 B | 0xE000..0xE11B |

The song region 0xD000..0xDFFF has room for 4096 B before the SFX at 0xE000.

## Sub-songs (the index `music_play()` uses)
| # | Name | Key / tempo | Notes |
|---|---|---|---|
| 0 | Game | D Phrygian, speed 6 | 6 patterns, loops from pattern 2 (index 1). Intro, theme, answer, varied theme, climax, answer. Harp ripple, shaker, lead. |
| 1 | Title | E Phrygian, speed 8 | 5 patterns, loops from index 1. Sparse harp or soft pad, slow bass, no drums. |
| 2 | Game over | D Phrygian, speed 6 | One 48-row pattern (~4.8 s). The game plays it ~4.5 s, so the phrase ends on D4 just before the screen changes. |
| 3 | Hi Score | D Phrygian, speed 6 | 2 patterns, ~12.8 s, loops. The screen shows it for ~8 s. |

## Instruments in `song.aks`
The songs use only 9 Deep Bass, 10 Glass Lead, 11 Harp, 12 Deep Pad (title only) and 13 Soft Shaker.

The other instruments (1-8 from the original song, and 14 Soft Tom) are not used by any track. The AKM export already leaves unused instruments out: its instrument table has 6 entries, the empty instrument 0 plus the 5 used ones. So they cost no ROM or RAM. Deleting them would only tidy the source, and `tools/gen_song.py` would then no longer match `song.aks`.

## Sound effects (`effects.aks`)
| # | Name | Use (`music.h`) | Channel |
|---|---|---|---|
| 1 | Shot | `SFX_TORPEDO` | 2 |
| 2 | Deathray | `SFX_DEATHRAY` | 0 |
| 3 | Enemy explodes | `SFX_EXPLOSION` | 1 |
| 4 | Enemy explodes big | `SFX_BIG_EXPLOSION` | 1 |
| 5 | Crash | `SFX_INSTALLATION_LOST` | 0 |
| 6 | Start | `SFX_START` (`music_play_start_jingle`, title → game) | 0; also on 1 and 2 at inverted volume 16 to mute the song |
| 7 | Empty | unused | |

"Start" is a rising arpeggio that fades from volume 15 to 1 over 15 steps of 3 frames (45 frames). "Empty" is exported into `effects.akx` but unused, so it takes ROM and SFX RAM. Deleting it in Arkos Tracker (it is the last one) keeps numbers 1-6 unchanged; after that, re-export as described below and update the SFX size in `src/platform/music.c` and in the CLAUDE.md memory map.

## Pause chime
Not in `effects.aks`: `music_play_pause_chime` (`src/platform/music.c`) writes it straight to PSG channel A while the player is stopped, so the paused song and SFX do not advance. D6, A5, D5 on pause and the reverse on resume; 5, 5 and 12 frames, each note starting at volume 13 and fading by 1 per frame. The notes and timing are the `CHIME_*` constants.

## Lessons learned (do not repeat)
- **Volume effects stick to a track** until the next volume effect. Every note on a channel needs an explicit volume, or one quiet note makes the following ones quiet. The generator always sets one.
- **Any note on a channel cuts the previous note on it.** Drums on the bass channel cut the bass sustain. The drums are on the harp channel's off-beat rows, and the bass channel only carries bass.
- **Noise in an instrument's first cell is audible** as a "tchhh" (that was the bass click). Keep noise out of pitched instruments.
- **The hardware envelope** (`hardOnly`, `softToHard`) makes the old bass buzzy and very low. There is one envelope for the whole chip, so avoid it.
- **`AKM_Stop()` also stops the SFX**: once the song is stopped, `AKM_Update()` returns without calling the player. To silence the song while an SFX plays, play an SFX at inverted volume 16 on the other channels: an active SFX owns its channel's PSG registers (`music_play_start_jingle`). `AKM_Play()` clears all SFX channels.
- Note naming: `A4` = 440 Hz (checked by rendering). Arkos note number = octave*12 + semitone (C=0).
- Final volumes (track volume + master 3, capped at 15): lead 15, bass 15 (12+3), harp 15 / 13, shaker 11 / 9. The master offset is `MASTER` in the generator. The music sounded quiet against the SFX in game, so it was raised, and there is no headroom left. If it needs to be louder, lower the SFX instead: `sfx_play(sfx, channel, inverted_volume)` takes 0 = loudest, and every call passes 0 today.

## Workflow (native Linux, no Arkos GUI needed)
Tools: the `tools/` folder of the Arkos Tracker 3 install (`SongToAkm`, `SongToSoundEffects`, `SongToWav`). The GUI is the `ArkosTracker3` binary next to it.

1. Generator: `tools/gen_song.py` builds `song.aks` from the original song, the new instruments and the compositions. It is a plain Python script (run it with `python3 -I`) and expects the original as `song_v1.xml` in the current directory. In a scratch directory, take the original from git history:
   `git -C <repo> show 3039941:assets/sfx/song.aks > song_v1.aks`
   `unzip -p song_v1.aks | tr -d '\r' | grep -v '^\s*$' > song_v1.xml`
   Run the script there. It writes `song.xml` and `song.aks`; copy `song.aks` to `assets/sfx/`. The compositions and `MASTER` are at the bottom of the script.
   You can also edit `song.aks` directly in the Arkos GUI. The script then no longer matches, so use one or the other.
2. Export: `SongToAkm -bin -adr 0xD000 song.aks song.akm`. The `-s` option of the tools is 1-based (1 = Game … 4 = Hi Score).
3. Listen without an emulator: `SongToWav -s <1..4> song.aks out.wav`.
4. Header: `MSXbin assets/sfx/song.akm -t g_SongData -ad -o song.h`. MSXbin returns a non-zero exit code even on success, so do not chain it with `&&`. Change the `Soure file:` line (MSXbin's spelling) to a relative path and move the result to `src/data/song.h`.
5. Update the size in `src/platform/music.c` (two comments) and the song row of the memory map in `CLAUDE.md`.
6. Run `./build.sh` and report ROM and RAM against the baseline in CLAUDE.md.

The SFX follow the same steps: `SongToSoundEffects -bin -adr 0xE000 effects.aks effects.akx`, then `MSXbin assets/sfx/effects.akx -t g_SFXData -ad -o effects.h`.
