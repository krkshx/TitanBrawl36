#pragma once

// LogicJSONBoolean::writeToString @0x356ac8: appends "true"/"false".

#include "titan/game/json/LogicJSONNode.cpp"

namespace titan {

class LogicJSONBoolean : public LogicJSONNode {
public:
    explicit LogicJSONBoolean(bool value = false) : value_(value) {}

    [[nodiscard]] Type type() const override { return Type::Boolean; }
    void writeToString(std::string& out) const override {
        out += value_ ? "true" : "false";
    }
    [[nodiscard]] bool value() const { return value_; }

private:
    bool value_ = false; // +8
};

} // namespace titan
