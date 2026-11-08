# Atlantis: MSX game in C (SDCC, Z80), migrating from ubox to MSXgl

## Current goal
Migrate this project from the ubox library to MSXgl, INCLUDING the whole asset
pipeline. When finished, the project must use MSXgl only: no ubox calls,
includes or ubox tools left in the build.

## Repository layout (two separate builds, never mixed)
- Legacy (ubox): everything in src/ plus the root Makefile. Reference only. It
  must keep working until the migration is finished.
- New (MSXgl): the project lives at the repo root (build.sh, project_config.js,
  msxgl_config.h, atlantis.c, ...). Build output goes to out/ and emul/ (git-ignored).
- Never include ubox headers from the MSXgl project, and never mix ubox and
  MSXgl code in the same build.

## Libraries and documentation (shared with other projects, READ-ONLY: never edit them)
- MSXgl (TARGET): /home/carlucio/Projects/MSXgl, reference alias `msxgl`.
- ubox 1.1.9 (LEGACY, being removed): /home/carlucio/Projects/ubox-msx-lib-1.1.9,
  reference alias `ubox`. Use it only to understand what the current code does.
  Its music player is based on Arkos Tracker AKM (see src/mplayer/akm/README.md).
- MSXgl wiki snapshot (offline copy of the user documentation):
  /home/carlucio/Projects/msxgl-wiki, reference alias `msxgl-wiki`.
- MSX and SDCC notes (hardware, memory map, VDP, interrupts, sound, SDCC
  specifics, common mistakes): /home/carlucio/Projects/msx-notes/MSX_SDCC_NOTES.md,
  reference alias `msx-notes`.
  READ THIS FILE before writing or changing any hardware-level code (VDP,
  interrupts, memory map, sound, assembly) and at the start of any planning task.

