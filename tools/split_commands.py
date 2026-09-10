#!/usr/bin/env python3
"""Split CmdBatchA/B.hpp into one file per command class.

- Expands TITAN_CMD_VINT/EMPTY/DATAREF macro uses into full classes.
- Drops TITAN_PENDING_ENTRY lines (now in messages/pending/*.hpp).
- Each `class X ... };` becomes commands/X.hpp.
- Out-of-line bodies in CmdBatchA.cpp become commands/X.cpp.
- Emits commands/AllCommands.hpp.
- Deletes the CmdBatch files afterwards (only with --apply).
"""
import glob
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CMD = os.path.join(ROOT, "include", "titan", "commands")
SRC = os.path.join(ROOT, "src", "commands")

APPLY = "--apply" in sys.argv

VINT_TMPL = """// ---- %(id)d %(cls)s: vint ----
// Split out of CmdBatchB.hpp; wire format unchanged.
class %(cls)s : public LogicCommand {
public:
    int getCommandType() const override { return %(id)d; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(v_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v_ = s.readVInt();
    }
    i32 v_ = 0;
};"""

EMPTY_TMPL = """// ---- %(id)d %(cls)s: empty ----
// Split out of CmdBatchB.hpp; wire format unchanged.
class %(cls)s : public LogicCommand {
public:
    int getCommandType() const override { return %(id)d; }
};"""

DATAREF_TMPL = """// ---- %(id)d %(cls)s: dataref ----
// Split out of CmdBatchB.hpp; wire format unchanged.
class %(cls)s : public LogicCommand {
public:
    int getCommandType() const override { return %(id)d; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        DataReference::encodeNullable(s, ref_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        ref_ = DataReference::decodeNullable(s);
    }
    std::optional<DataReference> ref_;
};"""


def expand_macros(text):
    def rep_vint(m):
        return VINT_TMPL % {"cls": m.group(1), "id": int(m.group(2))}
    def rep_empty(m):
        return EMPTY_TMPL % {"cls": m.group(1), "id": int(m.group(2))}
    def rep_dataref(m):
        return DATAREF_TMPL % {"cls": m.group(1), "id": int(m.group(2))}
    text = re.sub(r"TITAN_CMD_VINT\((\w+),\s*(\d+),\s*\w+\)\s*;", rep_vint, text)
    text = re.sub(r"TITAN_CMD_EMPTY\((\w+),\s*(\d+)\)\s*;", rep_empty, text)
    text = re.sub(r"TITAN_CMD_DATAREF\((\w+),\s*(\d+),\s*\w+\)\s*;", rep_dataref, text)
    # Strip multi-line #define blocks line-wise (they end at the first
    # line without a trailing backslash), then the trailing #undefs.
    lines = []
    skipping = False
    for line in text.splitlines(keepends=True):
        s = line.strip()
        if s.startswith("#define TITAN_CMD_"):
            skipping = True
        if skipping:
            if not line.rstrip("\n").rstrip().endswith("\\"):
                skipping = False
            continue
        if re.match(r"#undef TITAN_CMD_(VINT|EMPTY|DATAREF)\b", s):
            continue
        lines.append(line)
    return "".join(lines)


def strip_pending(text):
    return re.sub(r"^\s*TITAN_PENDING_ENTRY\(\w+\);\s*$", "", text, flags=re.M)


def split_classes(text):
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


PRELUDE = """#pragma once

// %(cls)s (%(id)s) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.hpp"
#include "titan/core/DataReference.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

%(block)s

} // namespace titan
"""


def cmd_id(block):
    m = re.search(r"return (\d+);", block)
    return m.group(1) if m else "?"


def main():
    all_classes = []
    for path in sorted(glob.glob(os.path.join(CMD, "CmdBatch*.hpp"))):
        with open(path, encoding="utf-8") as fh:
            text = fh.read()
        text = expand_macros(text)
        text = strip_pending(text)
        for cls, block in split_classes(text):
            if cls == "LogicCommand":
                continue
            with open(os.path.join(CMD, cls + ".hpp"), "w",
                      encoding="utf-8", newline="\n") as fh:
                fh.write(PRELUDE % {"cls": cls, "id": cmd_id(block),
                                    "block": block.strip()})
            all_classes.append(cls)
        print(os.path.basename(path), "->",
              len([c for c, _ in split_classes(text) if c != "LogicCommand"]))
    # Bodies.
    for path in sorted(glob.glob(os.path.join(SRC, "CmdBatch*.cpp"))):
        with open(path, encoding="utf-8") as fh:
            text = fh.read()
        pat = re.compile(r"^[\w:<>~]+\s+(\w+)::(\w+)\s*\(", flags=re.M)
        marks = [(m.start(), m.group(1)) for m in pat.finditer(text)]
        for idx, (pos, cls) in enumerate(marks):
            end = marks[idx + 1][0] if idx + 1 < len(marks) else len(text)
            block = text[pos:end].rstrip() + "\n"
            block = re.sub(r"\n\}\s*//\s*namespace[^\n]*\s*$", "\n", block)
            with open(os.path.join(SRC, cls + ".cpp"), "w",
                      encoding="utf-8", newline="\n") as fh:
                fh.write("// %s bodies — split out; wire format unchanged.\n\n" % cls)
                fh.write('#include "titan/commands/%s.hpp"\n\n' % cls)
                fh.write("namespace titan {\n\n" + block + "\n} // namespace titan\n")
        print(os.path.basename(path), "->", len(marks))
    with open(os.path.join(CMD, "AllCommands.hpp"), "w",
              encoding="utf-8", newline="\n") as fh:
        fh.write("#pragma once\n\n// Generated by tools/split_commands.py — all commands.\n\n")
        for cls in sorted(all_classes):
            fh.write('#include "titan/commands/%s.hpp"\n' % cls)
    print("total command classes:", len(all_classes))
    if APPLY:
        for path in glob.glob(os.path.join(CMD, "CmdBatch*.hpp")):
            os.remove(path)
        for path in glob.glob(os.path.join(SRC, "CmdBatch*.cpp")):
            os.remove(path)
        print("batch files deleted")


if __name__ == "__main__":
    main()
