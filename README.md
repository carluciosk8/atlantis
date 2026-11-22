# Atlantis

A clone of the classic game *Atlantis* for the MSX1, written in C with [SDCC](https://sdcc.sourceforge.net/) (Z80) on top of the [MSXgl](https://github.com/aoineko-fr/MSXgl) library.

The ROM is a plain 32 KB cartridge image (loaded at `4000h`) that runs on any MSX1 or later.

## Gameplay

Gorgon ships attack Atlantis in waves. Defend the city's installations by shooting the ships down with torpedoes before they reach the end of their lane and fire the deathray.

- Fighters and bombers score 1 point (straight) or 2 (angled). Bandits score 10 or 20.
- Each installation still standing at the end of a wave is worth 5 points.
- Every time the score crosses a multiple of 100 you earn a bonus installation. The score saturates at 999999.

The game cycles through the title screen, the waves, game over and the hi-score screen.

## Building

Requirements:

- [MSXgl](https://github.com/aoineko-fr/MSXgl), which bundles Node and SDCC 4.6.0. By default it is expected next to this project, at `../MSXgl`.

```sh
./build.sh
```

To use MSXgl from another location:

```sh
MSXGL_PATH=/path/to/MSXgl ./build.sh
```

Output (both directories are git-ignored):

- `emul/rom/atlantis.rom`: the 32 KB ROM
- `out/atlantis.map`: the linker map

Run the ROM in an MSX1 emulator such as [openMSX](https://openmsx.org/) or [blueMSX](http://www.bluemsx.com/).

## Project layout

| Path | Contents |
|---|---|
| `atlantis.c` | `main()`, boot, vblank hook and the game loop |
| `src/` | Game modules: waves, lanes, torpedoes, collisions, city, HUD, score, sprites and music |
| `assets/gfx/` | Tileset source images |
| `assets/sfx/` | Arkos Tracker 2 song and sound effects (`.aks` sources, `.akm`/`.akx` exports) |
| `project_config.js` | MSXgl target, library modules and project sources |
| `msxgl_config.h` | Compile-time switches for the MSXgl modules |

When adding a new `src/*.c` file, list it in `AddSources` in `project_config.js`. Do not name a source file after an MSXgl module (`input.c`, `system.c`, `math.c`, …): the object files would collide at link time.

## Technical notes

- **Video:** TMS9918A in Screen 2, with 16×16 sprites held in RAM and copied to the sprite attribute table once per frame, right after vblank.
- **Sound:** the Arkos AKM player runs from the vblank (`H_TIMI`) hook. The song and sound effects are position-dependent and are copied from ROM to fixed RAM addresses at boot.
- **Memory map:** game RAM from `C000h`, song at `D000h`, sound effects at `E000h`, Arkos buffer at `F000h`.
- **Code conventions:** no float, long, `printf` or `malloc`. Plain `char` is unsigned in this build, so signed 8-bit values use `i8`.

## Regenerating assets

`src/tileset.h`, `src/song.h` and `src/effects.h` are committed and are not produced by the build. They are generated with the MSXtk tools in `MSXgl/tools/MSXtk/bin/`:

```sh
MSXimg assets/gfx/tiles_msx1.png -mode gm2 -pal msx1 --tilesUnique -name tileset -out tileset.h
MSXbin assets/sfx/song.akm -t g_SongData -ad -o song.h
MSXbin assets/sfx/effects.akx -t g_SFXData -ad -o effects.h
```

`tiles_msx1.png` is a copy of `tiles.png` with two greens adjusted to the MSX1 palette. The `.akm` and `.akx` files are exported from Arkos Tracker 2 with "Encode to address" set to `D000h` and `E000h` respectively.

## Licence

This work is licensed under [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International](LICENCE).
