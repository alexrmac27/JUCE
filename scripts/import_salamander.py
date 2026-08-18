# Salamander import script
# Scans a directory of WAVs (Salamander-style names) and emits a JSON manifest
# Usage: python import_salamander.py /path/to/salamander_wavs /path/to/out/manifest.json

import sys
import os
import re
import json

NOTE_NAME_TO_SEMITONE = {
    'C':0,'D':2,'E':4,'F':5,'G':7,'A':9,'B':11
}

note_re = re.compile(r'([A-Ga-g])([#b]?)(-?\d)')


def note_name_to_midi(name):
    # name e.g. C4, C#4, Db3
    m = re.match(r'^([A-Ga-g])([#b]?)(-?\d+)$', name)
    if not m:
        return None
    letter = m.group(1).upper()
    accidental = m.group(2)
    octave = int(m.group(3))
    semitone = NOTE_NAME_TO_SEMITONE.get(letter, 0)
    if accidental == '#': semitone += 1
    if accidental == 'b' or accidental == 'B': semitone -= 1
    midi = (octave + 1) * 12 + semitone
    return max(0, min(127, midi))


def infer_note_from_filename(fn):
    # try to find matches like C4 or C#4 or Db3
    m = re.search(r'([A-Ga-g])([#b]?)(-?\d+)', fn)
    if not m:
        return None
    return note_name_to_midi(m.group(1) + m.group(2) + m.group(3))


def build_manifest(src_dir, out_path):
    files = [f for f in os.listdir(src_dir) if f.lower().endswith('.wav')]
    files.sort()
    manifest = []
    for f in files:
        midi = infer_note_from_filename(f)
        if midi is None:
            midi = 60
        entry = {
            'file': f,
            'root': int(midi),
            'lo': int(midi),
            'hi': int(midi),
            'vel_lo': 1,
            'vel_hi': 127
        }
        manifest.append(entry)

    with open(out_path, 'w', encoding='utf-8') as of:
        json.dump(manifest, of, indent=2)
    print(f'Wrote manifest with {len(manifest)} entries to {out_path}')


if __name__ == '__main__':
    if len(sys.argv) < 3:
        print('Usage: import_salamander.py /path/to/wavs /path/to/manifest.json')
        sys.exit(1)
    src = sys.argv[1]
    out = sys.argv[2]
    if not os.path.isdir(src):
        print('Source directory not found:', src)
        sys.exit(1)
    build_manifest(src, out)
