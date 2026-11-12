# Atlantis → MSXgl migration plan

Durable, self-contained plan. A fresh session only needs this file + `AGENTS.md`
+ the MSXgl headers to execute any single step. Update the **Status** section
and the **ROM/RAM** table at the end of every step.

---

## Status

- [x] Step 1 — Scaffold MSXgl project (build.sh, project_config.js, msxgl_config.h, stub atlantis.c)
- [x] Step 2 — Port display layer (tiles/city/hud/wave/score)
- [x] Step 3 — Port frame loop + interrupts + tick counter
- [x] Step 4a — sprites.c/h (patterns, flip, SAT flush)
- [ ] Step 4b — queue, gorgons, difficulty, lane, wave_start
- [ ] Step 4c — torpedo, collision, score/city integration
- [ ] Step 4d — input stub, wave_finish, full game flow in main
- [ ] Step 5 — Port music + SFX (Arkos AKM)
- [ ] Step 6 — Port input
- [ ] Step 7 — Port RNG (rand/srand)
- [ ] Step 8 — Remove legacy plumbing, finalize build

Legacy `Makefile` must keep working until step 8.

---

## Goal and rules (from AGENTS.md)

- Migrate from ubox to MSXgl, including the whole asset pipeline.
- End state: MSXgl only — no ubox calls/includes/tools in the build.
- Keep gameplay logic unchanged; only change the platform layer.
- Small steps, build after every step. Report ROM/RAM from the linker map each
  step and compare to legacy (~14.5 KB ROM, ~444 B RAM).
- Never `git commit`/`git push`.
- Do not use ubox tools (png2tiles.py, rasm, hex2bin, mplayer) in the final build.

## Confirmed project facts

- Target: **MSX1** (TMS9918A).
- Screen mode: **Screen 2 (Graphic 2), 256×192**.
- ROM: **plain 32 KB, no mapper**, at 4000h, header "AB".
- Legacy ROM usage ~14.5 KB (0x4000–0x7999), padded to 32 KB.
- Legacy RAM: ~444 B game data (0xC0DE–0xC29A) + 222-byte AKM buffer at 0xC000.
- Legacy stack: SP = 0xF380.
- No explicit 50/60 Hz handling; frame-animated via `ubox_tick`.
- Sound: **PSG only**, Arkos Tracker 2 **AKM** player + SFX bank.
- Sprites: **16×16**, pattern table 0x3800, SAT 0x1B00, 17 attributes (68 B), max 32.
- Interrupts: BIOS **HTIMI hook**; user ISR = `mplayer_play`.

## Key decisions (settled)

- **RAM: 32 KB** assumed.
- **Song lives in RAM** at `0xD000` (copied at boot from ROM), not pinned in ROM.
- **SFX lives in RAM** at `0xE000` (copied at boot from ROM).
- **Priority SFX dropped**: `mplayer_play_effect_p` → plain `AKM_PlaySFX`
  (MSXgl AKM has no priority variant; effects always replace).
- Arkos player buffer at default `ARKOS_BUFFER_ADDR = 0xF000`.
- `ARKOS_SFX_START_IDX = 1` (game uses effect numbers 1–5).
- Target: `Target = "ROM_32K"`, `Machine = "1"`.
- Music update: from the main loop after vblank wait (matches `s_arkos.c`),
  OR from the H_TIMI hook. Prefer main-loop for simplicity; revisit if needed.

## Target memory map (MSXgl ROM_32K)

| Region | Address |
|---|---|
| Code (page 1) | 0x4000 … |
| Data (RAM, page 3) | 0xC000 … (must stay below 0xD000) |
| Song | 0xD000 (1108 B → 0xD454) |
| SFX | 0xE000 (283 B → 0xE11B) |
| Arkos buffer (`ARKOS_BUFFER_ADDR`) | 0xF000 (275 B) |
| Stack (HIMEM) | ~0xF380 |

---

## ubox → MSXgl API mapping (authoritative)

### Includes
| ubox | MSXgl |
|---|---|
| `<ubox.h>` | `"msxgl.h"` |
| `<mplayer.h>` | `"arkos/akm_player.h"` |
| `<stdlib.h>` (rand/srand) | `"math.h"` (or keep SDCC rand) |

