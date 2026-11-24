# Atlantis (MSX1 / MSXgl) — modularization review and refactor plan

## Context

The developer wants to clean up and modularize the codebase before it grows, without breaking MSX1 limits. The game works. The code is small (about 1,600 hand-written lines in 15 `.c` files) and already fairly tidy. The real problems are **ownership** (state that lives in the wrong module) and **duplicated frame loops**, not missing abstractions. Measured from the current `out/*.rel` and `out/atlantis.map`:

| Object | ROM (B) | Note |
|---|---|---|
| tiles.rel | 4159 | full 256-tile tileset linked; only 138 used → **1888 B dead** |
| akm_player | 1917 | library |
| music.rel | 1454 | 1391 B song+SFX data (copied to RAM at boot) |
| vdp.rel | 1310 | library, several unused modes enabled |
| sprites.rel | 1220 | ~950 B pattern data + deathray code |
| hud.rel | 1112 | **local `const u8 x[]` arrays get built on the stack byte by byte** (4 B of code per char, confirmed in `out/hud.asm`) |
| lane / city / collision / wave | 1076 / 1053 / 606 / 579 | game logic |
| **Total** | ≈17.0 KB of 32 KB | RAM 496 B (0xC000–0xC1F0) of the 4 KB below 0xD000 |

**Conclusion on banking:** it is not needed. There is about 15 KB of headroom, and Phase 1 frees about 2.2 KB more. The plan only makes the code *banking-ready* (data split from code). It does not switch to a mapper.

---

## 1. Diagnosis

**Good, keep:**
- One SAT shadow (`sprites_attributes[17]`) flushed once right after vblank. This is the best MSX1 pattern.
- The minimal `__naked` H_TIMI hook. Arkos data lives in RAM, so the ISR never depends on ROM banking.
- Score kept as ASCII inside the HUD buffer. Drawing it needs no conversion.
- Fixed lane and torpedo indices with unrolled calls (`lane_update(&g_lanes[0])…`). This gives cheap absolute addressing.
- No circular includes. Every header includes only `msxgl.h` (plus `queue.h` / `hud.h` / `tiles.h` where needed).

**Problems:**
1. **Misplaced ownership (the main coupling problem)**
   - City state (`city_left_edge/right_edge`) is defined in `collision.c` and declared in `collision.h`. `wave.c` edits it to rebuild the city.
   - `g_FrameCounter` is declared in `city.h` but defined in `atlantis.c`.
   - Deathray logic is spread over 4 modules: `g_is_attacking` in lane.c, sprite code in sprites.c, the hit test in collision.c, and the reset in `map_destroy_installation` (city.c → lane global).
   - `lane_start_state_exploding` reaches into `torpedoes[]` to hide the torpedo.
2. **Frame loop duplicated 4×.** `Halt(); sprites_flush(); map_update(); …` appears in main, `wave_draw_hud`, `hud_draw_game_over` and `hud_draw_hi_score`. So "hud" depends on lane, torpedo and collision.
3. **Mixed responsibilities.**
   - `hud.c` = HUD buffer + title, game-over and hi-score screens, including a gameplay loop.
   - `wave.c` = wave generation + banner + end-of-wave bonus + city rebuild + wave counter.
   - `atlantis.c` = platform (hook, frame counter) + the state flow.
4. **Over-exported internals.** Examples: `lane_execute_state_*`, `lane_init`, `torpedo_init`, `map_state`, `map_explosion_counter`, all the `map_*` tile arrays, `wave_text`, `difficulty_speed*_limit`, `title_*`. None are used outside their file.
5. **Dead state.** `torpedo_is_fired` is only ever written. `wave_init`'s `g_wave_size = 4` is always overwritten by `difficulty_adjust`.
6. **Calling-convention waste.** SDCC 4.6 uses `__sdcccall(1)`: the 1st param goes in A/HL, the 2nd in L/DE, and **the 3rd and later go on the stack plus an IX frame**. `torpedo_fire` (5 params) and `lane_start_state_exploding` (3 params) pay this at every call site (seen in `out/controls.asm` and `out/torpedo.asm`).

---

## 2. Globals vs. parameters on Z80 (policy)

