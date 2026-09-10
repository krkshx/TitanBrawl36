#!/usr/bin/env python3
"""Split MsgBatchNN.hpp/.cpp into one file per message class.

- Shared bases/derived classes (now in Common.hpp / per-class files) and
  TITAN_PENDING_ENTRY lines (now in pending/*.hpp) are stripped.
- Each remaining top-level `class X ... };` becomes messages/X.hpp.
- Each `... X::method ... {...}` block in the .cpp becomes messages/X.cpp
  (file-static helpers are relocated per HELPER_MOVE).
- Emits messages/AllMessages.hpp (used by tests/factory).
- Deletes the MsgBatch files afterwards (only when --apply is passed).
"""
import glob
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
MSG = os.path.join(ROOT, "include", "titan", "messages")
SRC = os.path.join(ROOT, "src", "messages")

APPLY = "--apply" in sys.argv

# Classes that already live elsewhere (do not emit; strip their blocks).
SKIP = {
    "FriendAvatarBase",          # struct -> Common.hpp
    "AllianceSettingsBase",      # Common.hpp
    "PlayingFriendsQuery",       # Common.hpp
    "BindAccountMessage",        # Common.hpp
    # Family-derived (tools/emit_derived.py outputs):
    "AskForPlayingFacebookFriendsMessage", "AskForPlayingGamecenterFriendsMessage",
    "AskForPlayingKakaoFriendsMessage", "AskForPlayingLineFriendsMessage",
    "AskForPlayingSupercellFriendsMessage", "AskForPlayingTencentFriendsMessage",
    "BindFacebookAccountMessage", "BindGamecenterAccountMessage",
    "BindGoogleServiceAccountMessage", "BindKakaoAccountMessage",
    "BindLineAccountMessage", "BindTencentAccountMessage", "BindYoozooAccountMessage",
    "ChangeAllianceSettingsMessage", "CreateAllianceMessage",
}

# File-static helpers relocated into the listed class .cpp (else dropped
# because they now live in Common.hpp).
HELPER_MOVE = {
    "writeIdList": "AskForAddableFriendsMessage",
    "readIdList": "AskForAddableFriendsMessage",
    "readCappedStringList": "LoginOkMessage",
}
# Helpers that now live in Common.hpp — strip from batch cpps.
HELPER_DROP = {
    "encodeEntryArray", "decodeEntryArray",
    "encodeOptionalPlayer", "decodeOptionalPlayer",
    "encodeCompressed", "decodeCompressed",
    "encodeIntList", "decodeIntList",
}

PENDING = sorted(os.path.splitext(os.path.basename(p))[0]
                 for p in glob.glob(os.path.join(MSG, "pending", "*.hpp")))


def strip_macros(text):
    for macro in ("TITAN_PLAYING_FRIENDS", "TITAN_BIND"):
        text = re.sub(r"#define %s\b.*?#undef %s[^\n]*\n?" % (macro, macro),
                      "", text, flags=re.S)
    text = re.sub(r"^\s*TITAN_(PLAYING_FRIENDS|BIND)\([\w,\s]+\)\s*;\s*$",
                  "", text, flags=re.M)
    return text


def strip_pending_lines(text):
    return re.sub(r"^\s*TITAN_PENDING_ENTRY\(\w+\);\s*$", "", text, flags=re.M)


def strip_struct(text, name):
    return re.sub(r"struct %s \{.*?\n\};" % name, "", text, flags=re.S)


def split_classes(text):
    """Yield (classname, block) for top-level `class X ... };` blocks."""
    out = []
    for m in re.finditer(r"^class (\w+)(?:\s*:\s*public[^\{]*)?\{", text, flags=re.M):
        cls = m.group(1)
        depth = 0
        i = m.end() - 1
        while i < len(text):
            if text[i] == "{":
                depth += 1
            elif text[i] == "}":
                depth -= 1
                if depth == 0:
                    j = text.find(";", i)
                    out.append((cls, text[m.start():j + 1]))
                    break
            i += 1
    return out


def class_id(block):
    m = re.search(r"return (\d+);", block)
    return m.group(1) if m else "?"


def extra_includes(block):
    inc = []
    for p in PENDING:
        if re.search(r"\b%s\b" % p, block):
            inc.append('#include "titan/messages/pending/%s.hpp"' % p)
    if re.search(r"\bLogicCommand\w*\b", block):
        inc.append('#include "titan/commands/LogicCommand.hpp"')
        inc.append('#include "titan/gen/LogicCommands.hpp"')
    return inc


def emit_header(cls, block, batch):
    lines = ["#pragma once", "",
             "// %s (%s) — split out of %s; wire format unchanged." %
             (cls, class_id(block), batch),
             "", '#include "titan/messages/MessagePrelude.hpp"']
    lines += extra_includes(block)
    lines += ["", "namespace titan {", "", block.strip(), "",
              "} // namespace titan", ""]
    with open(os.path.join(MSG, cls + ".hpp"), "w",
              encoding="utf-8", newline="\n") as fh:
        fh.write("\n".join(lines))


