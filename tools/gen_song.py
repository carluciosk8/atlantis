import re, zipfile
src = open('song.xml', encoding='utf-8').read()

NAMES = {'C':0,'C#':1,'D':2,'D#':3,'E':4,'F':5,'F#':6,'G':7,'G#':8,'A':9,'A#':10,'B':11}
def note(s):
    m = re.fullmatch(r'([A-G]#?)(\d)', s)
    return int(m.group(2))*12 + NAMES[m.group(1)]

# ---------------- new instruments ----------------
def cell(vol, noise=0, sa=0, spi=0, link='softOnly', ratio=4, hc=8):
    return (f"      <aks:fmInstrumentCell>\n        <aks:link>{link}</aks:link>\n        <aks:volume>{vol}</aks:volume>\n"
            f"        <aks:noise>{noise}</aks:noise>\n        <aks:softwarePeriod>0</aks:softwarePeriod>\n"
            f"        <aks:softwareArpeggio>{sa}</aks:softwareArpeggio>\n        <aks:softwarePitch>{spi}</aks:softwarePitch>\n"
            f"        <aks:ratio>{ratio}</aks:ratio>\n        <aks:hardwareCurve>{hc}</aks:hardwareCurve>\n"
            f"        <aks:hardwarePeriod>0</aks:hardwarePeriod>\n        <aks:hardwareArpeggio>0</aks:hardwareArpeggio>\n"
            f"        <aks:hardwarePitch>0</aks:hardwarePitch>\n        <aks:isRetrig>false</aks:isRetrig>\n      </aks:fmInstrumentCell>\n")

def inst(num, title, speed, loop_start, cells, color):
    looping = loop_start is not None
    s = (f'    <aks:fmInstrument version="1.0">\n      <aks:number>{num}</aks:number>\n      <aks:title>{title}</aks:title>\n'
         f'      <aks:speed>{speed}</aks:speed>\n      <aks:isLooping>{"true" if looping else "false"}</aks:isLooping>\n'
         f'      <aks:loopStartIndex>{loop_start or 0}</aks:loopStartIndex>\n      <aks:endIndex>{len(cells)-1}</aks:endIndex>\n'
         f'      <aks:isRetrig>false</aks:isRetrig>\n      <aks:colorArgb>{color}</aks:colorArgb>\n')
    return s + ''.join(cells) + '    </aks:fmInstrument>\n'

# 9 Deep Bass: attack click, quick decay, sustained body (no hardware envelope)
bass = [cell(15), cell(15), cell(14), cell(14), cell(13), cell(13), cell(12), cell(12), cell(11)]
# 10 Glass Lead: breathy onset, slow swell, vibrato loop
vib = [0, 3, 6, 3, 0, -3, -6, -3]
lead = [cell(6, noise=12), cell(9), cell(11), cell(12)] + [cell(12, spi=v) for v in vib]
# 11 Harp: bright octave pluck then decay
harp = [cell(15, sa=12), cell(14)] + [cell(v) for v in (13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2)]
# 12 Deep Pad: soft sustained arp-less pad with slight vibrato (used for held chord tones)
pad = [cell(2), cell(4), cell(6), cell(7)] + [cell(7, spi=v) for v in (0, 2, 4, 2, 0, -2, -4, -2)]
# 13 Soft Shaker: quieter noise hat
shaker = [cell(9, noise=2, link='noSoftNoHard'), cell(6, noise=2, link='noSoftNoHard'), cell(3, noise=2, link='noSoftNoHard')]
# 14 Soft Tom: punchy but not sub-bass, short pitch dip
tom = [cell(15, noise=8), cell(14, spi=0), cell(13, spi=40), cell(12, spi=80), cell(10, spi=120), cell(8, spi=160), cell(5, spi=200), cell(3, spi=240)]