### VDP / screen
| ubox | MSXgl (exact signature) |
|---|---|
| `ubox_set_mode(2)` | `void VDP_SetMode(const u8 mode);` — pass `VDP_MODE_SCREEN2` (`vdp.h:476`, enum `vdp.h:263`) |
| `ubox_disable_screen()` | `VDP_EnableDisplay(FALSE)` (`vdp.h:710`) |
| `ubox_enable_screen()` | `VDP_EnableDisplay(TRUE)` (`vdp.h:710`) |
| `ubox_set_colors(fg,bg,border)` | `VDP_SetColor2(border, fg)` (`vdp.h:739`, declared as `VDP_SetColor2(u8 bg, u8 text)`): R#7 low nibble = border, high nibble = text (unused in Screen 2). The legacy `bg` argument is unused. NOT cosmetic: map and wave use border 7 (cyan), the explosion flash cycles it. |
| `ubox_fill_screen(tile)` | `VDP_FillScreen_GM2(u8 value)` (`vdp.h:1395`) |
| `ubox_set_tiles(tiles)` | `VDP_LoadPattern_GM2(const u8* src, u8 count, u8 offset)` (`vdp.h:1412`), count=0 → 256 |
| `ubox_set_tiles_colors(colors)` | `VDP_LoadColor_GM2(const u8* src, u8 count, u8 offset)` (`vdp.h:1421`) |
| `ubox_write_vm(dst,len,src)` | `VDP_WriteVRAM_16K(const u8* src, u16 dest, u16 count)` (`vdp.h:576`). **Arg order swapped** (src,dest,count) |
| `ubox_put_tile(x,y,tile)` | `VDP_Poke_GM2(u8 x, u8 y, u8 value)` (`vdp.h:1399`) |
| `ubox_wvdp(reg,data)` | `VDP_RegWrite(u8 reg, u8 value)` (`vdp.h:531`). R#1 = 0xE2 → `VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16)`; R#1 = 0xE3 (x2 magnification) → `VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16 \| VDP_SPRITE_SCALE_2)` (confirm names in vdp.h). It preserves the display and interrupt bits. |
| `ubox_set_sprite_pat16(data,pat)` | `VDP_LoadSpritePattern(data, pat*4, 4)` (`vdp.h:1116`): index and count are in 8x8 units (a 16x16 sprite is 4 units). Bulk: `VDP_WriteVRAM_16K(sprites, g_SpritePatternLow + 384, 544)`. |
| `ubox_set_sprite_pat16_flip(data,pat)` | **none**: 6 patterns (indices 2, 3, 6, 7, 10, 11). Use pre-flipped const arrays (bit-reverse every byte, swap the two 16-byte halves), see "Step 4 decisions". |

### Interrupt / loop
| ubox | MSXgl |
|---|---|
| `ubox_init_isr(wait_ticks)` | `VDP_EnableVBlank(TRUE)` (`vdp.h:717`) + `BIOS_SetHookCallback(H_TIMI, cb)` |
| `ubox_set_user_isr(fn)` | `BIOS_SetHookCallback(H_TIMI, cb)` (`bios_hook.h:35`, `H_TIMI`=0xFD9F `bios_hook.h:117`) |
| `ubox_wait_vsync()` (macro `halt`) | `Halt()` (`system.h:145`), or spin on a `volatile` vblank flag set by the hook |
| `ubox_tick` (global) | **none** — add own `volatile u8` incremented in the H_TIMI hook |
| `ubox_reset_tick()` | zero own counter |

### Input (cursor keys + space)
| ubox | MSXgl |
|---|---|
| `ubox_read_ctl(UBOX_MSX_CTL_CURSOR)` | `Keyboard_Read(8)` (`keyboard.h:155`) or `Keyboard_ReadAsJoystick()` (`keyboard.h:220`, needs `INPUT_KB_AS_JOYSTICK=TRUE`). **Polarity inverted**: ubox active-high, MSXgl active-low (bit=0 = pressed). Bit positions match (UP=1,DOWN=2,LEFT=4,RIGHT=8,TRIG_A=16). |
| `UBOX_MSX_CTL_FIRE1` (16) / `LEFT` (4) / `RIGHT` (8) | `JOY_INPUT_TRIGGER_A` / `JOY_INPUT_DIR_LEFT` / `JOY_INPUT_DIR_RIGHT` (`joystick.h`), or `KEY_SPACE`/`KEY_LEFT`/`KEY_RIGHT` (`keyboard.h`) |

