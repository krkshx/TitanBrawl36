#pragma once

// LogicJSONNode — base of the JSON value model (see LogicJSONParser).
// Binary layout: vtable + type field; node kinds Null/Boolean/Number/
// String/Array/Object with getType @0x8733ec and writeToString (slot +32).
// Our port models values directly; observable behavior (parse/serialize)
// matches the binary exactly (see LogicJSONParser).

#include <string>

namespace titan {

class LogicJSONNode {
public:
    enum class Type { Null, Boolean, Number, String, Array, Object };

    virtual ~LogicJSONNode() = default;
    [[nodiscard]] virtual Type type() const = 0;
    virtual void writeToString(std::string& out) const = 0;
    [[nodiscard]] std::string toString() const {
        std::string out;
        writeToString(out);
        return out;
    }
};

} // namespace titan
