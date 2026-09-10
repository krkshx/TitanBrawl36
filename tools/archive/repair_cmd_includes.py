#!/usr/bin/env python3
"""Add missing pending-entry includes to per-class command headers."""
import glob
import os
import re

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PEND = os.path.join(ROOT, "include", "titan", "messages", "pending")

pending = sorted(os.path.splitext(os.path.basename(p))[0]
                 for p in glob.glob(os.path.join(PEND, "*.hpp")))

fixed = 0
for path in glob.glob(os.path.join(ROOT, "include", "titan", "commands", "*.hpp")):
    if os.path.basename(path) in ("AllCommands.hpp", "LogicCommand.hpp"):
        continue
    with open(path, encoding="utf-8") as fh:
        text = fh.read()
    need = ['#include "titan/messages/pending/' + p + '.hpp"'
            for p in pending if re.search(r"\b%s\b" % p, text)]
    need = [n for n in need if n not in text]
    if need:
        anchor = '#include "titan/commands/LogicCommand.hpp"'
        ins = anchor + "\n" + "\n".join(need)
        text = text.replace(anchor, ins, 1)
        with open(path, "w", encoding="utf-8", newline="\n") as fh:
            fh.write(text)
        fixed += 1
print("fixed:", fixed)
