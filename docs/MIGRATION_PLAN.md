# Atlantis → MSXgl migration plan

Durable, self-contained plan. A fresh session only needs this file + `AGENTS.md`
+ the MSXgl headers to execute any single step. Update the **Status** section
and the **ROM/RAM** table at the end of every step.

---

## Status

- [x] Step 1 — Scaffold MSXgl project (build.sh, project_config.js, msxgl_config.h, stub atlantis.c)
- [ ] Step 2 — Port display layer (tiles/city/hud/wave/score)
- [ ] Step 3 — Port frame loop + interrupts + tick counter
- [ ] Step 4 — Port sprites
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
| `ubox_set_colors(fg,bg,border)` | `VDP_SetColor2(u8 bg, u8 text)` (`vdp.h:739`) — R#7 packs text+border only; cosmetic |
| `ubox_fill_screen(tile)` | `VDP_FillScreen_GM2(u8 value)` (`vdp.h:1395`) |
| `ubox_set_tiles(tiles)` | `VDP_LoadPattern_GM2(const u8* src, u8 count, u8 offset)` (`vdp.h:1412`), count=0 → 256 |
| `ubox_set_tiles_colors(colors)` | `VDP_LoadColor_GM2(const u8* src, u8 count, u8 offset)` (`vdp.h:1421`) |
| `ubox_write_vm(dst,len,src)` | `VDP_WriteVRAM_16K(const u8* src, u16 dest, u16 count)` (`vdp.h:576`). **Arg order swapped** (src,dest,count) |
| `ubox_put_tile(x,y,tile)` | `VDP_Poke_GM2(u8 x, u8 y, u8 value)` (`vdp.h:1399`) |
| `ubox_wvdp(reg,data)` | `VDP_RegWrite(u8 reg, u8 value)` (`vdp.h:531`); for 16×16 use `VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16)` |
| `ubox_set_sprite_pat16(data,pat)` | `VDP_LoadSpritePattern(const u8* addr, u8 index, u8 count)` (`vdp.h:1116`); bulk: `VDP_WriteVRAM_16K(sprites, 0x3800+384, 544)` |
| `ubox_set_sprite_pat16_flip(data,pat)` | **none** — pre-flip the 3 sprite pairs in assets or write a tiny local flip routine |

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
- Port `tiles.c/h`, `city.c`, `hud.c`, `wave.c`, `score.c` (VDP only) using the
  mapping table above. Keep `TILES_MAKE_ADDRESS`/`TILES_PUT_BUFFER` macros
  (reduce to `VDP_WriteVRAM_16K`).
- Ship tileset via MSXimg (or reuse `src/tileset.h` temporarily).
- Verify: emulator — correct tiles/colours, title screen, HUD, map.

### Step 3 — Loop + interrupts + tick
- Replace `ubox_init_isr`/`ubox_wait_vsync`/`ubox_set_user_isr`/`ubox_tick` with
  `VDP_EnableVBlank(TRUE)`, `volatile u8` frame counter incremented by an
  `H_TIMI` hook, and `Halt()`/vblank-flag waits.
- Verify: emulator — correct frame pacing; water/wave animation timing.

### Step 4 — Sprites
- `ubox_wvdp(1,0xe2)` → `VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16)`.
- `ubox_set_sprite_pat16*` + `ubox_write_vm(0x3800…)` → `VDP_LoadSpritePattern` /
  `VDP_WriteVRAM_16K`.
- SAT flush → `VDP_WriteVRAM_16K(attrs, g_SpriteAttributeLow, 68)`.
- `struct sprite_attr` → `struct VDP_Sprite`. Handle h-flip (pre-flip or local routine).
- Verify: emulator — gorgons, torpedos, deathray, explosion.

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
| 2 | | | |
| 3 | | | |
| 4 | | | |
| 5 | | | |
| 6 | | | |
| 7 | | | |
| 8 | | | |
