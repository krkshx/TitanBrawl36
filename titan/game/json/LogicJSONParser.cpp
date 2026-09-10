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

#include "titan/game/json/LogicJSONNode.cpp"

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

// LogicJSONParser bodies — grammar verified against the binary (see header).


#include "titan/game/json/LogicJSONArray.cpp"
#include "titan/game/json/LogicJSONBoolean.cpp"
#include "titan/game/json/LogicJSONNull.cpp"
#include "titan/game/json/LogicJSONNumber.cpp"
#include "titan/game/json/LogicJSONObject.cpp"
#include "titan/game/json/LogicJSONString.cpp"

#include <cstdio>
#include <cstring>

namespace titan {

namespace {
// Cursor over UTF-8 bytes, tracking a codepoint index like the binary's
// String cursor (rawNextCharAscii advances per byte here; multibyte
// sequences pass through as raw bytes, matching appendCodePoint).
class Cursor {
public:
    explicit Cursor(const std::string& text) : text_(text) {}

    // Skip codepoints <= 32 (binary whitespace rule). Returns false on end.
    bool skipSpace() {
        while (pos_ < text_.size()
               && static_cast<unsigned char>(text_[pos_]) <= 32) {
            ++pos_;
        }
        return pos_ < text_.size();
    }
    // Next raw byte, or -1 at end.
    int next() {
        if (pos_ >= text_.size()) return -1;
        return static_cast<unsigned char>(text_[pos_++]);
    }
    // Step one byte back (the binary rewinds m_length by one after peek).
    void back() {
        if (pos_ > 0) --pos_;
    }
    // Consume one byte (caller checked bounds).
    void advance() { ++pos_; }
    [[nodiscard]] std::size_t pos() const { return pos_; }
    [[nodiscard]] const std::string& text() const { return text_; }

private:
    const std::string& text_;
    std::size_t pos_ = 0;
};

std::unique_ptr<LogicJSONNode> parseValue(Cursor& cur);

// parseStringValue @0x7f9e2c + parseEscapedString @0x42c37c: expects the
// opening quote already consumed... here: consumes '"', reads until the
// closing quote, handling \", \\, \/, \b, \f, \n, \r, \t, \uXXXX.
std::unique_ptr<std::string> parseString(Cursor& cur) {
    if (!cur.skipSpace()) return nullptr;
    if (cur.next() != '"') return nullptr;
    std::string out;
    for (;;) {
        if (cur.pos() >= cur.text().size()) return nullptr;
        const unsigned char c = static_cast<unsigned char>(cur.text()[cur.pos()]);
        cur.advance();
        if (c == '"') return std::make_unique<std::string>(out);
        if (c != '\\') {
            out += static_cast<char>(c);
            continue;
        }
        if (cur.pos() >= cur.text().size()) return nullptr;
        const char e = cur.text()[cur.pos()];
        cur.advance();
        switch (e) {
            case '"': out += '"'; break;
            case '\\': out += '\\'; break;
            case '/': out += '/'; break;
            case 'b': out += '\b'; break;
            case 'f': out += '\f'; break;
            case 'n': out += '\n'; break;
            case 'r': out += '\r'; break;
            case 't': out += '\t'; break;
            case 'u': {
                if (cur.pos() + 4 > cur.text().size()) return nullptr;
                unsigned code = 0;
                for (int i = 0; i < 4; ++i) {
                    const char h = cur.text()[cur.pos()];
                    cur.advance();
                    code <<= 4;
                    if (h >= '0' && h <= '9') code |= static_cast<unsigned>(h - '0');
                    else if (h >= 'a' && h <= 'f') code |= static_cast<unsigned>(h - 'a' + 10);
                    else if (h >= 'A' && h <= 'F') code |= static_cast<unsigned>(h - 'A' + 10);
                    else return nullptr;
                }
                // Encode as UTF-8 (binary appends the codepoint).
                if (code < 0x80) {
                    out += static_cast<char>(code);
                } else if (code < 0x800) {
                    out += static_cast<char>(0xC0 | (code >> 6));
                    out += static_cast<char>(0x80 | (code & 0x3F));
                } else {
                    out += static_cast<char>(0xE0 | (code >> 12));
                    out += static_cast<char>(0x80 | ((code >> 6) & 0x3F));
                    out += static_cast<char>(0x80 | (code & 0x3F));
                }
                break;
            }
            default: return nullptr;
        }
    }
}

// parseNumber @0x7c35ec: optional '-', integer digits accumulate exactly;
// on '.', 'e', 'E' the token re-parses through float precision.
std::unique_ptr<LogicJSONNode> parseNumber(Cursor& cur, bool negative) {
    const std::size_t start = cur.pos(); // cursor sits on '-' or 1st digit
    if (negative) cur.advance(); // consume '-' like the binary
    i64 intVal = 0;
    bool isFloat = false;
    for (;;) {
        if (cur.pos() >= cur.text().size()) break;
        const char c = cur.text()[cur.pos()];
        if (c >= '0' && c <= '9') {
            intVal = negative ? intVal * 10 - (c - '0') : intVal * 10 + (c - '0');
            cur.advance();
        } else if (c == '.' || c == 'e' || c == 'E') {
            isFloat = true;
            break;
        } else {
            break;
        }
    }
    if (!isFloat) return std::make_unique<LogicJSONNumber>(intVal);
    // Consume the float tail ([.digits][e[+-]digits]) then atof-equivalent.
    while (cur.pos() < cur.text().size()) {
        const char c = cur.text()[cur.pos()];
        if ((c >= '0' && c <= '9') || c == '.' || c == 'e' || c == 'E'
            || c == '+' || c == '-') {
            cur.advance();
        } else {
            break;
        }
    }
    const std::string token = cur.text().substr(start, cur.pos() - start);
    // Binary stores float: parse through float precision deliberately.
    const float f = std::strtof(token.c_str(), nullptr);
    return std::make_unique<LogicJSONNumber>(static_cast<double>(f));
}

// parseArray @0x6ef9d0: '[' (empty -> empty array), values, ',' / ']' loop.
std::unique_ptr<LogicJSONNode> parseArray(Cursor& cur) {
    auto arr = std::make_unique<LogicJSONArray>();
    if (!cur.skipSpace()) return nullptr;
    if (cur.next() != '[') return nullptr; // binary consumes it here
    if (!cur.skipSpace()) return nullptr;
    if (static_cast<unsigned char>(cur.text()[cur.pos()]) == ']') {
        cur.advance(); // consume ']'
        cur.next(); // consume one trailing byte like the binary
        return arr;
    }
    for (;;) {
        auto v = parseValue(cur);
        if (!v) return nullptr;
        arr->add(std::move(v));
        if (!cur.skipSpace()) return nullptr;
        const int c = cur.next();
        if (c == ']') return arr;
        if (c != ',') return nullptr;
    }
}

// parseObject @0x90fb70: '{' (empty -> empty object), string ':' value,
// ',' loop, '}' end. Any failure -> null.
std::unique_ptr<LogicJSONNode> parseObject(Cursor& cur) {
    auto obj = std::make_unique<LogicJSONObject>();
    if (!cur.skipSpace()) return nullptr;
    if (cur.next() != '{') return nullptr; // binary consumes it here
    if (!cur.skipSpace()) return nullptr;
    if (static_cast<unsigned char>(cur.text()[cur.pos()]) == '}') {
        cur.advance(); // consume '}'
        cur.next(); // consume one trailing byte like the binary
        return obj;
    }
    for (;;) {
        auto key = parseString(cur);
        if (!key) return nullptr;
        if (!cur.skipSpace()) return nullptr;
        if (cur.next() != ':') return nullptr;
        auto v = parseValue(cur);
        if (!v) return nullptr;
        obj->put(*key, std::move(v));
        if (!cur.skipSpace()) return nullptr;
        const int c = cur.next();
        if (c == '}') return obj;
        if (c != ',') return nullptr;
    }
}

std::unique_ptr<LogicJSONNode> parseValue(Cursor& cur) {
    if (!cur.skipSpace()) return nullptr;
    const int c = cur.next();
    switch (c) {
        case '"':
            cur.back();
            if (auto s = parseString(cur)) {
                return std::make_unique<LogicJSONString>(*s);
            }
            return nullptr;
        case '-':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            cur.back(); // binary peeks the dispatch char; number owns it
            return parseNumber(cur, c == '-');
        case '[':
            cur.back();
            return parseArray(cur);
        case 'f':
        case 't': {
            cur.back();
            const char* word = (c == 'f') ? "false" : "true";
            const std::size_t len = std::strlen(word);
            if (cur.text().compare(cur.pos(), len, word) != 0) return nullptr;
            // Advance past the literal (binary consumes via its cursor).
            for (std::size_t i = 0; i < len; ++i) cur.next();
            return std::make_unique<LogicJSONBoolean>(c == 't');
        }
        case 'n': {
            cur.back();
            if (cur.text().compare(cur.pos(), 4, "null") != 0) return nullptr;
            for (int i = 0; i < 4; ++i) cur.next();
            return std::make_unique<LogicJSONNull>();
        }
        case '{':
            cur.back();
            return parseObject(cur);
        default:
            return nullptr; // binary warns + nulls
    }
}

} // namespace

inline std::unique_ptr<LogicJSONNode> LogicJSONParser::parse(const std::string& text) {
    Cursor cur(text);
    // BOM skip: U+FEFF (EF BB BF in UTF-8) counts as one position.
    if (text.size() >= 3 && static_cast<unsigned char>(text[0]) == 0xEF
        && static_cast<unsigned char>(text[1]) == 0xBB
        && static_cast<unsigned char>(text[2]) == 0xBF) {
        cur.next();
        cur.next();
        cur.next();
    }
    return parseValue(cur);
}

inline std::string LogicJSONParser::stringify(const LogicJSONNode& node) {
    return node.toString();
}

inline void LogicJSONParser::writeString(const std::string& value, std::string& out) {
    out += '"';
    for (const unsigned char c : value) {
        switch (c) {
            case '\b': out += "\\b"; break;
            case '\t': out += "\\t"; break;
            case '\n': out += "\\n"; break;
            case '\f': out += "\\f"; break;
            case '\r': out += "\\r"; break;
            case '"': out += "\\\""; break;
            case '/': out += "\\/"; break;
            case '\\': out += "\\\\"; break;
            default: out += static_cast<char>(c); break; // appendCodePoint
        }
    }
    out += '"';
}

inline void LogicJSONString::writeToString(std::string& out) const {
    LogicJSONParser::writeString(value_, out);
}

inline void LogicJSONNumber::writeToString(std::string& out) const {
    if (!isFloat_) {
        out += std::to_string(int_);
        return;
    }
    // Binary uses a printf-style float format (resource string); %.9g
    // round-trips float32 exactly and is documented as an approximation.
    char buf[32];
    std::snprintf(buf, sizeof(buf), "%.9g", dbl_);
    out += buf;
}

inline void LogicJSONObject::writeToString(std::string& out) const {
    out += '{';
    bool first = true;
    for (const auto& [k, v] : members_) {
        if (!first) out += ',';
        first = false;
        LogicJSONParser::writeString(k, out);
        out += ':';
        v->writeToString(out);
    }
    out += '}';
}

} // namespace titan