### Sprite attribute struct
- `struct sprite_attr { uint8_t y; x; pattern; attr; }` → `struct VDP_Sprite { u8 Y; u8 X; u8 Pattern; u8 Color; }` (`vdp.h:113`). Field order identical.

### mplayer → Arkos AKM
| ubox mplayer | MSXgl `arkos/akm_player.h` |
|---|---|
| `mplayer_init(song, sub)` | `void AKM_Play(u8 sng, const void* data);` (`:34`). **Arg order swapped** |
| `mplayer_init_effects(effects)` | `u8 AKM_InitSFX(const void* data);` (`:63`) |
| `mplayer_play()` | `bool AKM_Update();` (`:52`) |
| `mplayer_play_effect(no,chan,vol)` | `void AKM_PlaySFX(u8 sfx, u8 chan, u8 vol);` (`:73`) |
| `mplayer_play_effect_p(no,chan,vol)` | **none** → `AKM_PlaySFX` (priority dropped) |

### RNG
- `srand(seed)` → `Math_SetRandomSeed16(u16 seed)` (`math.h:377`).
- `rand()` → `Math_GetRandom8()` (`math.h:324`), `Math_GetRandomMax8(max)` (`math.h:334`).
  (Or keep SDCC `rand()` if it links.)

---

## Asset pipeline

### Graphics
- Source: `assets/gfx/tiles.png` (256 tiles, Screen 2).
- Legacy: `png2tiles.py` → `src/tileset.h` (hand-committed; will be regenerated).
- MSXgl: `MSXimg tiles.png -mode gm2 -pal msx1 -name tileset -out <out>.h` → pattern + color tables
  (Screen 2 = `-mode gm2 -pal msx1`, see `MSXimg.txt`).
- **Verify colours in emulator** — Screen 2 has 2 colours per 8×1 row; a
  conversion can "succeed but look wrong".

### Music / SFX (already exported and verified — DO NOT re-export)
- `assets/sfx/song.akm` — AKM binary, **"Encode to address" 0xD000**, 1108 B, 4 sub-songs.
- `assets/sfx/effects.akx` — AKX binary, **"Encode to address" 0xE000**, 283 B, 7 SFX (game uses 1–5).
- Convert with MSXbin (MSXtk):
  - `MSXbin song.akm -t g_SongData -ad -o song.h` → `const u8 g_SongData[]`.
  - `MSXbin effects.akx -t g_SFXData -ad -o effects.h` → `const u8 g_SFXData[]`.
- At boot: `Mem_Copy(g_SongData, (void*)0xD000, sizeof(g_SongData))` and
  `Mem_Copy(g_SFXData, (void*)0xE000, sizeof(g_SFXData))`, then
  `AKM_Play(sub, (void*)0xD000)` / `AKM_InitSFX((void*)0xE000)`.

---

## Architectural differences to handle

1. **crt0**: do NOT reuse `src/crt0.z80`. MSXgl `ROM_32K` provides crt0 (header,
   slot backup, `SP = HIMEM`, heap/globals init, `call main`).
2. **Interrupts**: BIOS ISR kept; use `BIOS_SetHookCallback(H_TIMI, …)`. Hook must
   be short + `volatile`-safe.
3. **VRAM**: Screen 2 VRAM layout is identical (pattern 0x0000, name 0x1800,
   colour 0x2000, SAT 0x1B00, sprite pattern 0x3800). Prefer `g_ScreenLayoutLow`
   / `g_SpriteAttributeLow` getters + `VDP_*` functions over raw addresses.
4. **Memory**: data at 0xC000, song 0xD000, SFX 0xE000, Arkos 0xF000, stack 0xF380.
   Re-check data does not grow past 0xD000 after each build.
5. **Toolchain**: legacy SDCC 4.0.0 (stack call conv) vs MSXgl embedded SDCC
   **4.6.0** (register call conv `__sdcccall(1)`). Re-check any hand asm. The only
   asm in the game is `ubox_wait_vsync`'s `halt` → `Halt()`.
6. **50/60 Hz**: legacy does no compensation; preserve (song authored 50 Hz plays
   ~20% fast at 60 Hz — same behaviour as legacy).

---

## The 8 steps

Each step is self-contained and ends with a passing build + ROM/RAM report +
emulator checklist. Update **Status** and **ROM/RAM** after each.

