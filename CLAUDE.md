# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

A clone of the game Atlantis for MSX1, written in C (SDCC, Z80) on the MSXgl library.

## Build

- `./build.sh` builds the ROM. It runs MSXgl's Node build tool (`engine/script/js/build.js`) with the Node bundled in MSXgl. It looks for MSXgl at `../MSXgl` by default; set `MSXGL_PATH` to use another location. Extra arguments go to `build.js`.
- Outputs: `emul/rom/atlantis.rom` (32 KB) and the linker map `out/atlantis.map`. Both `out/` and `emul/` are git-ignored.
- There are no tests and no linter. A passing build only shows that the code compiles. Real behaviour has to be checked in an emulator, which the agent cannot run, so after a change list what the developer should check there.
- Never call the system `sdcc` or `node`. The MSXgl tool uses its own embedded SDCC (4.6.0) from `MSXgl/tools/sdcc`. Do not install or replace tools.
- After each build, report ROM usage (the `Seg[0000]: Lower=4000h Higher=…h Size=…` line) and RAM usage (`s__DATA`/`l__DATA` and `s__INITIALIZED`/`l__INITIALIZED` in the map). Warn if either grows. The baseline is 23965 B ROM (0x4000–0x9D9C) and 498 B RAM (0xC000–0xC1F2), after commit 26866c1 (title star twinkle).

## Build configuration

- `project_config.js`: sets the machine (`Machine = "1"`, MSX1), the target (`Target = "ROM_32K"`, a plain 32 KB ROM at 4000h), the MSXgl library modules (`LibModules`) and the project sources (`AddSources`). **Every new `src/**/*.c` file must be added to `AddSources` by hand.** `CompileOpt` puts `src/`, `src/platform`, `src/game`, `src/ui` and `src/data` on the include path, so headers are included by bare name (`"city.h"`) and a new source directory needs its own `-I`.
- `msxgl_config.h`: the compile-time switches for the MSXgl modules (VDP modes, input devices, Arkos options, the RNG method, and so on).
- MSXgl is compiled from source according to these two files. If a module is not in `LibModules`, or a feature is turned off in `msxgl_config.h`, it does not exist in the build. Check both before you use an API.
- Never give a project source file the same name as an MSXgl module (`input.c`, `system.c`, `vdp.c`, `math.c`, `memory.c`, …). The build writes `out/<basename>.rel` for every source file, so the project object overwrites the library object and the link fails. This is why input lives in `controls.c`.

## Architecture

