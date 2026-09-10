#!/usr/bin/env python3
"""Rewrite avatar-scope pending entries as AvatarStreamEntry subclasses."""
import os

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(ROOT, "include", "titan", "messages", "pending")

NAMES = """BattleReportStreamEntry JoinAllianceResponseAvatarStreamEntry
AllianceInvitationAvatarStreamEntry AllianceKickOutStreamEntry
AllianceMailAvatarStreamEntry DonationReceivedStreamEntry
DeviceLinkedStreamEntry""".split()

for n in NAMES:
    with open(os.path.join(OUT, n + ".hpp"), "w", encoding="utf-8", newline="\n") as fh:
        fh.write("#pragma once\n\n")
        fh.write("// %s — avatar-scope stream entry, not yet reversed.\n" % n)
        fh.write("// Derives AvatarStreamEntry so the avatar factory typechecks;\n")
        fh.write("// encode/decode throw pending_reverse until reversed.\n\n")
        fh.write('#include "titan/game/AvatarStreamEntry.hpp"\n\n')
        fh.write("namespace titan {\n\n")
        fh.write("class %s : public AvatarStreamEntry {\n" % n)
        fh.write("public:\n")
        fh.write("    void encode(ByteStream&) const override {\n")
        fh.write('        throw pending_reverse("%s pending reverse");\n' % n)
        fh.write("    }\n")
        fh.write("    void decode(ByteStream&) override {\n")
        fh.write('        throw pending_reverse("%s pending reverse");\n' % n)
        fh.write("    }\n")
        fh.write("};\n\n} // namespace titan\n")
print("rewrote:", len(NAMES))