| Pattern | Cost | Use for |
|---|---|---|
| `static` file-scope variable | same as a global (absolute `ld a,(nn)`) | **everything file-private: free encapsulation** |
| global with one writer module | absolute addressing | hot shared state: `sprites_attributes`, `g_lanes`, `torpedoes`, `g_wave_size`, `g_FrameCounter` |
| ≤2 small params | in registers, about as cheap as a global | module APIs (`lane_explode(lane, is_big)`) |
| ≥3 params | stack push at each call site + IX frame | avoid. Pass an index into a `const` table instead |
| getter function | call/ret per access | avoid on hot paths. Use a macro or the `inline` header style MSXgl already uses |
| `lane_t*` pointer access | IX/IY-indexed loads | fine for state machines. Keep the constant-index unrolling |

Rule: **each piece of shared state has exactly one owning module that writes it. Other modules read it directly through an `extern` in the owner's header, or call the owner's 0–2-param API to change it.**

---

## 3. Proposed layout (Phase 3)

Basenames must stay unique and must not clash with MSXgl modules (`game.c`, `state.c`, `input.c`, `tile.c`, `debug.c`, … all exist in `engine/src`). That is why the flow file is `flow.c` and not `game.c`.

```
atlantis/
├── atlantis.c                 boot only: video/sprites/music init, hook install, flow_run()
├── project_config.js          AddSources + CompileOpt -I for each subdir
├── msxgl_config.h
└── src/
    ├── platform/              the only layer that talks to VDP/BIOS/PSG/Arkos
    │   ├── frame.c/.h         g_FrameCounter, MusicFrameHook, frame_install_hook()
    │   ├── tiles.c/.h         TILES_PUT_BUFFER, new TILES_FILL, tiles_init, border
    │   ├── sprites.c/.h       SAT shadow, flush, magnification, pattern upload
    │   ├── music.c/.h         AKM wrappers (DI/EI) — unchanged
    │   └── controls.c/.h      keyboard read + fire edge — unchanged logic
    ├── game/                  pure game logic: no direct VDP calls except via platform macros
    │   ├── flow.c/.h          title → waves → game over → hi-score; flow_play_frames()
    │   ├── wave.c/.h          wave build + wave counter
    │   ├── difficulty.c/.h
    │   ├── lane.c/.h          lane state machine
    │   ├── queue.c/.h
    │   ├── gorgons.c/.h
    │   ├── torpedo.c/.h       + torpedo_hide(), launcher table
    │   ├── deathray.c/.h      NEW: armed flag, ray sprites, ray-vs-city hit
    │   ├── collision.c/.h     torpedo-vs-gorgon only
    │   ├── city.c/.h          installations, edges (moved here), rebuild, animation
    │   └── score.c/.h
    ├── ui/
    │   ├── hud.c/.h           HUD row buffer only
    │   └── screens.c/.h       title, wave banner, game-over and hi-score text
    └── data/                  const-only, zero code → future bank segments
        ├── tileset.h          generated (trimmed to 138 tiles)
        ├── song.h effects.h   generated
        ├── sprite_data.c/.h   gorgon + fx patterns, explosion anim tables (from sprites.c)
        ├── city_data.c/.h     map_background, installation tables (from city.c)
        └── text_data.c/.h     title logo, fixed strings (from hud.c)
```

Dependency direction: `atlantis.c → game/flow → game/* , ui/* → platform/* , data/*`. `platform/` and `data/` never include `game/`.

---

## 4. Critical refactoring snippets

**4.1 One frame-loop helper** (`game/flow.c`). It replaces 3 duplicated loops and keeps the existing update order.
```c
#define PLAY_LANES      0x01
#define PLAY_TORPEDOES  0x02
#define PLAY_COLLIDE    0x04

// Run `count` frames (count > 0) of the non-interactive game loop.
static void flow_play_frames(u16 count, u8 what)
{
    do {
        Halt();
        sprites_flush();
        map_update();
        if (what & PLAY_LANES)     lane_update_all();
        if (what & PLAY_TORPEDOES) torpedo_update_all();
        if (what & PLAY_COLLIDE)   collision_handle_torpedoes();
    } while (--count);
}
// wave banner: flow_play_frames(120, PLAY_TORPEDOES);
// game over:   flow_play_frames(270, PLAY_LANES | PLAY_TORPEDOES | PLAY_COLLIDE);
// hi-score:    flow_play_frames(480, 0);
```
`screens.c` then only draws text, and `flow.c` sequences draw → play frames → clear. The main in-wave loop stays explicit (it adds `controls_update()` before `Halt()`).

