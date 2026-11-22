# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

A clone of the game Atlantis for MSX1, written in C (SDCC, Z80) on the MSXgl library.

## Build

- `./build.sh` builds the ROM. It runs MSXgl's Node build tool (`engine/script/js/build.js`) with the Node bundled in MSXgl. It looks for MSXgl at `../MSXgl` by default; set `MSXGL_PATH` to use another location. Extra arguments go to `build.js`.
- Outputs: `emul/rom/atlantis.rom` (32 KB) and the linker map `out/atlantis.map`. Both `out/` and `emul/` are git-ignored.
- There are no tests and no linter. A passing build only shows that the code compiles. Real behaviour has to be checked in an emulator, which the agent cannot run, so after a change list what the developer should check there.
- Never call the system `sdcc` or `node`. The MSXgl tool uses its own embedded SDCC (4.6.0) from `MSXgl/tools/sdcc`. Do not install or replace tools.
- After each build, report ROM usage (the `Seg[0000]: Lower=4000h Higher=…h Size=…` line) and RAM usage (`s__DATA`/`l__DATA` and `s__INITIALIZED`/`l__INITIALIZED` in the map). Warn if either grows. The baseline is 17541 B ROM (0x4000–0x8484) and about 495 B RAM (0xC000–0xC1EF).

## Build configuration

- `project_config.js`: sets the machine (`Machine = "1"`, MSX1), the target (`Target = "ROM_32K"`, a plain 32 KB ROM at 4000h), the MSXgl library modules (`LibModules`) and the project sources (`AddSources`). **Every new `src/*.c` file must be added to `AddSources` by hand.**
- `msxgl_config.h`: the compile-time switches for the MSXgl modules (VDP modes, input devices, Arkos options, the RNG method, and so on).
- MSXgl is compiled from source according to these two files. If a module is not in `LibModules`, or a feature is turned off in `msxgl_config.h`, it does not exist in the build. Check both before you use an API.
- Never give a project source file the same name as an MSXgl module (`input.c`, `system.c`, `vdp.c`, `math.c`, `memory.c`, …). The build writes `out/<basename>.rel` for every source file, so the project object overwrites the library object and the link fails. This is why input lives in `controls.c`.

## Architecture