## Where to look things up (in this order)
1. API signatures, types, macros: the library headers.
   - MSXgl: /home/carlucio/Projects/MSXgl/engine/src/*.h (module headers, with
     Natural Docs comments)
   - ubox: /home/carlucio/Projects/ubox-msx-lib-1.1.9/include/
2. Real usage examples:
   - MSXgl samples: /home/carlucio/Projects/MSXgl/projects/samples/ (e.g. the
     Arkos sample s_arkos.c for the Arkos players). Before using a module, grep
     the samples for it.
   - MSXgl project templates: /home/carlucio/Projects/MSXgl/projects/template and
     projects/template_msx2 (show the layout of build.sh, project_config.js,
     msxgl_config.h and main.c).
   - ubox example game and tests: /home/carlucio/Projects/ubox-msx-lib-1.1.9/game/
     and tests/.
3. Library documentation:
   - ubox (small Markdown files, read them when understanding the current code):
     /home/carlucio/Projects/ubox-msx-lib-1.1.9/docs/
       - reference.ubox.md: main API reference (start here for ubox)
       - mplayer.md and src/mplayer/akm/README.md: music player (Arkos AKM)
       - spman.md: sprite manager
       - tools.md: ubox asset tools (png2tiles, etc.)
       - extra.md: additional notes
   - MSXgl wiki (user documentation: modules, Arkos RAM area, MSXimg, build tool),
     offline snapshot: /home/carlucio/Projects/msxgl-wiki/. One wikitext file per
     page, e.g. Modules__arkos__akm_player.txt. Use grep/glob to find the page,
     then open only that file.
   - MSXgl generated HTML docs: /home/carlucio/Projects/MSXgl/engine/doc/html
     (large; last resort, open only the specific file you need).
4. MSXgl build system:
   - Defaults: /home/carlucio/Projects/MSXgl/projects/default_config.js
   - Build scripts: /home/carlucio/Projects/MSXgl/engine/script/ (js/build.js)
   - Build targets: /home/carlucio/Projects/MSXgl/projects/targets/
   - Embedded tools (SDCC, MSXtk converters such as MSXimg):
     /home/carlucio/Projects/MSXgl/tools/ (use ls to find the exact binaries).
5. Project-local notes: docs/ in this repo (if present).

- Do not read documentation folders in bulk. Search (grep) for the symbol or
  module name first, then open only the relevant file.
- If the wiki and the headers disagree, the headers are right (the wiki may
  describe a different MSXgl version).
- Prefer local files. Use webfetch only if a page is missing from the snapshot
  (the wiki is at https://aoineko.org/msxgl/), and tell the developer when you do.
- If the answer is not in any of these, say so. Do not guess. Never rely on
  memory for MSX hardware facts, SDCC behavior or MSXgl/ubox APIs.

## MSX and SDCC essentials (hard rules; details in MSX_SDCC_NOTES.md)
Machine and memory
- Z80 at about 3.58 MHz, one frame is about 59k (60 Hz) or 71k (50 Hz) T-states.
  Game logic, sprite updates, VRAM flush and sound replay must fit in one frame.
- RAM is scarce: do not assume more than 16 KB unless the project facts below say so.
  VRAM is separate and reachable only through the VDP I/O ports.
- Memory is four 16 KB pages mapped to slots. ROM carts live at 4000h-BFFFh.
  NEVER write to ROM addresses (on mapped ROMs a write switches banks).
Video and interrupts
- Respect the limits of the screen mode in use (MSX1: 4 sprites per scanline,
  one colour per sprite, tile colour rules). A tile conversion can succeed and
  still look wrong: check the screen mode before converting assets.
- Write VRAM during vblank or flush a RAM queue then. Use the MSXgl VDP functions.
- The VDP raises one interrupt per frame. The handler must read VDP status
  register 0 to acknowledge it. Keep handlers short, share variables as
  `volatile`, protect 16-bit shared values with DI/EI, and never let the handler
  and the main loop interleave VDP access sequences.
- Sound players must be called once per frame. Songs made for 50 Hz run faster at 60 Hz.
C (SDCC)
- char = 8 bits, int = 16 bits, long = 32 bits, pointers = 16 bits. Prefer
  uint8_t/uint16_t. Beware integer promotion.
- No hardware multiply or divide: use shifts, masks and tables. No float or long
  in hot code. No printf, stdio or malloc.
- Locals live on a small stack: no large local arrays, no recursion.
- Use `const` for read-only tables so they stay in ROM.
- After each build, report ROM and RAM usage from the linker map and compare
  with the legacy build. Warn when it grows.
- For hot code, read the generated assembly instead of guessing.
Assembly
- Inline assembly uses the sdasz80 (ASxxxx) syntax, which differs from standard Z80 syntax:
  - immediate numeric values are prefixed with `#` (e.g. `ld a, #5`)
  - assembler directives start with `.`
  - indexed addressing writes the offset before the register: `2(ix)`, not `(ix+2)`
- SDCC has no clobber lists: assume nothing about registers across C statements.
  Prefer `__naked` functions with an explicit `ret` or existing MSXgl assembly helpers.
- The SDCC calling convention (`__sdcccall(0)` stack-based vs `__sdcccall(1)`
  register-based) depends on the SDCC version. The legacy build (SDCC 4.0.0) and
  the SDCC embedded in MSXgl may differ. Review every piece of hand-written or
  inline assembly for calling convention, register usage and syntax before
  carrying it over.

## Toolchains (do NOT change or override them)
- Legacy build: the Makefile pins SDCC 4.0.0 and ubox 1.1.9 with absolute paths.
  Never edit SDCC_PATH or UBOX_PATH, and never call the system `sdcc` (4.6.0).
  Keep the legacy Makefile working until the MSXgl build is complete and verified.
- MSXgl build: ./build.sh uses the SDCC embedded in MSXgl/tools/sdcc (SDCC 4.6.0,
  confirmed with `--version`) and the Node bundled in MSXgl/tools/build/Node (the
  system has no node). Never call the system `sdcc` yourself: the MSXgl build
  tool picks its own compiler.
- Do not install or replace tools.
- The legacy build uses --std-sdcc99, -mz80, --fsigned-char, --opt-code-speed and
  --Werror, builds the ROM at 4000h with hex2bin, and uses --no-std-crt0 with the
  ubox startup code. Do not change compiler flags unless asked.

## Project facts
- Target machine (MSX1, MSX2, ...): MSX1 (TMS9918A)
- Screen mode used: Screen 2 (Graphic 2), 256×192
- ROM type (plain 16K/32K or mapped, which mapper): Plain 32 KB ROM, no mapper, at 4000h, header "AB"
- ROM size : ~14.5 KB used (0x4000–0x799B), padded to 32 KB
- RAM used by the game (from the linker map) and where the stack lives: ~444 bytes game data (0xC0DE–0xC29A) + 222-byte AKM player buffer at 0xC000
- Stack: SP = 0xF380 (grows down), set in crt0.z80
- PAL (50 Hz) or NTSC (60 Hz) assumptions in the game logic: No explicit 50/60 Hz handling; frame-animated via ubox_tick (8-bit counter); song authored for 50 Hz will play ~20% fast at 60 Hz
- Sound hardware used (PSG only, SCC, ...): PSG only, via Arkos Tracker 2 AKM player (+ SFX bank)
- Sprite usage (size, count, multiplexing): 16×16 sprites, pattern table at 0x3800, SAT at 0x1B00; 17 attributes (68 bytes), max 32
- Interrupt handler usage in the legacy code: BIOS HTIMI hook (keeps BIOS handler); user ISR = mplayer_play; ubox_tick incremented per frame

## Asset pipeline migration
- Legacy pipeline (to be REPLACED, not ported): ubox png2tiles.py for graphics;
  rasm + disark + ubox mplayer with an Arkos Tracker 2 song (src/akm.z80) for music.
- Target pipeline, all from MSXgl:
  - Graphics: MSXimg (MSXgl tools/MSXtk), driven by the MSXgl build tool data
    configuration in project_config.js (see the template project and samples).
  - Music/SFX: the MSXgl Arkos modules (arkos/akm_player, akg_player, aky_player).
- Original assets (source of truth, never delete or overwrite them):
  - assets/gfx/tiles.png: tileset image.
  - assets/sfx/song.aks: Arkos Tracker 2 music.
  - assets/sfx/effects.aks: Arkos Tracker 2 sound effects bank.
  Generated data (tile data, exported songs and effects) is build output.
- The sound effects come from effects.aks, so the target player must support
  Arkos SFX (the AKG and AKM players do, per the MSXgl documentation). Do not
  switch to ayFX unless asked.
- Do not use ubox tools (png2tiles.py, rasm, hex2bin, mplayer) in the final MSXgl build.
- Arkos Tracker 2 is a GUI app that the agent cannot run. When a song must be
  re-exported, tell the developer the exact format (AKG/AKM/AKY), the "Encode to
  address" value and the options to use, and wait.
- The Arkos player uses a fixed RAM work area (default 0xF000, ~275 bytes) and
  the song must be placed at its export address. Check the memory map and make
  sure nothing overlaps before choosing addresses.

## Migration rules
- The migration plan is docs/MIGRATION_PLAN.md. Read it at the start of every
  session. At the end of every step, update its Status section and ROM/RAM log.
- Never migrate everything at once. Work in small steps; each step must end with
  a passing build.
- For every ubox call being replaced, find the MSXgl equivalent by reading the
  MSXgl header. If there is no direct equivalent, say so and propose options.
  NEVER invent an API.
- Do not assume ubox and MSXgl share the same architecture (startup code/crt0,
  interrupt handling, VRAM/sprite handling, memory map, ROM type). Check these
  explicitly and list the differences before changing code.
- The compiler changes from SDCC 4.0.0 (legacy) to SDCC 4.6.0 (MSXgl) during this
  migration: code size, speed and warnings differ. Do not attribute a change in
  ROM/RAM size or behavior to the library alone, and expect new warnings (the
  legacy build uses --Werror).
- Keep gameplay logic unchanged; only change the platform layer, unless asked otherwise.
- Compiling does not prove correct behavior. After each step, list what the
  developer should verify in the emulator (the agent cannot run it).

## Code rules
- Before using any MSXgl function, read the corresponding header and confirm
  its signature, types and macros.
- MSXgl is compiled dynamically according to msxgl_config.h and the module list
  (LibModules) in project_config.js: a module that is not enabled there does not
  exist in the build. Check both before using a module.
- Match the style of the existing code.

## Build and verification
- Legacy ubox build: `make` (legacy code in src/)
- MSXgl build: `./build.sh` at the repo root. Outputs: emul/rom/atlantis.rom and
  the linker map out/atlantis.map.
- After editing any .c/.h/.asm file, run the applicable build and fix compile
  errors before finishing the task.
- NEVER run `git commit` or `git push`. Stop after the build passes and leave
  review and commit to the developer.