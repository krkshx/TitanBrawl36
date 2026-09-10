#!/usr/bin/env python3
"""Emit one pending-entry header per TITAN_PENDING_ENTRY name."""
import glob
import os
import re

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

names = set()
files = (glob.glob(os.path.join(ROOT, "include", "titan", "messages", "MsgBatch*.hpp"))
         + glob.glob(os.path.join(ROOT, "include", "titan", "commands", "Cmd*.hpp")))
for f in files:
    with open(f, encoding="utf-8") as fh:
        for line in fh:
            m = re.match(r"TITAN_PENDING_ENTRY\((\w+)\);", line.strip())
            if m:
                names.add(m.group(1))

outdir = os.path.join(ROOT, "include", "titan", "messages", "pending")
os.makedirs(outdir, exist_ok=True)
for n in sorted(names):
    path = os.path.join(outdir, n + ".hpp")
    with open(path, "w", encoding="utf-8", newline="\n") as fh:
        fh.write("#pragma once\n\n")
        fh.write("// %s — not yet reversed; encode/decode throw pending_reverse.\n" % n)
        fh.write("// Replaces the TITAN_PENDING_ENTRY(%s) placeholder.\n\n" % n)
        fh.write('#include "titan/messages/Nested.hpp"\n\n')
        fh.write("namespace titan {\n\n")
        fh.write("TITAN_PENDING_ENTRY(%s);\n\n" % n)
        fh.write("} // namespace titan\n")
print("pending entries:", len(names))