**4.2 ROM-resident text + VDP fill** (`ui/screens.c`, `platform/tiles.h`). This removes the stack-built arrays, about −350 B.
```c
#define TILES_FILL(X, Y, VALUE, SIZE)  VDP_FillVRAM_16K((VALUE), g_ScreenLayoutLow + (Y)*32 + (X), (SIZE))

void screens_draw_game_over(void)
{
    static const u8 text[] = { ' ','G','A','M','E',' ','O','V','E','R',' ' };   // ROM
    TILES_FILL(11, 5, ' ', sizeof(text));
    TILES_PUT_BUFFER(11, 6, text, sizeof(text));
    TILES_FILL(11, 7, ' ', sizeof(text));
}
// wave banner cleanup: TILES_FILL(11, 5..7, MAP_TILE_SKY, 10) instead of copying
// from map_background (its first 10 tiles are plain sky 0x23), which removes
// wave.c's dependency on city data.
```

**4.3 City owns its edges** (`game/city.h`). The edges move out of collision, and `collision_init` folds into `map_init`.
```c
extern u8 city_left_edge, city_right_edge;      // defined in city.c, reset in map_init()
#define city_is_standing() \
    (city_left_edge <= city_right_edge || map_have_installation[MAP_ACROPOLIS_COMMAND_ID])

u8 city_rebuild(u8 credits);   // returns the unspent credits (code moved from wave_finish)
// wave_finish(): score_credits = city_rebuild(score_credits);
// main loop:     while (city_is_standing()) { ... }
```

**4.4 Deathray module** (`game/deathray.c`). It gathers the 4 scattered pieces and keeps their exact semantics.
```c
static u8 deathray_armed;                 // was lane.c g_is_attacking

void deathray_arm(void)  { deathray_armed = 1; }          // last-lane gorgon enters
void deathray_hide(void) { /* 5× Color = TRANSPARENT (from sprites.c) */ }

// Called by the last lane while moving (replaces the lane.c block).
void deathray_update(u8 x, i8 velocity)
{
    if (!deathray_armed) return;
    sfx_play(SFX_DEATHRAY, 0, 0);
    /* move/animate the 5 sprites (from sprites_update_deathray) */
    /* hit test (from collision_handle_deathray); on a hit:
       map_destroy_installation(i); deathray_armed = 0; deathray_hide(); */
}
```
`map_destroy_installation` loses its `g_is_attacking = 0; sprites_disable_deathray();` lines. Its only callers are in the deathray hit path, so behaviour does not change, and city.c stops depending on lane and sprites.

**4.5 Fewer stack params**
```c
// torpedo.h — 1 param in A instead of 5 on the stack
#define TORPEDO_LAUNCHER_LEFT   0
#define TORPEDO_LAUNCHER_CENTER 1
#define TORPEDO_LAUNCHER_RIGHT  2
void torpedo_fire(u8 launcher);           // reads a static const {patt,x,y,dx,dy}[3] table
void torpedo_hide(u8 index);              // the 3 lines currently in lane_start_state_exploding

// lane.h — 2 register params instead of 3 on the stack; collision.c calls torpedo_hide() first
void lane_explode(u8 lane_index, u8 is_big);
```

**4.6 Banking-ready grouping (design only, not executed).** If the ROM ever passes 32 KB, use `Target = "ROM_ASCII16"` (or ASCII8):
- **Segment 0, fixed at 4000h:** crt0, `atlantis.c`, `platform/*`, `game/*`, `ui/*`, MSXgl libs, the AKM player. The ISR and all per-frame code must stay here.
- **Switchable segments at 8000h:** each `data/*` file, renamed to MSXgl's `<ProjSegments>_s<seg>_b<bank>.c` convention. Each is mapped with `SET_BANK_SEGMENT(bank, seg)` (`engine/src/rom_mapper.h`) **only during load-time copies to VRAM/RAM** (boot, title screen). It is never mapped in the frame loop or the ISR. This works because the song, SFX and all graphics are already copied out at load time.
- Avoid `__banked` calls on the hot path.