- `atlantis.c` holds `main()` only. Boot sets Screen 2, loads tiles and sprites, turns off the key click, starts the music, calls `frame_install_hook()`, then `flow_run()`.
- `frame.c` owns `volatile u8 g_FrameCounter` and `MusicFrameHook`, a `__naked` `H_TIMI` hook. The hook increments the counter and calls `AKM_Update`. It wraps that work in `push af`/`pop af` because the BIOS needs A (a copy of VDP status S#0) preserved. It must not touch the VDP.
- `flow.c` runs title → waves → game over → hi-score, forever. Each in-wave frame does `controls_update` → `Halt()` (wait for vblank) → `sprites_flush` → map/lane/torpedo updates → collisions. P or F1 (`controls_pause_pushed`) pauses an in-wave frame loop (not the banners or other screens) in `flow_pause`: nothing updates, `music_stop`/`music_resume` mute and continue the player, `music_play_pause_chime` writes a short chime straight to the PSG in between (each toggle blocks for 22 frames), and a PAUSED plaque takes the wave banner's place, then `screens_clear_banner` and `sky_refresh` restore the sky. The non-interactive loops (wave banner, game over, hi-score) all go through `flow_play_frames(count, what)`.
- Source layout: `src/platform/` (frame, tiles, sprites, music: the layer that talks to VDP, BIOS and Arkos), `src/game/` (flow, wave, difficulty, lane, queue, gorgons, torpedo, deathray, collision, controls, city, sky, score), `src/ui/` (hud, screens) and `src/data/` (const tables only, no code: `sprite_data`, `city_data`, `sky_data`, `text_data`, the generated `logo_data`, plus the generated `tileset.h`, `song.h`, `effects.h`). Declare each table in its header with an explicit size (`extern const u8 x[32];`) so a wrong initialiser length fails the build and `sizeof` works in other files. `controls.c` lives in `game/` for now: it reads game state (`map_have_installation`, `torpedoes[]`), so it does not fit `platform/` until the keyboard read is split from the firing logic.
- Game modules:
  - **wave / difficulty**: `wave_start` seeds the RNG with `g_FrameCounter`, builds the gorgon list for the wave with its velocities, and pushes every gorgon into lane 0's queue.
  - **lane**: four lanes, each a state machine (waiting / moving / exploding). A gorgon that leaves the screen is passed to `lane->next` through its `queue_t`. In the last lane (`next == 0`) the gorgon attacks with the deathray. `g_wave_size` counts down as gorgons finish, and the wave ends when it reaches 0.
  - **torpedo / collision / controls**: the player's torpedoes (`torpedo_fire(launcher)` reads a const launcher table), and torpedo-vs-gorgon collision. Callers hide the torpedo with `torpedo_hide()` before `lane_explode()`.
  - **deathray**: armed when a gorgon enters the last lane, it follows that gorgon and destroys at most one installation per pass (`deathray_update`, called by the last lane).
  - **sky** (`sky_*`): name-table rows 0..11. The sky cycles with the wave (`wave_sky`: day, evening, night, then day again). `sky_set()` runs between waves with the display off. It recolours the cyan sky pixels of the city, wave and plaque-frame tiles in the top and middle colour banks (`tiles_replace_color`), sets the border (`sky_get_border()`), and draws the rows and decorations. `sky_update()` (in-wave frames only, so the animation never draws over a banner or plaque) moves the day clouds and evening birds 1 px at a time, by rewriting the patterns of reserved tile slots (0xE0..0xF8, above `TILES_COUNT`), and sweeps the night spotlights (4 whole-sky frames of rows 0..11 with the beam tiles and the stars, in `sky_data.c`, each written in one `VDP_WriteVRAM_16K` block, swept 1-2-3-4-3-2, a new one every 40 frames). At night `sky_draw` loads the 66 pool tiles (`sky_beam_tiles`: 64 beam tiles and 2 dim stars) into the top and middle banks at 0x60..0x76 and 0xD5..0xFF, and `sky_update` twinkles the bright stars by cycling the colours of tiles 0x98..0x9A every 8 frames (`sky_twinkle`). The title twinkles its stars the same way in all three banks (`screens_animate_title` calls `sky_set_star_colors`), which also restores them when it is drawn after a night wave. The night sky also has a crescent moon on the horizon. At night the gorgons are all black (`lane.c`), so they show only in the beams. `flow_play_wave` calls `sky_refresh()` after clearing the banner.
  - **city** (`map_*`, `city_*`): the background, the 7 installations (`map_have_installation[]`), the surviving edges (`city_left_edge`/`city_right_edge`, reset by `map_init`), `city_rebuild()`, and the water and generator animation driven by `g_FrameCounter`.
  - **hud / screens / score**: `hud.c` is only the HUD text row. `screens.c` draws the title, wave banner, game-over and hi-score text. The score and hi-score are 6 ASCII digits kept in the HUD text buffer (`hud[9..14]` and `hud[23..28]`), most significant digit first. Scoring follows: `score_add(points)` with fighter/bomber +1 (straight) or +2 (angled), bandit +10/+20, and +5 per standing installation at wave end. A credit (bonus installation) is earned each time the score crosses a multiple of 100, and the score saturates at 999999.
- **Video** (TMS9918A, Screen 2): name-table writes use `TILES_PUT_BUFFER` / `VDP_WriteVRAM_16K` relative to `g_ScreenLayoutLow`. Sprites are 16×16, held in the RAM array `sprites_attributes[17]` and copied to the SAT once per frame right after vblank (`sprites_flush`). The sprite plane assignments are documented in `src/platform/sprites.h`.
- **Tile map** (`tiles.png`, one 8×8 tile per index, 32 per sheet row). The same tileset is loaded into all three screen thirds; the sky code recolours some of it per third.

  | Tiles | Use |
  |---|---|
  | 0x00–0x05, 0x07–0x09, 0x0B–0x0D, 0x0F–0x18, 0x1A–0x1C | blank (magenta) in `tiles.png`. The title loads its logo over them in the top bank, so a tile placed here would show correctly only in rows 8..23 |
  | 0x23 | plain sky (cyan; recoloured per sky, black at night) |
  | 0x60–0x76 | night beam pool: overwritten at night in the top and middle banks by `sky_beam_tiles`; blank (magenta) in `tiles.png`, keep them so |
  | 0x8C–0x8F | day sky gradient |
  | 0x90–0x97 | evening cloud streaks |
  | 0x98–0x9A | stars (title and night) |
  | 0x9B–0xA2 | gold plaque frame; 0xA3–0xA4 HUD ornaments |
  | 0xA5–0xC4 | breaking-wave animation (see `map_water_anim`) |
  | 0xC5–0xCC | evening sky bands; 0xCD–0xD4 setting sun (the last loaded tile, `TILES_COUNT` = 213) |
  | 0xD5–0xDF, 0xF9–0xFF | not loaded; night beam pool (top and middle banks) |
  | 0x06, 0x0A, 0x0E, 0x19 | night crescent moon (top-left, top-right, bottom-left, bottom-right), light yellow on black |
  | 0x7F | free: loaded but never drawn (a leftover). Reuse it (or, for tiles drawn only in rows 8..23, a blank slot of 0x00–0x1C) before growing `TILES_COUNT`; no unloaded slot is free any more |
  | 0xE0–0xF8 | reserved: patterns written at run time by `sky.c` (day clouds, evening birds; part of the night beam pool); keep them blank in `tiles.png` |

  The other indices hold the font (ASCII positions), the city background and the installations. The font (`.`, `0`–`9`, `@` as ©, `A`–`Z`) has one colour per pixel row on black, following the title logo's gradient: white, light yellow ×2, dark yellow, cyan, light blue, dark blue ×2.
- **Title logo**: its own artwork, `assets/gfx/logo.png` (192×56, drawn at tile (4, 1)). `tools/gen_logo.py` cuts it into unique tiles and writes `src/data/logo_data.c`/`.h`; `screens_draw_logo` loads them into the top bank (now 85 tiles in 0x00–0x1C, 0x60–0x76 and 0xD5–0xF5) on every title draw, because the sky code overwrites some of those slots. A top-bank slot may be added to the logo's `SLOT_RANGES` only if no game screen shows it in rows 0..7, or the sky code reloads it before showing it. `title_stars` are written after the logo, so a star must not sit on a logo cell.
- **Sky colours**: in the game screen, cyan (7) means sky. Any tile with sky pixels that is shown in rows 0..15 must be listed in `sky_recolour` (`sky_data.c`), or it keeps a cyan patch in the evening and at night. The night spotlight frames (`sky_beam_sky`, 4 × 32×12 tiles, stars included), `sky_beam_row12` and `sky_beam_tiles` are generated by `tools/gen_beams.py` (the beam wedges, pivots, moon, bright and dim star positions are at its top): run `python3 -I tools/gen_beams.py` and paste its output over the marked block in `sky_data.c`. Do not edit the tables by hand.
- **Sound**: the Arkos AKM player (`arkos/akm_player`). The song and SFX data are position-dependent and are copied from ROM to fixed RAM addresses at boot (`src/platform/music.c`). Sub-songs are 0 = game, 1 = title, 2 = game over, 3 = hi-score. SFX numbers start at 1 (`ARKOS_SFX_START_IDX=1`). How to build, export and audition the song and SFX, the sub-song and SFX tables, and the lessons learned are in `docs/music.md`.
- **Memory map** (nothing may overlap):

  | Region | Address |
  |---|---|
  | Game RAM | 0xC000 and up (must stay below 0xD000) |
  | Song | 0xD000–0xD713 |
  | SFX | 0xE000–0xE11B |
  | Arkos buffer (`ARKOS_BUFFER_ADDR`) | 0xF000–0xF113 |
  | Stack top | HIMEM (normally 0xF380) |

## Generated data (committed, not produced by the build)

`src/data/tileset.h`, `src/data/song.h` and `src/data/effects.h` were made by hand with the MSXtk tools in `../MSXgl/tools/MSXtk/bin/`:

- `MSXimg assets/gfx/tiles.png -mode gm2 -pal msx1 --tilesUnique -name tileset -out tileset.h`, then move the result to `src/data/`
  - Paint `tiles.png` with the colours of MSXimg's MSX1 palette, as the existing tiles do. MSXimg's GM2 export ignores custom palettes.
  - MSXimg exports all 256 tiles as one block. After exporting, set `TILES_COUNT` in `src/platform/tiles.h` to the number of tiles the game uses, then run `python3 tools/trim_tileset.py`. It trims `tileset_Patterns` and `tileset_Colors` in `src/data/tileset.h` to `TILES_COUNT*8` bytes each and fixes the size comments. If you skip the `TILES_COUNT` update, new tiles are not loaded into VRAM.
- `MSXbin assets/sfx/song.akm -t g_SongData -ad -o song.h`
- `MSXbin assets/sfx/effects.akx -t g_SFXData -ad -o effects.h`

`src/data/logo_data.c` and `logo_data.h` are written by `python3 -I tools/gen_logo.py` (run from the repository root, needs Pillow) from `assets/gfx/logo.png`, painted with the same MSX1 palette and at most 2 colours per 8×1 tile row. Do not edit them by hand.

The `.akm` and `.akx` files were exported from Arkos Tracker 2 with "Encode to address" set to 0xD000 and 0xE000. If the song data must move, the developer has to re-export it from the Arkos Tracker GUI. Give the developer the format and the address to use, and wait for the new export. The original assets (`tiles.png`, `logo.png`, `song.aks`, `effects.aks`) are the source of truth: never overwrite or delete them. The exceptions are `tiles.png` and `logo.png`, which are edited in place for graphics changes the developer asked for.

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
- SDCC codegen traps that were found by comparing ROMs: exact-fit string initialisers (`u8 a[3] = "abc";`) are not equivalent to brace lists (they warn and grow the ROM), so keep text as `{ 'a','b','c' }`. Adding a zero constant to an index (`[0 + 2*n]`) changes promotion and codegen. `y >= 192` and `y > 191` compile differently.
- Never run `git commit` or `git push` unless asked. Stop once the build passes and leave review and commits to the developer.
