#!/usr/bin/env python3
"""Generate the compiled function registry from data/ida_shard_*.csv.

Every function known to IDA ends up in the registry with a status:
  Reimplemented      - hand-reversed in src/ (see REIMPLEMENTED below)
  ThirdPartyExternal - vendored lib, replaced by system dep (docs/THIRDPARTY.md)
  Pending            - game code still to reverse

Run:  python tools/generate_registry.py
Out:  include/titan/gen/FunctionRegistry.hpp
      src/gen/Registry_N.cpp (4 translation units, sharded for fast builds)
      src/gen/Registry.cpp   (aggregator: counts + address lookup)
"""
import csv
import glob
import os
import re

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

THIRD_PARTY_MARKERS = [
    "St6__ndk", "icu_58", "hb_", "safe_jni", "yoozoo", "miit", "linecorp",
    "_png_", "png_", "_ZSTD", "ZSTD", "HUF_", "FSE_", "HIST_", "POOL_",
    "XXH", "ZDICT_", "ZSTDMT_", "asio", "_FT_", "FT_", "ft_", "TT_",
    "FMOD", "sentry", "Java_", "gl", "egl", "AAsset", "ANative",
    "OT", "graphite", "titan3com", "ucnv", "uloc", "ures", "uenum",
    "utext", "uiter", "unorm", "ucase", "uprv", "udata", "ucol",
    "ubrk", "ubidi", "uscript", "uset", "ustring", "emutls", "Unwind",
    "_uw_", "TT_RunIns", "cf2_", "CFF_", "cff_", "CID_", "pfr_",
    "otv_", "otl_", "psaux", "pshinter", "psnames", "raster", "smooth",
    "sfnt", "truetype", "type1", "type42", "winfnt", "gzip", "FTC_",
    "FCC_", "af_", "bdf_", "pcf_", "fnt_", "svg_", "wmem", "locale_",
]

# Mangled names of functions already hand-reimplemented in src/.
REIMPLEMENTED = {
    "_ZN15ChecksumEncoder12writeBooleanEb",
    "_ZN15ChecksumEncoder9writeVIntEi",
    "_ZN15ChecksumEncoder8writeIntEi",
    "_ZN15ChecksumEncoder11writeStringEPK6String",
    "_ZN10ByteStream12writeBooleanEb",
    "_ZN10ByteStream8writeIntEi",
    "_ZN10ByteStream9writeVIntEi",
    "_ZN10ByteStream10writeVLongEx",
    "_ZN10ByteStream9readVLongEv",
    "_ZN15ChecksumEncoder10writeVLongEx",
    "_ZN10ByteStream19writeIntToByteArrayEi",
    "_ZN10ByteStream11writeStringEPK6String",
    "_ZN10ByteStream10writeBytesEPci",
    "_ZN10ByteStream11readBooleanEv",
    "_ZN10ByteStream7readIntEv",
    "_ZN10ByteStream8readVIntEv",
    "_ZN14PiranhaMessage6encodeEv",
    "_ZN19ResetAccountMessage6encodeEv",
    "_ZN19ResetAccountMessage6decodeEv",
    "_ZNK19ResetAccountMessage14getMessageTypeEv",
    "_ZN16KeepAliveMessage6encodeEv",
    "_ZN16KeepAliveMessage6decodeEv",
    "_ZNK16KeepAliveMessage14getMessageTypeEv",
    "_ZN22KeepAliveServerMessage6encodeEv",
    "_ZNK22KeepAliveServerMessage14getMessageTypeEv",
}


def module_of(name):
    m = re.match(r"_ZN?K?(?:\d+)([A-Za-z_][A-Za-z0-9_]*)", name)
    if m:
        return m.group(1)[:48]
    if name.startswith("_."):
        return "plt_" + name[2:24]
    if name.startswith("_sub_") or name.startswith("sub_"):
        return "unnamed"
    return "global"


def load_reimplemented_classes():
    path = os.path.join(ROOT, "data", "reimplemented_classes.txt")
    out = set()
    if os.path.exists(path):
        with open(path, encoding="utf-8") as fh:
            for line in fh:
                line = line.strip()
                if line and not line.startswith("#"):
                    out.add(line)
    return out