NEW = (inst(9, 'Deep Bass', 0, 8, bass, 4291584204) + inst(10, 'Glass Lead', 1, 4, lead, 4283472332) +
       inst(11, 'Harp', 0, None, harp, 4294951167) + inst(12, 'Deep Pad', 1, 4, pad, 4287688304) +
       inst(13, 'Soft Shaker', 0, None, shaker, 4289339562) + inst(14, 'Soft Tom', 0, None, tom, 4292927712))
src = src.replace('  </aks:fmInstruments>', NEW + '  </aks:fmInstruments>', 1)

BASS, LEAD, HARP, PAD, SHAKE, TOM, KICK = 9, 10, 11, 12, 13, 14, 7

# ---------------- composition: D Phrygian ----------------
# chord = (root, third, fifth) as note names; bass root one octave lower
CH = {
  'Dm': ('D', 'F', 'A'), 'Eb': ('D#', 'G', 'A#'), 'C': ('C', 'E', 'G'),
  'Gm': ('G', 'A#', 'D'), 'A': ('A', 'C#', 'E'), 'Bb': ('A#', 'D', 'F'),
}
def n(name, octv):  # absolute note from pitch class name + octave
    return note(f'{name}{octv}')

def harp_bar(ch, up=False, drums='basic'):
    r, a, b = CH[ch]
    if not up:   # falling ripple, like water
        seq = [n(r,5), n(b,4), n(a,4), n(r,4), n(b,4), n(a,4), n(r,5) , n(b,4)]
    else:        # rising
        seq = [n(r,4), n(a,4), n(b,4), n(r,5), n(b,4), n(a,4), n(r,4), n(a,4)]
    t = {}
    for i, nt in enumerate(seq):
        t[i*2] = (nt, HARP, 13 if i % 4 == 0 else 10)
    if drums != 'none':
        t[7] = (n('D',4), SHAKE, 8); t[15] = (n('D',4), SHAKE, 8)
    if drums == 'full':
        t[3] = (n('D',4), SHAKE, 6); t[11] = (n('D',4), SHAKE, 6)
    return t

def bass_bar(ch, drums='basic'):
    r = CH[ch][0]
    t = {0: (n(r,2), BASS, 12), 3: (n(r,2), BASS, 12), 8: (n(r,2), BASS, 12), 10: (n(r,3), BASS, 12)}
    return t

def lead_bars(spec):
    t = {}
    for bi, bar in enumerate(spec):
        toks = bar.split()
        for i in range(0, len(toks), 2):
            nm = toks[i+1]
            t[bi*16 + int(toks[i])] = (note(nm), LEAD, 12)
    return t

def pad_bars(chs, voice=2):
    t = {}
    for i, c in enumerate(chs):
        t[i*16] = (n(CH[c][voice], 4), PAD, None)
    return t

def join(*bars_list):
    t = {}
    for i, b in enumerate(bars_list):
        for r, v in b.items(): t[i*16 + r] = v
    return t

prog_A = ['Dm', 'Dm', 'Eb', 'Dm']                # intro
prog_B = ['Dm', 'Eb', 'Dm', 'C']                 # theme
prog_C = ['Gm', 'Eb', 'Dm', 'A']                 # answer (A major = Phrygian dominant tension)
prog_D = ['Gm', 'Eb', 'Bb', 'A']                 # climax

L_B = lead_bars(['0 D5 8 C5 12 A4',  '0 A#4 8 G4 12 A#4',  '0 D5 6 D#5 8 D5 12 C5',  '0 A4'])
L_B2 = lead_bars(['0 D5 8 C5 12 A4', '0 A#4 6 C5 8 A#4 12 G4', '0 D5 6 D#5 8 F5 12 D5', '0 C5 8 A4 12 G4'])
L_C = lead_bars(['0 D5 4 A#4 8 D5 12 F5', '0 G5 8 F5 12 D#5', '0 D5 6 C5 8 A4 12 A#4', '0 C#5 4 E5 8 C#5 12 A4'])
L_D = lead_bars(['0 G5 8 F5 12 D5', '0 D#5 8 G5 12 A#5', '0 A#5 6 A5 8 F5 12 D5', '0 C#5 4 E5 8 A5 12 C#5'])


