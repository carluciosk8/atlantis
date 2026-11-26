# Music and SFX handoff

State at the end of the first music session (2026-11-24). Nothing here is committed yet.

## Done
- New songs for all four sub-songs, in `assets/sfx/song.aks` (source) and `song.akm` (export at 0xD000). They replaced the original files of the same names, which commit `3039941` still has.
- `src/data/song.h` is generated from `song.akm`. Size 1811 B, so 0xD000..0xD713. `music.c` comments and the CLAUDE.md memory map were updated.
- Build at that point: ROM 17409 B (0x4000-0x8400), RAM 464 B.
- Effects (`effects.aks/.akx`, `effects.h`) are **not touched yet**.

## Sub-songs (index = what `music_play()` uses)
| # | Name | Key / tempo | Notes |
|---|---|---|---|
| 0 | Game | D Phrygian, speed 6 | 6 patterns, loops from pattern 2 (index 1). Intro, theme, answer, varied theme, climax, answer. Harp ripple, shaker/kick, lead. |
| 1 | Title | E Phrygian, speed 8 | 5 patterns, loops from index 1. Sparse harp or soft pad, slow bass, no drums. Deliberately different from the game song. |
| 2 | Game over | D Phrygian, speed 6 | One 48-row pattern (~4.8 s). The game plays it ~4.5 s, so the phrase ends on D4 just before the screen changes. |
| 3 | Hi Score | D Phrygian, speed 6 | 2 patterns, ~12.8 s, loops. The screen shows ~8 s. |

The old "Silence" sub-song was replaced by the Game song, and the old "In game" song (which used to play on the title) was replaced by the new Title.

## Instruments (added to song.aks, numbers 9-14; old 1-8 kept)
| # | Name | Use |
|---|---|---|
| 9 | Deep Bass | Bass. Plain tone, no noise, no hardware envelope. Notes at octave 2/3. |
| 10 | Glass Lead | Lead. Breathy onset, slow swell, vibrato loop. |
| 11 | Harp | Arpeggio pluck with a bright octave attack. |
| 12 | Deep Pad | Soft sustained chord note with slight vibrato (title only). |
| 13 | Soft Shaker | Quiet noise hat. |
| 14 | Soft Tom | Defined, **unused**. |
The old kick (7) is still used in "full" game patterns. Old instruments 1-6 and 8 are unused now (kept so nothing else breaks).

## Lessons learned (do not repeat)
- **Volume effects stick to a track** until the next volume effect. Every note on a channel needs an explicit volume or a quiet note makes the following ones quiet. The generator always sets one.
- **Any note on a channel cuts the previous note on it.** Drums on the bass channel cut the bass sustain. Drums now live on the harp channel's off-beat rows, and the bass channel only carries bass.
- **Noise in an instrument's first cell is audible** as a "tchhh" (that was the bass click). Keep noise out of pitched instruments.
- **Hardware envelope** (`hardOnly`, `softToHard`) makes the old bass buzzy and very low, and there is one envelope for the whole chip, so avoid it.
- Note naming: `A4` = 440 Hz (checked by rendering). Arkos note number = octave*12 + semitone (C=0).
- Final volumes (track volume + master 3, capped at 15): lead 15, bass 15 (12+3), harp 15 / 13, shaker 11 / 9. The master offset is `MASTER` in the generator. The music was judged quiet against the SFX in game, so the music was raised, and there is no headroom left. If it needs more, lower the SFX instead: `sfx_play(sfx, channel, inverted_volume)` takes 0 = loudest, and every call passes 0 today.

## Workflow (all native Linux, nothing needs the Arkos GUI)
Tools: `/home/carlucio/Applications/ArkosTracker3/tools/` (`SongToAkm`, `SongToSoundEffects`, `SongToWav`). The GUI is `/home/carlucio/Applications/ArkosTracker3/ArkosTracker3`.

1. Generator: `tools/gen_song.py` builds `song.aks` from the original song plus the new instruments and the compositions. It is a plain Python script (use `python3 -I`). It expects the original as `song_v1.xml` in the current directory. In a scratch directory, get the original from git history:
   `git -C <repo> show 3039941:assets/sfx/song.aks > song_v1.aks`
   `unzip -p song_v1.aks | tr -d '\r' | grep -v '^\s*$' > song_v1.xml`
   Run it there. It writes `song.xml` and `song.aks`; copy `song.aks` to `assets/sfx/`. Edit the compositions or `MASTER` at the bottom of the script.
   Alternatively, edit `song.aks` directly in the Arkos GUI. The script would then no longer match, so prefer one or the other.
2. Export: `SongToAkm -bin -adr 0xD000 song.aks song.akm`. The `-s` option of the tools is 1-based (1=Game ... 4=Hi Score).
3. Audition without an emulator: `SongToWav -s <1..4> song.aks out.wav`.
4. Header: `MSXbin assets/sfx/song.akm -t g_SongData -ad -o song.h`. MSXbin returns a non-zero exit code even on success, so do not chain it with `&&`. Then change the `Soure file:` line to a relative path and move the result to `src/data/song.h`.
5. Update the size in `src/platform/music.c` (two comments) and the song row of the memory map in `CLAUDE.md`. The song region 0xD000..0xDFFF allows 4096 B before the SFX at 0xE000.
6. `./build.sh`, then report ROM and RAM (baselines in CLAUDE.md are stale, see below).

## Next steps
1. **SFX redo** (`effects.aks`, 6 instruments: Shot, Deathray, Enemy explodes, Enemy explodes big, Crash, Start). Same workflow with `SongToSoundEffects -bin -adr 0xE000 effects_v2.aks effects_v2.akx`. Current indices (see `music.h`): 1 torpedo (channel 2), 2 deathray (channel 0), 3 explosion (channel 1), 4 big explosion (channel 1), 5 installation lost (channel 0). Keep the numbering or update `music.h`. Goals: make them fit the new music (key D/E Phrygian, avoid masking the lead on channel 0), punchier torpedo, a clear deathray sweep, layered explosions.
2. **Wave-intensity music** (optional code): switch to a more tense variant on later waves via `music_play` in `src/game/flow.c`. This needs another sub-song or a different loop point.
3. **Hear the title pad** and tune it (it was never heard, only rendered). It may be too quiet.
4. Maybe re-balance bass vs lead after listening in a real emulator with SFX playing.
5. After the SFX are final, remove the unused instruments (1-6, 8, 14) if size matters (it does not much).

## Open points
- CLAUDE.md baselines (14429 B ROM, 464 B RAM) are stale: HEAD before the music work was already 16706 B ROM (earlier HUD and screen commits), RAM unchanged at 464 B. Refresh them when committing.
- Untested in the emulator: loop points, song switches, SFX audibility over the music, 50 Hz timing (the game-over song is cut at 4.5 s).
