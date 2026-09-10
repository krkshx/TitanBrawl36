#pragma once

// LogicJSONParser — recursive-descent parser + serializer.
//
// Grammar (verified against parseValue @0x8909a4 and friends):
//   - BOM: a leading U+FEFF is skipped (parse @0x1d083c).
//   - Whitespace: codepoints <= 32 are skipped between tokens.
//   - '"…"': string (escapes via parseEscapedString @0x42c37c).
//   - '-', '0'-'9': number — integer digits accumulate exactly (i64);
//     on '.', 'e', 'E' the token re-parses through float precision
//     (parseNumber @0x7c35ec; binary stores float).
//   - '[' / '{': array / object (empty forms allowed); elements/entries
//     separated by ',', failure anywhere -> null (+Debugger::warning).
//   - 'f'/'t': true/false literals; 'n': null literal.
//   - Anything else -> null.
// Serialization (createJSONString @0x595724 dispatches writeToString):
// compact, no spaces; strings use the standard 8-escape set
// (writeString @0x614418: \b \t \n \f \r \" \/ \\).

#include "titan/game/LogicJSONNode.hpp"

#include <memory>
#include <string>

namespace titan {

class LogicJSONParser {
public:
    // parse @0x1d083c / parseJSONNode @0x59fa54: BOM skip, then parseValue.
    // Returns nullptr on any syntax error (binary warns + nulls).
    [[nodiscard]] static std::unique_ptr<LogicJSONNode> parse(const std::string& text);
    // createJSONString @0x595724: serialize any node.
    [[nodiscard]] static std::string stringify(const LogicJSONNode& node);
    // writeString @0x614418: quoted + escaped append.
    static void writeString(const std::string& value, std::string& out);
};

} // namespace titan