def pattern(lead_t, harp_prog, bass_prog, up=False, drums='basic'):
    return (lead_t, join(*[harp_bar(c, up, drums) for c in harp_prog]), join(*[bass_bar(c, drums) for c in bass_prog]))

def build_tracks(pats):
    tracks, cache = [], {}
    def tid(t):
        key = tuple(sorted(t.items()))
        if key not in cache:
            cache[key] = len(tracks); tracks.append(t)
        return cache[key]
    return tracks, [tuple(tid(t) for t in p) for p in pats]

MASTER = 3
def cell_xml(row, nt, ins, vol):
    if vol is not None: vol = min(15, vol + MASTER)
    s = f"          <aks:cell>\n            <aks:index>{row}</aks:index>\n            <aks:note>{nt}</aks:note>\n            <aks:instrument>{ins}</aks:instrument>\n"
    if vol is not None:
        s += f"            <aks:effectAndValue>\n              <aks:index>0</aks:index>\n              <aks:effect>volume</aks:effect>\n              <aks:hexValue>#{vol:x}00</aks:hexValue>\n            </aks:effectAndValue>\n"
    return s + "          </aks:cell>\n"

def subsong_xml(title, speed, loop_idx, pats, template, height=64):
    tracks, patterns = build_tracks(pats)
    head = template[:template.index('<aks:tracks>')]
    head = re.sub(r'<aks:title>[^<]*</aks:title>', f'<aks:title>{title}</aks:title>', head, count=1)
    head = re.sub(r'<aks:initialSpeed>\d+', f'<aks:initialSpeed>{speed}', head)
    head = re.sub(r'<aks:endIndex>\d+', f'<aks:endIndex>{len(patterns)-1}', head)
    head = re.sub(r'<aks:loopStartIndex>\d+', f'<aks:loopStartIndex>{loop_idx}', head)
    x = head + "<aks:tracks>\n"
    for i, t in enumerate(tracks):
        x += f"        <aks:track>\n          <aks:number>{i}</aks:number>\n          <aks:title>Unnamed</aks:title>\n"
        for row in sorted(t):
            nt, ins, vol = t[row]; x += cell_xml(row, nt, ins, vol)
        x += "        </aks:track>\n"
    x += ("      </aks:tracks>\n      <aks:speedTracks>\n        <aks:speedTrack>\n          <aks:number>0</aks:number>\n          <aks:title>Unnamed</aks:title>\n        </aks:speedTrack>\n      </aks:speedTracks>\n"
          "      <aks:eventTracks>\n        <aks:eventTrack>\n          <aks:number>0</aks:number>\n          <aks:title>Unnamed</aks:title>\n        </aks:eventTrack>\n      </aks:eventTracks>\n      <aks:patterns>\n")
    for pat in patterns:
        x += f"        <aks:pattern>\n          <aks:height>{height}</aks:height>\n          <aks:speedTrackNumber>0</aks:speedTrackNumber>\n          <aks:eventTrackNumber>0</aks:eventTrackNumber>\n"
        for tn in pat:
            x += f"          <aks:patternCell>\n            <aks:transposition>0</aks:transposition>\n            <aks:trackNumber>{tn}</aks:trackNumber>\n          </aks:patternCell>\n"
        x += "        </aks:pattern>\n"
    return x + "      </aks:patterns>\n    </aks:subsong>\n"

# ---- 0: Game (unchanged composition) ----
game_pats = [
  pattern({}, prog_A, prog_A, False, 'none'),
  pattern(L_B, prog_B, prog_B, False, 'basic'),
  pattern(L_C, prog_C, prog_C, True, 'basic'),
  pattern(L_B2, prog_B, prog_B, False, 'full'),
  pattern(L_D, prog_D, prog_D, True, 'full'),
  pattern(L_C, prog_C, prog_C, True, 'full'),
]