def split_methods(text):
    """Map class -> list of out-of-line member definition blocks."""
    methods = {}
    pat = re.compile(r"^[\w:<>~]+\s+(\w+)::(\w+)\s*\(", flags=re.M)
    marks = [(m.start(), m.group(1)) for m in pat.finditer(text)]
    for idx, (pos, cls) in enumerate(marks):
        end = marks[idx + 1][0] if idx + 1 < len(marks) else len(text)
        block = text[pos:end].rstrip() + "\n"
        # The last method swallows the file's trailing namespace close.
        block = re.sub(r"\n\}\s*//\s*namespace[^\n]*\s*$", "\n", block)
        methods.setdefault(cls, []).append(block)
    return methods


def helper_blocks(text):
    """Map helper-name -> static/free function block preceding class methods."""
    found = {}
    for name in list(HELPER_MOVE) + list(HELPER_DROP):
        for m in re.finditer(
                r"^(?:static\s+)?[\w:<>~&*]+\s+%s\s*\(" % name, text, flags=re.M):
            depth = 0
            started = False
            i = m.start()
            while i < len(text):
                if text[i] == "{":
                    depth += 1
                    started = True
                elif text[i] == "}":
                    depth -= 1
                    if started and depth == 0:
                        found[name] = text[m.start():i + 1] + "\n"
                        break
                i += 1
    return found


def process_header(path):
    batch = os.path.basename(path)
    with open(path, encoding="utf-8") as fh:
        text = fh.read()
    text = strip_macros(text)
    text = strip_pending_lines(text)
    text = strip_struct(text, "FriendAvatarBase")
    # Drop whole skipped class blocks.
    classes = [(c, b) for c, b in split_classes(text) if c not in SKIP]
    skipped = [c for c, _ in split_classes(text) if c in SKIP]
    emitted = []
    for cls, block in classes:
        emit_header(cls, block, batch)
        emitted.append(cls)
    return emitted, skipped


def process_cpp(path, header_classes):
    batch = os.path.basename(path)
    with open(path, encoding="utf-8") as fh:
        text = fh.read()
    helpers = helper_blocks(text)
    # Remove helpers FIRST: mid-file helpers would otherwise be swallowed
    # into the preceding method block by split_methods slicing.
    notext = text
    for name, block in helpers.items():
        notext = notext.replace(block, "")
    methods = split_methods(notext)
    # Leftover check against fully stripped text.
    text = notext
    for cls, blocks in methods.items():
        for b in blocks:
            text = text.replace(b, "")
    emitted = []
    for cls, blocks in methods.items():
        if cls in SKIP:
            continue
        lines = ["// %s bodies — split out of %s; wire format unchanged." % (cls, batch),
                 "", '#include "titan/messages/%s.hpp"' % cls, "",
                 "namespace titan {", ""]
        target = None
        for name, dest in HELPER_MOVE.items():
            if dest == cls and name in helpers:
                target = dest
        if target:
            for name, dest in HELPER_MOVE.items():
                if dest == target and name in helpers:
                    lines.append(helpers[name])
        lines.extend(blocks)
        lines.append("} // namespace titan\n")
        with open(os.path.join(SRC, cls + ".cpp"), "w",
                  encoding="utf-8", newline="\n") as fh:
            fh.write("\n".join(lines))
        emitted.append(cls)
    # Warn about leftover non-trivial content.
    rest = [l for l in text.splitlines()
            if l.strip() and not l.strip().startswith("//")
            and '#include' not in l and 'namespace titan' not in l
            and l.strip() not in ("{", "}")]
    dropped_helpers = [n for n in helpers if n in HELPER_DROP]
    return emitted, dropped_helpers, rest


def main():
    all_classes = []
    for path in sorted(glob.glob(os.path.join(MSG, "MsgBatch*.hpp"))):
        emitted, skipped = process_header(path)
        all_classes += emitted
        print(os.path.basename(path), "->", len(emitted), "skipped:", skipped)
    for path in sorted(glob.glob(os.path.join(SRC, "MsgBatch*.cpp"))):
        emitted, dropped, rest = process_cpp(path, all_classes)
        print(os.path.basename(path), "->", len(emitted), "dropped helpers:", dropped)
        if rest:
            print("  LEFTOVER:", rest[:6])
    with open(os.path.join(MSG, "AllMessages.hpp"), "w",
              encoding="utf-8", newline="\n") as fh:
        fh.write("#pragma once\n\n// Generated by tools/split_messages.py — all messages.\n\n")
        for cls in sorted(all_classes):
            fh.write('#include "titan/messages/%s.hpp"\n' % cls)
    print("total message classes:", len(all_classes))
    if APPLY:
        for path in glob.glob(os.path.join(MSG, "MsgBatch*.hpp")):
            os.remove(path)
        for path in glob.glob(os.path.join(SRC, "MsgBatch*.cpp")):
            os.remove(path)
        print("batch files deleted")


if __name__ == "__main__":
    main()
