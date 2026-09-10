#pragma once

// LogicJSONString — string value; serialization escapes via
// LogicJSONParser::writeString @0x614418.

#include "titan/game/json/LogicJSONNode.cpp"

namespace titan {

class LogicJSONString : public LogicJSONNode {
public:
    explicit LogicJSONString(std::string value = {}) : value_(std::move(value)) {}

    [[nodiscard]] Type type() const override { return Type::String; }
    void writeToString(std::string& out) const override;
    [[nodiscard]] const std::string& value() const { return value_; }

private:
    std::string value_; // +8
};

} // namespace titan
