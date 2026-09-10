#!/usr/bin/env python3
"""Emit pending-entry headers for an explicit name list (one file per class)."""
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(ROOT, "include", "titan", "messages", "pending")

NAMES = """ReplayStreamEntry MessageDataStreamEntry QuickChatStreamEntry
TeamCreatedStreamEntry JoinAllianceResponseAvatarStreamEntry
AllianceInvitationAvatarStreamEntry AllianceKickOutStreamEntry
AllianceMailAvatarStreamEntry DonationReceivedStreamEntry DeviceLinkedStreamEntry
BattlePlayerMap ChronosTextEntry ChronosFileEntry LogicRankedSeason HeroEntry
LogicUuid GatchaDrop LogicDailyData LogicConfData""".split()

made = 0
for n in NAMES:
    path = os.path.join(OUT, n + ".hpp")
    if os.path.exists(path):
        continue
    with open(path, "w", encoding="utf-8", newline="\n") as fh:
        fh.write("#pragma once\n\n")
        fh.write("// %s — not yet reversed; encode/decode throw pending_reverse.\n" % n)
        fh.write("\n")
        fh.write('#include "titan/messages/Nested.hpp"\n\n')
        fh.write("namespace titan {\n\n")
        fh.write("TITAN_PENDING_ENTRY(%s);\n\n" % n)
        fh.write("} // namespace titan\n")
    made += 1
print("new pending:", made)