- `atlantis.c` holds `main()` and the platform layer:
  - Boot sets Screen 2, loads tiles and sprites, turns off the key click, starts the music, and installs the `H_TIMI` hook.
  - `MusicFrameHook` is a `__naked` hook. It increments `volatile u8 g_FrameCounter` and calls `AKM_Update`. It wraps that work in `push af`/`pop af` because the BIOS needs A (a copy of VDP status S#0) preserved. It must not touch the VDP.
  - The game loop runs title → waves → game over → hi-score, forever. Each frame does `handle_input` → `Halt()` (wait for vblank) → `sprites_flush` → map/lane/torpedo updates → collisions.
- Game modules in `src/`:
  - **wave / difficulty**: `wave_start` seeds the RNG with `g_FrameCounter`, builds the gorgon list for the wave with its velocities, and pushes every gorgon into lane 0's queue.
  - **lane**: four lanes, each a state machine (waiting / moving / exploding). A gorgon that leaves the screen is passed to `lane->next` through its `queue_t`. In the last lane (`next == 0`) the gorgon attacks with the deathray. `g_wave_size` counts down as gorgons finish, and the wave ends when it reaches 0.
  - **torpedo / collision / controls**: the player's torpedoes, and collision checks against gorgons and the deathray.
  - **city** (`map_*`): the background, the 7 installations (`map_have_installation[]`), and the water and generator animation driven by `g_FrameCounter`.
  - **hud / score**: the screens and the score. The score and hi-score are ASCII digits kept in a HUD text buffer.
- **Video** (TMS9918A, Screen 2): name-table writes use `TILES_PUT_BUFFER` / `VDP_WriteVRAM_16K` relative to `g_ScreenLayoutLow`. Sprites are 16×16, held in the RAM array `sprites_attributes[17]` and copied to the SAT once per frame right after vblank (`sprites_flush`). The sprite plane assignments are documented in `src/sprites.h`.
- **Sound**: the Arkos AKM player (`arkos/akm_player`). The song and SFX data are position-dependent and are copied from ROM to fixed RAM addresses at boot (`src/music.c`). Sub-songs are 0 = game, 1 = title, 2 = game over, 3 = hi-score. SFX numbers start at 1 (`ARKOS_SFX_START_IDX=1`).
- **Memory map** (nothing may overlap):

  | Region | Address |
  |---|---|
  | Game RAM | 0xC000 and up (must stay below 0xD000) |
  | Song | 0xD000–0xD454 |
  | SFX | 0xE000–0xE11B |
  | Arkos buffer (`ARKOS_BUFFER_ADDR`) | 0xF000–0xF113 |
  | Stack top | HIMEM (normally 0xF380) |

## Generated data (committed, not produced by the build)

`src/tileset.h`, `src/song.h` and `src/effects.h` were made by hand with the MSXtk tools in `../MSXgl/tools/MSXtk/bin/`:

- `MSXimg assets/gfx/tiles_msx1.png -mode gm2 -pal msx1 --tilesUnique -name tileset -out tileset.h`
  - `tiles_msx1.png` is a copy of `tiles.png` with two greens changed to MSXimg's MSX1 palette values (0x66CC66 → 0x3EB849, 0x55AA55 → 0x3AA241). MSXimg's GM2 export ignores custom palettes.
  - The header holds 256 tiles, but only the first 138 are loaded (`TILES_COUNT`).
- `MSXbin assets/sfx/song.akm -t g_SongData -ad -o song.h`
- `MSXbin assets/sfx/effects.akx -t g_SFXData -ad -o effects.h`

The `.akm` and `.akx` files were exported from Arkos Tracker 2 with "Encode to address" set to 0xD000 and 0xE000. If the song data must move, the developer has to re-export it from the Arkos Tracker GUI. Give the developer the format and the address to use, and wait for the new export. The original assets (`tiles.png`, `song.aks`, `effects.aks`) are the source of truth: never overwrite or delete them.

## Reference material (read-only, shared with other projects; never edit)

- MSXgl: `../MSXgl`. For API signatures, read the module headers in `engine/src/*.h`. For usage examples, see `projects/samples/` (for example `s_arkos.c`). Build defaults are in `projects/default_config.js`.
- MSXgl wiki snapshot: `../msxgl-wiki`, one wikitext file per page (for example `Modules__arkos__akm_player.txt`). Search with grep and open only the matching page. If the wiki and the headers disagree, the headers are right.
- MSX and SDCC notes: `../msx-notes/MSX_SDCC_NOTES.md`. Read this before changing hardware-level code (VDP, interrupts, memory map, sound, assembly).
- Never invent an MSXgl API, and never answer MSX hardware or SDCC questions from memory. Confirm the signature in the header first.

## Hard rules for this codebase

- Plain `char` is **unsigned** in this build. Use `i8` for every signed 8-bit value (for example velocities). Watch for integer promotion.
- There is no hardware multiply or divide. Do not use float, long, printf, stdio or malloc in game code. Keep locals small and do not use recursion. Use `const` for tables so they stay in ROM.
- The music player runs inside the vblank hook. Any call into the player from the main loop (play, SFX, stop) must be wrapped in `DisableInterrupt()`/`EnableInterrupt()`, as `music_play` and `sfx_play` already are. Install or replace the hook only with interrupts disabled, and only after the player has been initialised.
- RNG: use `Math_GetRandom16()` / `Math_SetRandomSeed16()`. Do not use `Math_GetRandom8()`: it reads the Z80 R register, so it ignores the seed, and `% 100` on its 0–255 range is biased, which would change the difficulty balance.
- Inline assembly uses sdasz80 syntax: `#` before immediates, `.` before directives, and the index offset before the register (`2(ix)`). SDCC has no clobber lists, so prefer `__naked` functions with an explicit `ret`.
- Respect the MSX1 limits: at most 4 sprites per scanline, one colour per sprite, and 2 colours per 8×1 tile row.
- Match the style of the existing code.
- Never run `git commit` or `git push` unless asked. Stop once the build passes and leave review and commits to the developer.
