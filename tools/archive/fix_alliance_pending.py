#!/usr/bin/env python3
"""Rewrite alliance-scope pending entries as StreamEntry subclasses."""
import os

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(ROOT, "include", "titan", "messages", "pending")

NAMES = """ReplayStreamEntry MessageDataStreamEntry QuickChatStreamEntry
TeamCreatedStreamEntry""".split()

for n in NAMES:
    with open(os.path.join(OUT, n + ".hpp"), "w", encoding="utf-8", newline="\n") as fh:
        fh.write("#pragma once\n\n")
        fh.write("// %s — alliance-scope stream entry, not yet reversed.\n" % n)
        fh.write("// Derives StreamEntry so the alliance factory typechecks;\n")
        fh.write("// encode/decode throw pending_reverse until reversed.\n\n")
        fh.write('#include "titan/game/StreamEntry.hpp"\n\n')
        fh.write("namespace titan {\n\n")
        fh.write("class %s : public StreamEntry {\n" % n)
        fh.write("public:\n")
        fh.write("    void encode(ByteStream&) const override {\n")
        fh.write('        throw pending_reverse("%s pending reverse");\n' % n)
        fh.write("    }\n")
        fh.write("    void decode(ByteStream&) override {\n")
        fh.write('        throw pending_reverse("%s pending reverse");\n' % n)
        fh.write("    }\n")
        fh.write("};\n\n} // namespace titan\n")
print("rewrote:", len(NAMES))