### Step 1 — Scaffold
- Create `build.sh` (node `MSXgl/engine/script/js/build.js $@`), `project_config.js`
  (`Machine="1"`, `Target="ROM_32K"`, `ProjName="atlantis"`, minimal `LibModules`),
  `msxgl_config.h` (MSX1, `VDP_USE_MODE_G2`, `VDP_USE_SPRITE`, `VDP_USE_16X16_SPRITE`),
  and stub `atlantis.c` (`main()` sets Screen 2 + halts).
- Build: `./build.sh`.
- Verify: toolchain produces a 32 KB ROM.

### Step 2 — Display layer
- Port the display code of `tiles.c/h`, `city.c`, `hud.c`, `wave.c`, `score.c` using the
  mapping table above. Keep `TILES_MAKE_ADDRESS`/`TILES_PUT_BUFFER` macros
  (reduce to `VDP_WriteVRAM_16K`).
- Tileset: MSXimg on the recolored copy, see "Step 2 decisions".
- Border color: `hud.c:73` (border 1) and `city.c:75` (border 7) are ported in this step.
  `lane.c:113` (wave, border 7) and `lane.c:126` (explosion flash) belong to `lane.c`,
  which is not ported here. Provide a helper `set_border(u8 border)` in the display layer,
  implemented as `VDP_SetColor2(border, 1)`, for `lane.c` to call when it is ported.
- NOT in this step (leave for later, do not invent stand-ins):
  - Sprite attribute table writes at 1B00h: `hud.c:38,57,66`, `wave.c:44`, `main.c:58` → Step 4.
  - R#1 writes (`ubox_wvdp(1, ...)`): `sprites.c:175`, `lane.c:112,146` → Step 4
    (`lane.c:146` writes 0xE3: bit 0 is sprite magnification x2).
  - Frame pacing, tick counter, music, input, RNG → Steps 3, 5, 6, 7.
- Verify: emulator — correct tiles/colours, title screen, HUD, map. Border: black on the
  title/HUD, cyan on the map.

### Step 2 decisions
- Colors: `ubox_set_colors(fg, bg, border)` → `VDP_SetColor2(border, fg)`. R#7 low nibble is the border color (the high nibble is the text color, unused in graphic modes).
  Title/HUD: `VDP_SetColor2(1, 1)`. Map and wave: `VDP_SetColor2(7, 1)` (cyan border). Explosion flash: `VDP_SetColor2(1 + counter % 15, 1)`.
  The `ubox_set_colors` row of the API mapping table above already carries this correction.
