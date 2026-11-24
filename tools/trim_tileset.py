#!/usr/bin/env python3
"""Trim src/tileset.h (exported by MSXimg with all 256 tiles) to TILES_COUNT tiles.

Usage: python3 tools/trim_tileset.py [tileset.h] [tiles.h]

Reads TILES_COUNT from src/tiles.h, keeps the first TILES_COUNT*8 bytes of
tileset_Patterns and tileset_Colors, drops the per-tile comments past the
limit and fixes the size comments. Safe to run on an already trimmed file.
"""
import re
import sys

tileset_path = sys.argv[1] if len(sys.argv) > 1 else 'src/tileset.h'
tiles_path = sys.argv[2] if len(sys.argv) > 2 else 'src/tiles.h'

m = re.search(r'#define\s+TILES_COUNT\s+(\d+)', open(tiles_path).read())
if not m:
    sys.exit('TILES_COUNT not found in ' + tiles_path)
count = int(m.group(1))
limit = count * 8

out = []
arr = None
kept = 0
sizes = {}
for line in open(tileset_path).read().split('\n'):
    name = re.match(r'const unsigned char (tileset_\w+)\[\]', line)
    if name:
        arr, kept = name.group(1), 0
    elif arr and line.startswith('};'):
        sizes[arr] = kept
        if kept < limit:
            sys.exit('%s has only %d bytes, need %d (TILES_COUNT too high?)' % (arr, kept, limit))
        arr = None
    elif arr:
        n = len(re.findall(r'0x[0-9A-Fa-f]{2}', line))
        if n:  # data row
            if kept >= limit:
                continue
            if kept + n > limit:
                sys.exit('a data row straddles the limit; unexpected MSXimg layout')
            kept += n
        else:
            sp = re.match(r'// Sprite\[(\d+)\]', line)
            if sp and int(sp.group(1)) * 8 >= limit and line.count('offset:') and kept >= limit:
                continue
    if line.startswith('// Patterns size'):
        line = '// Patterns size: %d Bytes (trimmed to the first %d tiles; see TILES_COUNT)' % (limit, count)
    elif line.startswith('// Colors size'):
        line = '// Colors size: %d Bytes (trimmed to the first %d tiles; see TILES_COUNT)' % (limit, count)
    elif line.startswith('// Total size'):
        line = '// Total size: %d Bytes' % (limit * 2)
    out.append(line)

open(tileset_path, 'w').write('\n'.join(out))
print('tileset trimmed to %d tiles (%d bytes per table)' % (count, limit))
