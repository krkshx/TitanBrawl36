#pragma once

// LogicJSONNull::writeToString @0x6255a8: appends "null".

#include "titan/game/LogicJSONNode.hpp"

namespace titan {

class LogicJSONNull : public LogicJSONNode {
public:
    [[nodiscard]] Type type() const override { return Type::Null; }
    void writeToString(std::string& out) const override { out += "null"; }
};

} // namespace titan
