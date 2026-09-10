#!/usr/bin/env python3
"""Drop stray `template <...>` header lines left in split per-class cpps."""
import glob
import os
import re

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

fixed = 0
for path in (glob.glob(os.path.join(ROOT, "src", "messages", "*.cpp"))
             + glob.glob(os.path.join(ROOT, "src", "commands", "*.cpp"))):
    with open(path, encoding="utf-8") as fh:
        lines = fh.readlines()
    kept = [l for l in lines if not re.match(r"^\s*template\s*<.*>\s*$", l)]
    if len(kept) != len(lines):
        with open(path, "w", encoding="utf-8", newline="\n") as fh:
            fh.writelines(kept)
        fixed += 1
print("fixed:", fixed)