REIMPLEMENTED_CLASSES = load_reimplemented_classes()


REIMPLEMENTED_METHODS = ("6encodeEv", "6decodeEv", "14getMessageTypeEv",
                        "18getServiceNodeTypeEv", "20getMessageTypeNameEv",
                        "8destructEv")

# Non-message classes: ported method tails (mangled tail startswith).
REIMPLEMENTED_EXTRA = {
    "BitStream": ("8writeIntE", "16writePositiveIntE", "12writeBooleanE",
                  "12readBooleanE", "16writePositiveVIntE", "15readPositiveIntE",
                  "16readPositiveVIntE", "12getByteArrayE", "9getLengthE",
                  "14ensureCapacityE", "8destructE", "C1", "C2", "D0", "D2"),
    "LogicCommandManager": ("13createCommandE", "13encodeCommandE",
                            "13decodeCommandE", "6encodeE", "6decodeE"),
    "GlobalID": ("10getClassIDE", "13getInstanceIDE"),
    "LogicCompressedString": ("6encodeE", "6decodeE", "8compressE",
                              "10decompressE"),
    "LogicDataSlot": ("6encodeE", "6decodeE"),
    "LogicClientAvatar": ("6encodeE", "6decodeE"),
    "PlayerDisplayData": ("6encodeE", "6decodeE"),
    "AllianceHeaderEntry": ("6encodeE", "6decodeE"),
    "AllianceFullEntry": ("6encodeE", "6decodeE"),
    "AllianceMemberEntry": ("6encodeE", "6decodeE"),
    "LogicDailyData": ("6encodeE", "6decodeE"),
    "ForcedDrops": ("6encodeE", "6decodeE"),
    "TimedOffer": ("6encodeE", "6decodeE"),
    "IntValueEntry": ("6encodeE", "6decodeE"),
    "CooldownEntry": ("6encodeE", "6decodeE"),
    "LogicOfferBundle": ("6encodeE", "6decodeE"),
    "LogicGemOffer": ("6encodeE", "6decodeE"),
    "ChronosTextEntry": ("6encodeE", "6decodeE"),
    "AdStatus": ("6encodeE", "6decodeE"),
    "ProLeagueSeasonData": ("6encodeE", "6decodeE"),
    "BrawlPassSeasonData": ("6encodeE", "6decodeE"),
    "LogicQuests": ("6encodeE", "6decodeE"),
    "VanityItems": ("6encodeE", "6decodeE"),
    "LogicBitList": ("6encodeE", "6decodeE"),
    "LogicPlayerRankedSeasonData": ("6encodeE", "6decodeE"),
    "LogicPlayerRewardData": ("6encodeE", "6decodeE"),
    "LogicConfData": ("6encodeE", "6decodeE"),
    "QuestData": ("6encodeE", "6decodeE"),
    "EventSlot": ("6encodeE", "6decodeE"),
    "ReleaseEntry": ("6encodeE", "6decodeE"),
    "TimedIntValueEntry": ("6encodeE", "6decodeE"),
    "CustomEvent": ("6encodeE", "6decodeE"),
    "VanityItemEntry": ("6encodeE", "6decodeE"),
    "VanityItemProp": ("6encodeE", "6decodeE"),
    "LogicCondition": ("6encodeE", "6decodeE"),
    "LogicRewardConfig": ("6encodeE", "6decodeE"),
    "GatchaDrop": ("6encodeE", "6decodeE"),
    "HeroEntry": ("6encodeE", "6decodeE"),
    "LogicPlayer": ("6encodeE", "6decodeE"),
    "LogicHeroUpgrades": ("6encodeE", "6decodeE"),
    "LogicBattleEmotes": ("6encodeE", "6decodeE"),
    "HeroDataEntry": ("6encodeE", "6decodeE"),
    "DeliveryUnit": ("6encodeE", "6decodeE"),
    "LogicMilestoneProgress": ("6encodeE", "6decodeE"),
    "AllianceWarFaction": ("6encodeE", "6decodeE"),
    "AllianceWarNode": ("6encodeE", "6decodeE"),
    "LogicVector2": ("6encodeE", "6decodeE"),
    "XpEntry": ("6encodeE", "6decodeE"),
    "LogicUuid": ("6encodeE", "6decodeE"),
    "PlayerEntry": ("6encodeE", "6decodeE"),
    "LogicRankedSeason": ("6encodeE", "6decodeE"),
    "LogicRankRewardConfig": ("6encodeE", "6decodeE"),
    "LogicBattlePlayerMap": ("6encodeE", "6decodeE"),
    "TeamEntry": ("6encodeE", "6decodeE"),
    "TeamMemberEntry": ("6encodeE", "6decodeE"),
    "TeamInviteEntry": ("6encodeE", "6decodeE"),
    "TeamJoinRequest": ("6encodeE", "6decodeE"),
    "LogicRankedMatch": ("6encodeE", "6decodeE"),
    "LogicRankedMatchPlayer": ("6encodeE", "6decodeE"),
    "RewardEntry": ("6encodeE", "6decodeE"),
    "TeamInvitation": ("6encodeE", "6decodeE"),
    "LogicRankedMatchRoundState": ("6encodeE", "6decodeE"),
    "LogicRankedMatchResultDebugInfo": ("6encodeE", "6decodeE"),
    "TeamInvitationDataEntry": ("6encodeE", "6decodeE"),
    "ChronosFileEntry": ("6encodeE", "6decodeE"),
    "FriendOnlineStatusEntry": ("6encodeE", "6decodeE"),
    "LatencyTestConfiguration": ("6encodeE", "6decodeE"),
    "PlayAgainStatus": ("6encodeE", "6decodeE"),
    "StatusChangeEntry": ("6encodeE", "6decodeE"),
    "LogicTencentAntiAddictionInstruction": ("6encodeE", "6decodeE"),
    "BattleLogPlayerEntry": ("6encodeE", "6decodeE"),
    "BrawlTvChannelInfo": ("6encodeE", "6decodeE"),
    "LatencyData": ("6encodeE", "6decodeE"),
    "LobbyInfoEntry": ("6encodeE", "6decodeE"),
    "StreamEntryFactory": ("24createStreamEntryByTypeE",),
    "AvatarStreamEntryFactory": ("24createStreamEntryByTypeE",),
}


