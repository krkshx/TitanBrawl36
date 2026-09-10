#!/usr/bin/env python3
"""Repair split per-class .cpps: helpers into namespace, single close."""
import glob
import os
import re

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

fixed = 0
for path in (glob.glob(os.path.join(ROOT, "src", "messages", "*.cpp"))
             + glob.glob(os.path.join(ROOT, "src", "commands", "*.cpp"))):
    with open(path, encoding="utf-8") as fh:
        text = fh.read()
    orig = text
    # Drop stray section comments.
    text = re.sub(r"^// ---- .*$", "", text, flags=re.M)
    # Collapse duplicate trailing namespace closes.
    text = re.sub(r"(\n\} // namespace titan\n)(\n\} // namespace titan\n)+",
                  r"\1", text)
    lines = text.splitlines(keepends=True)
    # Find namespace open; move file-static helper defs inside it.
    try:
        ns = next(i for i, l in enumerate(lines) if l.startswith("namespace titan {"))
    except StopIteration:
        continue
    head = lines[:ns]
    body = lines[ns:]
    # Helpers = top-level function definitions in head (after includes).
    helpers = []
    keep_head = []
    i = 0
    while i < len(head):
        if re.match(r"^(?:static\s+)?[\w:<>~&*]+\s+\w+\s*\(", head[i]):
            depth = 0
            started = False
            j = i
            while j < len(head):
                if head[j].count("{"):
                    depth += head[j].count("{")
                    started = True
                if head[j].count("}"):
                    depth -= head[j].count("}")
                    if started and depth == 0:
                        helpers.append("".join(head[i:j + 1]))
                        i = j + 1
                        break
                j += 1
            else:
                keep_head.append(head[i])
                i += 1
        else:
            keep_head.append(head[i])
            i += 1
    if helpers:
        body = [body[0], "\n"] + helpers + ["\n"] + body[1:]
        text = "".join(keep_head) + "".join(body)
    # Collapse 3+ blank lines.
    text = re.sub(r"\n{4,}", "\n\n\n", text)
    if text != orig:
        with open(path, "w", encoding="utf-8", newline="\n") as fh:
            fh.write(text)
        fixed += 1
print("fixed:", fixed)
