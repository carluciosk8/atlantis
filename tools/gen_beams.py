#!/usr/bin/env python3
"""Generate the night spotlight tables of src/data/sky_data.c.

Run it with `python3 -I tools/gen_beams.py` and paste its output over the
block between the "gen_beams.py" markers in src/data/sky_data.c.

Each spotlight is a wedge from a pivot above its launcher, bounded by two
lines of slope dx/dy (0 = straight up, larger = closer to the horizon). Every
layout lights both beams with the same wedge, mirrored. The pixels are cut into
8x8 tiles: plain black cells use the night sky tile (0x23) or keep their star
or moon tile, fully lit cells use the solid beam tile, and every other cell gets
an edge tile (a beam hides whatever it touches). The beam tiles and the 2 dim
star tiles are loaded at night into pool slots (see SKY_BEAM_SLOTS_* in
sky_data.h): the beam tiles light yellow on black, the dim stars with the
colours in sky_beam_star_colors.
"""

LEFT_PIVOT = (0.0, 104.0)     # above the left launcher (row 13, columns 0..1)
RIGHT_PIVOT = (256.0, 96.0)   # above the right launcher (row 12, columns 30..31)

# Wedges (slope range) of the layouts, swept 1 -> 2 -> 3 -> 4 -> 3 -> 2.
WEDGES = [(0, 1), (0.5, 2), (1, 4), (2.5, 8)]

ROWS = 13                     # rows 0..11, plus row 12 columns 0..1 (left beam only)
SKY = 0x23
# Bright stars (tiles.png): (row, column, tile).
STARS = [(0, 7, 0x98), (0, 14, 0x99), (0, 26, 0x99), (1, 2, 0x9A), (1, 19, 0x9A),
         (2, 12, 0x99), (3, 21, 0x98), (3, 29, 0x98), (5, 4, 0x99), (6, 24, 0x9A),
         (9, 9, 0x98), (9, 28, 0x99), (10, 1, 0x9A), (10, 16, 0x98)]

# Crescent moon rising over the sea (tiles.png 0x06, 0x0A, 0x0E, 0x19): the
# least lit cells of the sweep (only layout 4 reaches them).
MOON = [(10, 20, 0x06), (10, 21, 0x0A), (11, 20, 0x0E), (11, 21, 0x19)]

# Dim stars: one pixel, alternately light blue and dark blue. Their 2 tiles
# follow the beam tiles in the pool.
DIM_STAR_TILES = [
    ((0, 0, 0, 0, 0, 0x40, 0, 0), 0x51),   # light blue pixel, line 5
    ((0, 0, 0x08, 0, 0, 0, 0, 0), 0x41),   # dark blue pixel, line 2
]
DIM_STARS = [(0, 4), (0, 9), (0, 12), (1, 22), (1, 24), (1, 30), (2, 5), (2, 10), (2, 16),
             (2, 26), (3, 2), (3, 8), (3, 14), (3, 24), (4, 18), (5, 1), (5, 6), (5, 9),
             (5, 12), (5, 31), (7, 6), (7, 10), (7, 17), (7, 31), (8, 0), (8, 4), (8, 14),
             (8, 21), (10, 4), (10, 14), (10, 26), (11, 7), (11, 10), (11, 23)]

# Pool slots: 23 from 0x60 (blank in tiles.png), then 43 from 0xD5 (free at
# night: the cloud and bird slots and the unloaded tiles).
SLOTS = [0x60 + i for i in range(23)] + [0xD5 + i for i in range(43)]


def lit(x, y, lo, hi):
    """Is pixel (x, y) inside the wedge lo..hi of either beam?"""
    for side, (px, py) in (('L', LEFT_PIVOT), ('R', RIGHT_PIVOT)):
        dy = py - (y + 0.5)
        if dy <= 0:
            continue
        if y >= 96 and not (side == 'L' and x < 16):
            continue
        dx = (x + 0.5 - px) if side == 'L' else (px - (x + 0.5))
        if dx >= 0 and lo <= dx / dy < hi:
            return True
    return False


def cell(lo, hi, r, c):
    return tuple(sum(lit(c * 8 + x, r * 8 + y, lo, hi) << (7 - x) for x in range(8))
                 for y in range(8))


FULL = (0xFF,) * 8
EMPTY = (0,) * 8
tiles = [FULL]                # pattern -> pool index; the solid tile first
layouts = []
for lo, hi in WEDGES:
    names = []
    for r in range(ROWS):
        for c in range(32 if r < 12 else 2):
            p = cell(lo, hi, r, c)
            if p == EMPTY:
                names.append(SKY)
                continue
            if p not in tiles:
                tiles.append(p)
            names.append(SLOTS[tiles.index(p)])
    layouts.append(names)

beam_count = len(tiles)
dim = [SLOTS[beam_count + k] for k in range(len(DIM_STAR_TILES))]
tiles += [p for p, _ in DIM_STAR_TILES]
assert len(tiles) <= len(SLOTS), f"{len(tiles)} tiles, {len(SLOTS)} slots"
decorations = STARS + MOON + [(r, c, dim[i % len(dim)]) for i, (r, c) in enumerate(DIM_STARS)]
for names in layouts:
    for r, c, t in decorations:
        if names[r * 32 + c] == SKY:
            names[r * 32 + c] = t
first = min(len(tiles), 23)

print("// ---- gen_beams.py: start (generated, do not edit by hand) ----")
print(f"// Wedges (dx/dy): {', '.join(f'{lo}..{hi}' for lo, hi in WEDGES)}.")
print(f"// {len(tiles)} tiles ({beam_count} beam, {len(dim)} dim star): {first} at 0x60.., {len(tiles) - first} at 0xD5..")
print(f"const u8 sky_beam_tiles[SKY_BEAM_TILES*8] =\n{{")
for i, p in enumerate(tiles):
    print("    " + ",".join(f"0x{b:02X}" for b in p) + f",   // 0x{SLOTS[i]:02X}")
print("};\n")
print("// Colours of the dim star tiles (the last SKY_BEAM_STAR_TILES of the pool).")
print("const u8 sky_beam_star_colors[SKY_BEAM_STAR_TILES*8] =\n{")
for _, colour in DIM_STAR_TILES:
    print("    " + ",".join([f"0x{colour:02X}"] * 8) + ",")
print("};\n")
print("const u8 sky_beam_sky[SKY_BEAM_LAYOUTS][SKY_ROWS*32] =\n{")
for i, names in enumerate(layouts):
    print(f"    {{   // layout {i + 1}: {WEDGES[i][0]}..{WEDGES[i][1]}")
    for r in range(12):
        print("        " + ",".join(f"0x{t:02X}" for t in names[r * 32:(r + 1) * 32]) + ",")
    print("    },")
print("};\n")
print("// Row 12, columns 0..1 (the left beam above the left launcher).")
print("const u8 sky_beam_row12[SKY_BEAM_LAYOUTS][2] =\n{")
print("    " + " ".join("{ " + ",".join(f"0x{t:02X}" for t in names[384:386]) + " }," for names in layouts))
print("};")
print("// ---- gen_beams.py: end ----")
print(f"// SKY_BEAM_TILES {len(tiles)}", file=__import__('sys').stderr)