# ---- 1: Title (E Phrygian, slow, ethereal: sparse harp / soft pad, long bass, no drums) ----
CH.update({'Em': ('E','G','B'), 'F': ('F','A','C'), 'Am': ('A','C','E'), 'E': ('E','G#','B')})
def slow_harp(ch):
    r, a, b = CH[ch]
    seq = {0: n(r,4), 4: n(b,4), 8: n(r,5), 12: n(b,4)}
    return {k: (v, HARP, 13 if k == 0 else 10) for k, v in seq.items()}
def pad_bar(ch):
    return {0: (n(CH[ch][2],4), PAD, 15)}
def slow_bass(ch):
    r = CH[ch][0]
    return {0: (n(r,2), BASS, 12), 8: (n(r,2), BASS, 12)}
def tpat(lead_t, prog, mid):
    return (lead_t, join(*[(slow_harp(c) if mid == 'harp' else pad_bar(c)) for c in prog]),
                    join(*[slow_bass(c) for c in prog]))
TP1 = ['Em', 'F', 'Em', 'Dm']
TP2 = ['Am', 'F', 'Dm', 'E']
TL1 = lead_bars(['0 E5 10 B4', '0 F5 8 C5 12 A4', '0 G5 8 E5 12 B4', '0 A4'])
TL2 = lead_bars(['0 A4 6 C5 8 E5 12 A5', '0 A5 8 G5 12 F5', '0 F5 6 E5 8 D5 12 A4', '0 G#4 4 B4 8 E5'])
title_pats = [
  tpat({}, TP1, 'harp'),
  tpat(TL1, TP1, 'pad'),
  tpat(TL2, TP2, 'harp'),
  tpat(TL1, TP1, 'harp'),
  tpat(TL2, TP2, 'pad'),
]

# ---- 2: Game over (falling, ~4.5 s: 3 bars of 16 rows at speed 6) ----
def seq(rows, inst, vol):
    return {r: (note(nm), inst, vol) for r, nm in rows}
go_lead = seq([(0,'A4'),(8,'G4'),(12,'F4'),(16,'G4'),(24,'F4'),(28,'D#4'),(32,'D4')], LEAD, 12)
go_harp = seq([(0,'D5'),(4,'A4'),(8,'F4'),(12,'A4'),(16,'G4'),(20,'D#4'),(24,'G4'),(28,'D#5'),(32,'D4')], HARP, 13)
go_bass = seq([(0,'G2'),(8,'G2'),(16,'D#2'),(24,'D#2'),(32,'D2')], BASS, 12)
go_pats = [(go_lead, go_harp, go_bass)]

# ---- 3: Hi-score (rising fanfare, loops, 2 patterns) ----
H1 = lead_bars(['0 D5 4 F5 8 A5 12 D6', '0 D#5 4 G5 8 A#5 12 D#6', '0 G5 4 A#5 8 D6 12 G5', '0 C#6 4 E6 8 A5 12 C#6'])
H2 = lead_bars(['0 D6 8 A5 12 F5', '0 E5 4 A5 8 C#6 12 E6', '0 D6 8 A5 12 D6', '0 D6'])
prog_H1 = ['Dm', 'Eb', 'Gm', 'A']
prog_H2 = ['Dm', 'A', 'Dm', 'Dm']
hs_pats = [
  pattern(H1, prog_H1, prog_H1, True, 'full'),
  pattern(H2, prog_H2, prog_H2, True, 'full'),
]

subs = re.findall(r'    <aks:subsong>.*?</aks:subsong>\n', src, re.S)
assert len(subs) == 4
new = [subsong_xml('Game', 6, 1, game_pats, subs[0]),
       subsong_xml('Title', 8, 1, title_pats, subs[1]),
       subsong_xml('Game over', 6, 0, go_pats, subs[2], height=48),
       subsong_xml('Hi Score', 6, 0, hs_pats, subs[3])]
out = src
for old, nw in zip(subs, new):
    out = out.replace(old, nw)
open('song_v2.xml', 'w', encoding='utf-8').write(out)
z = zipfile.ZipFile('song_v2.aks', 'w', zipfile.ZIP_DEFLATED); z.write('song_v2.xml', 'song.aks'); z.close()
print('ok')