- Graphics: MSXimg `-mode gm2 -pal msx1 --tilesUnique` on a recolored copy of the tileset (assets/gfx/tiles_msx1.png), which gives 256 tiles in source order, no names table.
  The copy differs from assets/gfx/tiles.png only in 2 colors: 0x66CC66 → 0x3EB849 and 0x55AA55 → 0x3AA241 (MSXimg's GM2 exporter ignores -pal input/custom and -trans, so the PNG must use the MSXimg MSX1 palette RGB values).
  The original tiles.png stays untouched. Decoded per-pixel output matches the legacy tileset on 138/138 tiles.
  Exact commands (executed in Step 2):
  - Recolor: `0x66CC66 → 0x3EB849` (112 px), `0x55AA55 → 0x3AA241` (801 px) in `assets/gfx/tiles_msx1.png` (copy of `assets/gfx/tiles.png`).
  - `MSXimg assets/gfx/tiles_msx1.png -mode gm2 -pal msx1 --tilesUnique -name tileset -out tileset.h` → `tileset_Patterns[]` (2048 B) + `tileset_Colors[]` (2048 B), no names table. Verified 138/138 per-pixel match against `src/tileset.h`.
- Load with `VDP_LoadPattern_GM2(..., 138, 0)` and `VDP_LoadColor_GM2(..., 138, 0)` (all three banks).
- Highest tile index used: 137 (0x89).
- Verify in the emulator against the legacy ROM, side by side: tiles, greens, border color on title/HUD (black), map/wave (cyan) and the explosion flash.

### Step 3 — Loop + interrupts + tick
- Replace `ubox_init_isr`/`ubox_wait_vsync`/`ubox_tick` with `VDP_EnableVBlank(TRUE)`,
  a `volatile u8` frame counter incremented by an `H_TIMI` hook, and `Halt()` waits.
  See "Step 3 decisions". (`ubox_set_user_isr` is the music player: Step 5.)
- Verify: emulator — correct frame pacing; water/wave animation timing.
  The 480/270/120-frame waits and the title blink are not reachable from the stub main. Both libraries wait with one `halt` per frame, so their duration matches by construction; observe them when the full game flow runs. Real timing risk: frames whose logic takes longer than one vblank (check the busiest moments side by side with the legacy ROM once the game logic is ported).
- Open question: does `VDP_WriteVRAM_16K` / `VDP_Poke_GM2` (and the legacy `ubox_write_vm` / `ubox_put_tile`) protect the two-byte VRAM address setup against the vblank interrupt (which reads VDP status and may reset the control-port latch)?
  - Answered: `VDP_WriteVRAM_16K` / `VDP_Poke_GM2` are DI/EI-protected (see "Step 3 decisions"); the legacy functions were not.

### Step 3 decisions
(From the read-only analysis of Step 3.)
- `ubox_init_isr(wait_ticks)`: Atlantis passes 1, and the argument only affects `ubox_wait()`, which the game never calls. Nothing to port; do not emulate it.
- Frame counter: `volatile u8 g_FrameCounter` (8 bits, wraps every 256 frames, like `ubox_tick`), incremented once per vblank by an `H_TIMI` hook installed with `BIOS_SetHookCallback(H_TIMI, cb)` after `VDP_EnableVBlank(TRUE)`. Zero it at boot (legacy `ubox_reset_tick()`, `main.c:23`). Expose it with `extern` for later steps (`wave.c:58` RNG seed, `lane.c:84` deathray animation).
- Animation: `four_frames_count = (g_FrameCounter % 16) >> 2; two_frames_count = four_frames_count >> 1;` (legacy `city.c:125`).
- Frame waits: `ubox_wait_vsync()` is a plain `halt` (one vblank per call). Use `Halt()` once per loop iteration. Counts: 480 (`hud.c`, hi-score screen), 270 (`hud.c`, game over screen), 120 (`wave.c`, wave HUD). The title blink is 10 x (7 + 7) = 140 vblanks. At 60 Hz: 8.0 / 4.5 / 2.0 / 2.33 s. At 50 Hz: 9.6 / 5.4 / 2.4 / 2.8 s. No 50/60 Hz compensation, as in the legacy.
- The hook only increments the counter. It must not touch the VDP and must not call the music player in this step (the music placement is decided in Step 5).
- VRAM writes: `VDP_WriteVRAM_16K` and `VDP_Poke_GM2` protect the two-byte address setup with DI/EI (`VDP_ISR_SAFE_DEFAULT`, `msxgl_config.h:112`). Use only MSXgl VDP functions for VRAM access, never raw port writes. (The legacy `ubox_write_vm` / `ubox_put_tile` were not protected.)
- The BIOS interrupt handler keeps running (keyboard scan, etc.). `BIOS_SetHookCallback` replaces only the empty `H_TIMI` hook, so there is nothing to chain.
- `ubox_set_user_isr(mplayer_play)` is the music player, so it belongs to Step 5, not here.
- Hook installation: `BIOS_SetHookCallback` writes 4 bytes into the hook (`RST #30`, slot, 16-bit callback address) and does no interrupt protection (confirmed in Step 3). Install it with interrupts disabled (`DisableInterrupt()` / `EnableInterrupt()`) so a vblank cannot run a half-written hook.
- `Halt()` needs interrupts enabled: make sure interrupts are enabled (`EI`) before the first `Halt()`. A frozen screen right after boot usually means this.
- Markers that mix steps: `srand(ubox_tick)` (`wave.c:45`) is RNG: leave it for Step 7 and rewrite the marker as `TODO(step 7)` (the frame counter only has to exist). In the title screen (`hud.c:71-73`, `hud.c:87-91`), port the blink timing (10 cycles of 7 frames off / 7 frames on) and leave the fire-button read (`wait_fire_button`) as `TODO(step 6)`.

### Step 4 — Sprites (split in 4a-4d)
- 4a: port `sprites.c/h` (pattern arrays, `sprites[]`, animation tables, `sprites_attributes[17]`,
  `init_sprites`, `sprites_flush`, deathray helpers). Visible: a temporary test in `main` shows
  every pattern, especially the flipped gorgons and the 2-color overlays.
- 4b: port `queue`, `gorgons`, `difficulty`, `lane` and finish `wave_start`. Stub
  `collision_handle_deathray()` as a no-op. Music calls stay commented (TODO(step 5)).
  Visible: gorgons cross the lanes, the deathray animates, the bandit big explosion (x2
  magnification and border flash).
- 4c: port `torpedo` and `collision`, the `wave.c:77` installation rebuild, and wire score/city.
  Temporary auto-fire stand-in (TODO(step 6)). Visible: torpedoes, hits, score, destroyed
  installations, building explosions.
- 4d: `input.c` as a stub (the title auto-advances after the blink, torpedoes auto-fire on a
  timer, TODO(step 6)); port `wave_finish`; rebuild the full game flow in `main`.
  Visible: title, wave HUD, gameplay, game over, hi-score screen, end to end.
- Final verify (after 4d): emulator, side by side with the legacy ROM: gorgons, torpedoes,
  deathray, explosions.

### Step 4 decisions
(From the read-only analysis of Step 4.)
- Sprite list terminator: this is MSX1 (TMS9918A, sprite mode 1), where the terminator Y value is
  208 (0xD0). 216 (0xD8) is the sprite mode 2 terminator (MSX2 screens 4+, as in s_gm3.c).
  Check s_sm1.c for the sprite mode 1 convention. A terminator is not needed now:
  `VDP_ClearVRAM()` at boot zeroes the SAT, and planes 17-31 have the lowest priority.
- SAT: 17 sprites x 4 bytes = 68 bytes, flushed with
  `VDP_WriteVRAM_16K((u8*)sprites_attributes, g_SpriteAttributeLow, 68)`. Wrap it in a
  `sprites_flush()` helper and call it wherever the legacy flushes the SAT (main loop, the hud
  waits, the wave HUD). `struct VDP_Sprite` has the same fields as `struct sprite_attr`
  ({Y, X, Pattern, Color}); the early clock bit is never used.
- Keep the plane order (gorgons 0-7, torpedoes 8-9, deathray 10-14, explosion 15-16): the
  4-sprites-per-scanline limit matches the legacy only if the order is kept.
- Patterns: `ubox_set_sprite_pat16(data, pat)` → `VDP_LoadSpritePattern(data, pat*4, 4)`;
  `ubox_write_vm(0x3800+384, 544, sprites)` → `VDP_WriteVRAM_16K(sprites, g_SpritePatternLow + 384, 544)`.
  16x16 indices and counts are in 8x8 units (multiples of 4). About 29 patterns in total
  (12 loaded individually, 17 in bulk): confirm the exact count in `sprites.c`.
- Flipped patterns (6): `figther1`→2, `figther2`→3, `bomber1`→6, `bomber2`→7, `bandit1`→10,
  `bandit2`→11 (used by the three LEFT gorgons). Legacy flip = bit-reverse every byte and swap the
  two 16-byte halves. Use pre-flipped const arrays generated by a throwaway script in /tmp and
  verified against an independent implementation of that algorithm. Do not hand-type the bytes.
- R#1: call `VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16)` in `init_sprites` (`VDP_SetModeGraphic2` does
  not set the 16x16 bit). Magnification x2 is global (R#1 bit 0), so the bandit big explosion
  magnifies every sprite on screen: `VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16 | VDP_SPRITE_SCALE_2)`,
  and restore without SCALE_2 (confirm the constant names in vdp.h). It preserves the display and
  interrupt bits.
- Torpedo naming (not a bug): `TORPEDO_LEFT_PATT` / `TORPEDO_RIGHT_PATT` name the LAUNCHER side, while
  the comments in the `sprites[]` data name the pointing direction. `TORPEDO_LEFT_PATT` (13*4) is the
  torpedo launched from the left launcher, drawn pointing right (`torpedo_right`, 16x16 pattern 13).
  Copy verbatim; do not rename or "fix" anything.
- Stand-ins (temporary): every one is marked `TODO(step N)` in code, listed in Findings, and
  removed in step N. Music calls are commented out (Step 5); `handle_input` / `wait_fire_button`
  are stubs (Step 6). RNG: keep the SDCC `rand()` / `srand()` as the legacy does (confirm it
  links); Step 7 decides whether to switch.
- The temporary sprite test in `main` (4a) must be removed when 4b starts.

### Step 5 — Music + SFX (Arkos AKM)
- Add `"arkos/akm_player"` to `LibModules`; set `ARKOS_BUFFER_ADDR=0xF000`,
  `ARKOS_SFX_START_IDX=1` in `msxgl_config.h`.
- MSXbin the two assets → headers; `Mem_Copy` to 0xD000 / 0xE000 at boot.
- Map mplayer calls per table. Decide ISR vs main-loop update (prefer main-loop).
- Verify: emulator — music, sub-song switches (title/gameplay/gameover/hiscore),
  all 5 SFX.

### Step 6 — Input
- `ubox_read_ctl(UBOX_MSX_CTL_CURSOR)` → `Keyboard_Read(8)` (or
  `Keyboard_ReadAsJoystick`). **Invert the polarity logic.**
- Verify: emulator — cursor move + fire (space).

### Step 7 — RNG
- `srand`/`rand` → `Math_SetRandomSeed16` / `Math_GetRandom8` / `Math_GetRandomMax8`.
- Verify: emulator — wave generation still behaves.

### Step 8 — Remove legacy plumbing, finalize
- Delete `src/crt0.z80`, `src/akm.z80`, `src/song.asm`, `src/effects.asm`,
  `src/*_playerconfig.asm`; remove png2tiles/rasm/disark/hex2bin/mplayer from build.
- Add MSXimg/MSXbin as `PreBuildScript`s. Make `build.sh` canonical.
- Verify: `grep -ri "ubox\|mplayer" src/` → zero; ROM/RAM vs legacy; full playthrough.

---

## ROM/RAM log

| Step | ROM used | RAM used | Notes |
|---|---|---|---|
| legacy | ~14.5 KB (0x4000–0x799B) | ~444 B + 222 B AKM buffer | baseline |
| 1 | 1402 B (0x4000–0x4579), padded to 32 KB | 65 B (0xC000–0xC040) | scaffold only: stub `main` + MSXgl `system`/`bios`/`vdp` |
| 2 | 8168 B (0x4000–0x5FE7), padded to 32 KB | 212 B (0xC000–0xC0D3) | display layer: tileset (256 tiles = 4096 B) + tiles/city/hud/wave/score + MSXgl vdp |
| 3 | 8959 B (0x4000–0x62FE), padded to 32 KB | 213 B (0xC000–0xC0D4) | frame loop + H_TIMI hook + `g_FrameCounter` (+1 B); water/generator animation live |
| 4a | 10366 B (0x4000–0x687D), padded to 32 KB | 281 B (0xC000–0xC118) | sprites.c/h: 29 patterns (938 B sprite data in ROM) + `sprites_attributes[17]` (68 B RAM); temp sprite test in atlantis.c |
| 4b | | | |
| 4c | | | |
| 4d | | | |
| 5 | | | |
| 6 | | | |
| 7 | | | |
| 8 | | | |

---

## Findings (learned during execution, affects later steps)

- **Tileset is 256 tiles in ROM but only 138 are loaded.** `--tilesUnique` emits all 256
  tiles (4096 B = 2048 pattern + 2048 colour), of which the game only references
  0..137 (`TILES_COUNT`). The blank tail (138..255, ~1888 B) is dead weight. If ROM
  size ever matters, trim `tileset.h` to the first 138 tiles (or drop `--tilesUnique`
  and remap all tile indices). Do not bother now — ROM is still far below 16 KB.
- **`map_installations_vaddress` type changed** from `const uint8_t*` (absolute `0x1800+…`
  addresses) to `const u16` (name-table offsets, added to `g_ScreenLayoutLow` at the
  use site). Callers are `map_create_instalation` / `map_destroy_instalation` only.
- **`set_border(u8 border)`** lives in `tiles.c` (`VDP_SetColor2(border, 1)`). Step 4
  (`lane.c`) must call it for the wave border (7) and the explosion flash
  (`1 + counter % 15`), instead of `ubox_set_colors`.
- **`draw_title_screen()` is now public** (`hud.h`), it was file-local in the legacy.
  `hud_show_title_screen()` ported the blink loop in Step 3 and keeps a TODO only for
  `wait_fire_button()` (Step 6).
- **`map_update()` water/generator animation**: stubbed in Step 2, resolved in Step 3
  (`g_FrameCounter`).
- **`VDP_SetColor2(bg, text)` argument order confirmed** (bg = border/backdrop = R#7
  low nibble, text = R#7 high nibble, unused in G2). Samples `s_gm3.c:117/125/153` and
  `s_psg.c:331` confirm it. The border is the *low* nibble.
- **`BIOS_SetHookCallback`/`BIOS_SetHookInterSlotCallback` do NOT DI/EI** (confirmed
  `bios.c:83-87`, `bios_hook.h:56-61`): the interslot hook write is `0xF7` (RST #30) +
  slot + 16-bit `cb` = **4 bytes** (not 5 as guessed in the Step 3 decisions), with no
  interrupt protection. `atlantis.c` installs the hook wrapped in `DisableInterrupt()`/
  `EnableInterrupt()` so a vblank can't run a half-written hook.
- **`g_FrameCounter`** (`volatile u8`, 8-bit, wraps every 256 frames like `ubox_tick`) is
  defined in `atlantis.c` and declared `extern volatile u8` in `city.h` (its primary
  consumer is `map_update()`). Step 4 (`lane.c` deathray) and Step 7 (`wave.c` RNG seed)
  read it through `city.h`.
- **The explosion branch in `map_update()` is entirely Step 4.** The legacy's local
  `g_buildings_explosion_frame = map_explosion_counter >> 2` was folded into the Step 4
  marker because the whole `MAP_STATE_EXPLODING` branch only writes sprite attributes
  (`sprites_attributes[15]/[16]`); it does not use `g_FrameCounter` (it uses
  `map_explosion_counter`, incremented per-frame inside that Step 4 branch).
- **Step 4a — sprite pattern count is 29.** 12 16×16 gorgon patterns loaded
  individually (`VDP_LoadSpritePattern(p, p*4, 4)`), 17 loaded in bulk
  (`VDP_WriteVRAM_16K(sprites, g_SpritePatternLow + 384, 544)`: torpedoes 12-14,
  explosion frames 15-24, deathray slots 25-28). `VDP_LoadSpritePattern` `index`/`count`
  are in 8×8 units (×8 bytes), so a 16×16 sprite is `index = p*4`, `count = 4`.
- **Step 4a — flip algorithm verified.** The 6 pre-flipped arrays
  (`*_flip` in sprites.c) were generated by a throwaway script in /tmp/opencode and
  cross-checked: an instruction-by-instruction simulation of
  `ubox_set_sprite_pat16_flip.z80` (rlca/rrc/xor/and) matches the classic bit-reversal
  on all 256 byte values (0 disagreements), and `flip(flip(x)) == x` holds for all 6.
  The flip is "bit-reverse every byte, swap the two 16-byte halves".
- **Step 4a — sprite size/scale flag names confirmed** in `vdp.h:1084-1089`:
  `VDP_SPRITE_SIZE_8 = 0`, `VDP_SPRITE_SIZE_16 = R01_ST`, `VDP_SPRITE_SCALE_1 = 0`,
  `VDP_SPRITE_SCALE_2 = R01_MAG`; `VDP_SetSpriteFlag(u8 flag)` (`vdp.h:1099`) masks only
  `R01_ST|R01_MAG`, so it preserves BL/IE0/M1/M2/K16. `init_sprites()` calls
  `VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16)` (the legacy `ubox_wvdp(1, 0xe2)`); `VDP_SetModeGraphic2`
  does not set the 16×16 bit. s_sm1.c (sprite mode 1) hides sprites by clearing VRAM
  (`VDP_FillVRAM_16K(0, 0x0000, 0x4000)`) rather than a terminator — confirming no
  terminator is needed here (atlantis.c already calls `VDP_ClearVRAM()`).
- **Step 4a — temporary test `TestSpritesStep4a()` in atlantis.c** shows all 29 patterns
  (8 rows, ≤4 sprites/row, rows 24 px apart), each flipped gorgon beside its unflipped
  sibling and 2-colour overlays stacked. Every line is marked `TODO(step 4b: remove)`;
  delete the whole function and its call when 4b starts.
- **Step 4a — `sprites_flush()`** wraps `VDP_WriteVRAM_16K((u8*)sprites_attributes,
  g_SpriteAttributeLow, 68)`. 4b-4d call it wherever the legacy flushed the SAT
  (`hud.c` waits, `wave.c` wave HUD, the main loop).