def class_of(name):
    # Returns the class name for _ZN<len><Class>... / _ZNK<len><Class>...
    # (greedy-safe: the length prefix disambiguates).
    m = re.match(r"_ZNK?(\d+)([A-Za-z_][A-Za-z0-9_]*)", name)
    if not m:
        return None
    length = int(m.group(1))
    cls = m.group(2)[:length]
    if len(cls) != length:
        return None
    return cls


def status_of(name):
    if name in REIMPLEMENTED:
        return "Reimplemented"
    cls = class_of(name)
    if cls is not None and cls in REIMPLEMENTED_CLASSES:
        rest = name.split(cls, 1)[1]
        if any(rest.startswith(t) or rest == t for t in REIMPLEMENTED_METHODS):
            return "Reimplemented"
        extra = REIMPLEMENTED_EXTRA.get(cls, ())
        if any(rest.startswith(t) for t in extra):
            return "Reimplemented"
    for marker in THIRD_PARTY_MARKERS:
        if marker in name:
            return "ThirdPartyExternal"
    return "Pending"


def cxx_escape(s):
    return s.replace("\\", "\\\\").replace('"', '\\"')


def main():
    rows = []
    for path in sorted(glob.glob(os.path.join(ROOT, "data", "ida_shard_*.csv"))):
        with open(path, encoding="utf-8") as fh:
            for rec in csv.DictReader(fh):
                rows.append((rec["address"], rec["name"], rec["size"]))
    rows.sort(key=lambda r: int(r[0], 16))

    gen_inc = os.path.join(ROOT, "include", "titan", "gen")
    gen_src = os.path.join(ROOT, "src", "gen")
    os.makedirs(gen_inc, exist_ok=True)
    os.makedirs(gen_src, exist_ok=True)

    with open(os.path.join(gen_inc, "FunctionRegistry.hpp"), "w",
              encoding="utf-8", newline="\n") as fh:
        fh.write("""#pragma once

#include <cstddef>
#include <cstdint>

// Generated by tools/generate_registry.py — do not edit by hand.
// One entry per function known to IDA in libg_decrypted.so.

namespace titan::registry {

enum class Status : std::uint8_t {
    Reimplemented = 0,
    ThirdPartyExternal = 1,
    Pending = 2,
};

struct Entry {
    std::uint32_t address; // IDA virtual address
    Status status;
    const char* name;      // mangled IDA name (may be "" if unnamed)
    const char* module;    // coarse class/library bucket
};

struct Counts {
    std::size_t total = 0;
    std::size_t reimplemented = 0;
    std::size_t thirdParty = 0;
    std::size_t pending = 0;
};

const Entry* entries(std::size_t* outCount);
Counts counts();
const Entry* findByAddress(std::uint32_t address);

} // namespace titan::registry
""")

    # Split into 4 TUs.
    n = 4
    chunk = (len(rows) + n - 1) // n
    part_names = []
    for i in range(n):
        part = rows[i * chunk:(i + 1) * chunk]
        fname = f"RegistryPart{i}.cpp"
        part_names.append(fname)
        with open(os.path.join(gen_src, fname), "w", encoding="utf-8",
                  newline="\n") as fh:
            fh.write("// Generated — do not edit. Part %d/%d (%d entries).\n"
                     % (i + 1, n, len(part)))
            fh.write('#include "titan/gen/FunctionRegistry.hpp"\n\n'
                     "namespace titan::registry::detail {\n\n")
            fh.write("extern const Entry kPart%d[] = {\n" % i)
            for addr, name, _size in part:
                st = status_of(name)
                fh.write('    {0x%07X, Status::%s, "%s", "%s"},\n'
                         % (int(addr, 16), st, cxx_escape(name),
                            cxx_escape(module_of(name))))
            fh.write("};\n")
            fh.write("extern const std::size_t kPart%dCount = %d;\n"
                     % (i, len(part)))
            fh.write("\n} // namespace titan::registry::detail\n")

    with open(os.path.join(gen_src, "Registry.cpp"), "w", encoding="utf-8",
              newline="\n") as fh:
        fh.write("// Generated — do not edit. Registry aggregator.\n")
        fh.write('#include "titan/gen/FunctionRegistry.hpp"\n\n'
                 "namespace titan::registry {\n\n"
                 "namespace detail {\n")
        for i in range(n):
            fh.write("extern const Entry kPart%d[];\n" % i)
            fh.write("extern const std::size_t kPart%dCount;\n" % i)
        fh.write("} // namespace detail\n\n")
        fh.write("namespace {\nstruct Part { const Entry* e; std::size_t n; };\n"
                 "const Part kParts[] = {\n")
        for i in range(n):
            fh.write("    {detail::kPart%d, detail::kPart%dCount},\n" % (i, i))
        fh.write("};\n} // namespace\n\n")
        fh.write("""const Entry* entries(std::size_t* outCount) {
    // Parts are individually sorted; the caller iterates all parts.
    // For simplicity expose part 0 here and let counts()/find cover all.
    // (Full flattened view is unnecessary for status queries.)
    if (outCount) *outCount = 0;
    for (const Part& p : kParts) *outCount += p.n;
    return kParts[0].e;
}

Counts counts() {
    Counts c;
    for (const Part& p : kParts) {
        for (std::size_t i = 0; i < p.n; ++i) {
            ++c.total;
            switch (p.e[i].status) {
                case Status::Reimplemented: ++c.reimplemented; break;
                case Status::ThirdPartyExternal: ++c.thirdParty; break;
                case Status::Pending: ++c.pending; break;
            }
        }
    }
    return c;
}

const Entry* findByAddress(std::uint32_t address) {
    for (const Part& p : kParts) {
        std::size_t lo = 0, hi = p.n;
        while (lo < hi) {
            const std::size_t mid = lo + (hi - lo) / 2;
            if (p.e[mid].address < address) lo = mid + 1;
            else hi = mid;
        }
        if (lo < p.n && p.e[lo].address == address) return &p.e[lo];
    }
    return nullptr;
}

} // namespace titan::registry
""")

    from collections import Counter
    c = Counter(status_of(nm) for _a, nm, _s in rows)
    print(f"functions: {len(rows)} "
          f"reimplemented={c['Reimplemented']} "
          f"thirdparty={c['ThirdPartyExternal']} pending={c['Pending']}")


if __name__ == "__main__":
    main()
