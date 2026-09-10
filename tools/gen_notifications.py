#!/usr/bin/env python3
"""Generate the *Notification family (one class per file, game/).

Source: NotificationFactory::create @0x5f5d50 (char-range type ids) plus
each class's getNotificationType/encode/decode addresses (see
docs/REVERSE_STATUS.md). Every subclass calls BaseNotification::encode
first, then its own fields; decodes mirror the encodes.

Run:  python3 tools/gen_notifications.py
Out:  titan/game/<Class>.hpp
"""
import os

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(ROOT, "titan", "game")


def emit(cls, tid, enc, dec, typ, doc, members, enc_body, dec_body):
    if enc == "base":
        head = (f"// {cls}::getNotificationType @{typ} (-> {tid}).\n"
                f"// No dedicated encode/decode symbols in the binary —\n"
                f"// inherits BaseNotification::encode/decode outright\n"
                f"// (factory case verified @0x5f5d50).")
    else:
        head = (f"// {cls}::encode @{enc}, decode @{dec},\n"
                f"// getNotificationType @{typ} (-> {tid}).\n"
                f"// Wire: BaseNotification fields{doc}\n"
                f"// Decode mirrors the same order.")
    body = "#pragma once\n\n" + head + "\n\n"
    body += '#include "titan/game/BaseNotification.hpp"\n\n'
    body += "namespace titan {\n\n"
    body += f"class {cls} : public BaseNotification {{\npublic:\n"
    body += f"    {cls}() {{ type_ = {tid}; }}\n\n"
    body += "    void encode(ByteStream& s) const override {\n" + enc_body + "\n    }\n"
    body += "    void decode(ByteStream& s) override {\n" + dec_body + "\n    }\n"
    body += f"    [[nodiscard]] int notificationType() const override {{ return {tid}; }}\n"
    body += "\n" + members + "\n};\n\n} // namespace titan\n"
    with open(os.path.join(OUT, cls + ".hpp"), "w", encoding="utf-8") as fh:
        fh.write(body)
    print("wrote", cls)


def simple(cls, tid, enc, dec, typ, shape):
    doc = {0: ".", 1: ", then vint @+48.", 2: ", then vint @+48/+52.",
           3: ", then vint @+48/+52/+56.", 5: ", then vint @+48..+64."}[shape]
    if shape == 0:
        members = ""
        enc_b = "        BaseNotification::encode(s);"
        dec_b = "        BaseNotification::decode(s);"
    else:
        names = ["v48_", "v52_", "v56_", "v60_", "v64_"][:shape]
        members = "\n".join(f"    i32 {n} = 0;" for n in names)
        enc_b = "        BaseNotification::encode(s);\n" + "\n".join(
            f"        s.writeVInt({n});" for n in names)
        dec_b = "        BaseNotification::decode(s);\n" + "\n".join(
            f"        {n} = s.readVInt();" for n in names)
    emit(cls, tid, enc, dec, typ, doc, members, enc_b, dec_b)


SIMPLES = [
    # (Class, type, enc, dec, getter, vint_count)
    ("FloaterTextNotification", 66, "base", "base", "0xa4aa50", 0),
    ("QualifyNotification", 76, "base", "base", "0x9acb0c", 0),
    ("VanityItemRewardNotification", 72, "0x648d1c", "0x91b658", "0x9db440", 1),
    ("BrawlPassRewardNotification", 73, "0x8a2a8c", "0x696890", "0x9c0db4", 1),
    ("SkinRewardNotification", 94, "0x72e720", "0x4610b4", "0x9e58bc", 1),
    ("IAPDeliveryNotification", 86, "0x5fca18", "0x2e5f7c", "0xa71144", 1),
    ("StarPointMigrationNotification", 80, "0x3aea2c", "0x6f7070", "0x9ec744", 1),
    ("GemRewardNotification", 89, "0x478458", "0x7bbd24", "0xa2b8a8", 2),
    ("HeroRewardNotification", 93, "0x646f18", "0x5c3bfc", "0xa0c950", 2),
    ("RankRewardNotification", 78, "0x5078f0", "0x8908c0", "0xa0e9c0", 2),
    ("TicketRewardNotification", 91, "0x241ba0", "0x19c014", "0xa2a064", 2),
    ("StarPowerRewardNotification", 84, "0x537d08", "0x250f6c", "0xa1da1c", 2),
    ("BrawlPassPointRewardNotification", 71, "0x667e58", "0x29ed94", "0xa0bd40", 2),
    ("CoinDoublerRewardNotification", 88, "0x6c3a54", "0x63bb74", "0xa29570", 2),
    ("TicketCompensationNotification", 74, "0x70cd68", "0x63f0e8", "0xa0b4c8", 2),
    ("ChallengeSkinRewardNotification", 75, "0x930b20", "0x8648d0", "0x9c7b88", 2),
    ("ResourceRewardNotification", 90, "0x1a10c4", "0x8ba598", "0xa0b03c", 3),
    ("HeroPowerRewardNotification", 92, "0x49edac", "0x1a697c", "0x994544", 3),
    ("BoxRewardNotification", 64, "0x395d38", "0x26c984", "0x9cf79c", 3),
    ("ProLeagueSeasonEndNotification", 77, "0x219370", "0x366de4", "0xa70894", 5),
]

for cls, tid, enc, dec, typ, shape in SIMPLES:
    simple(cls, tid, enc, dec, typ, shape)
print("done:", len(SIMPLES))