---

## 5. MSXgl integration verdict

| Area | Native option | Verdict |
|---|---|---|
| Input | `input_manager` (IPM) | **Keep the custom code.** IPM adds event tables and timers for a single rising-edge button. The DI/EI around `Keyboard_ReadAsJoystick` stays. |
| Sprites | `game_pawn`, `sprite_fx` | **Keep the SAT shadow.** Pawn adds RAM per pawn plus RT loading and physics that are not needed with fixed planes. `sprite_fx` flip could replace 6 mirrored gorgon patterns (−192 B data) but brings its own code. Only worth it if ROM gets tight. |
| Game loop / states | `game/state` (`Game_SetState`, `Game_VSyncHook`), `fsm` | **Don't adopt.** `game/state` installs its own vsync hook and 50 Hz sync, which conflicts with the H_TIMI + AKM hook. Four linear states don't need function-pointer dispatch. |
| Audio | Arkos AKM | Already native. Keep the `music.c` DI/EI wrappers. |
| VRAM text | `print` | No. Use the native `VDP_FillVRAM_16K` for blanks (4.2). |
| Config | `msxgl_config.h` | Try turning off unused features and keep each change only if the ROM shrinks: `INPUT_USE_MOUSE`, `INPUT_USE_DETECT`, `VDP_USE_MODE_T1/G1/MC`, `VDP_USE_COMMAND`, `VDP_USE_UNDOCUMENTED`. |

---

## 6. Execution phases (on approval; build after every step)

**Phase 1: size wins, no behaviour change**
1. Make function-local text arrays `static const`. Replace space-only borders with `TILES_FILL` (`hud.c`, `wave.c`).
2. Trim the tileset to 138 tiles (−1888 B). Regenerate `src/tileset.h` with MSXimg, using its `-num`/range option if it works with `--tilesUnique`. Otherwise truncate the generated arrays to `TILES_COUNT*8` bytes. Either way, check that the first 1104 bytes of `tileset_Patterns` and of `tileset_Colors` are byte-identical to the current header, and update the regeneration note in CLAUDE.md.
3. Remove `torpedo_is_fired` and the dead `g_wave_size = 4`.
4. Make the `msxgl_config.h` trims one at a time, measuring each.

**Phase 2: ownership and encapsulation (same files)**
- Add `static` to every file-private symbol from Diagnosis item 4 and remove it from its header.
- Move `g_FrameCounter` and the hook into `frame.c/.h`.
- Apply 4.3, 4.4 and 4.5.
- Add `flow.c` with 4.1. `atlantis.c` becomes boot plus `flow_run()`.
- Split `hud.c` into `hud.c` and `screens.c`.

**Phase 3: directory layout (moves only)**
- Create the tree from §3.
- Move data tables into `data/*.c`.
- Update `AddSources` and `CompileOpt` (`-I` for each subdir) in `project_config.js`, and the README/CLAUDE.md architecture section.
- `.vscode/c_cpp_properties.json` has uncommitted user edits: tell the developer which include paths to add rather than overwriting it.

**Phase 4:** banking stays design-only (4.6).

## Verification

- `./build.sh` after each step. Report the `Seg[0000]` ROM line and `s__DATA`/`l__DATA`/`s__INITIALIZED`/`l__INITIALIZED` against the baseline (17050 B ROM, 496 B RAM). Expected after Phase 1: about 14.8 KB ROM, RAM unchanged or lower. Phases 2–3 should be roughly ROM-neutral; flag any growth.
- After Phase 1 step 2, compare the VRAM pattern/colour bytes by diffing the trimmed arrays against the old header.
- Emulator checks for the developer:
  - title logo and text, and the blink;
  - the WAVE banner, and the sky restored behind it;
  - all 3 launchers, and the centre launcher disabled without the acropolis;
  - small and bandit explosions (magnification and border flash);
  - deathray animation and SFX, installations destroyed from both edges and the acropolis first;
  - end-of-wave bonus and rebuild order;
  - GAME OVER sequence with gorgons still flying, the hi-score screen, music switching;
  - generator and water animation.
- Never `git commit`. Leave the review to the developer.
