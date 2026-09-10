#pragma once

// LogicJSONNumber::writeToString @0x2b10c0: integers append raw,
// floats go through a printf-style format (resource string; our port uses
// shortest-float formatting — see LogicJSONParser).
// Integers accumulate exactly (i64); fraction/exponent input re-parses via
// float precision (binary stores float @0x7c37b0; we keep double parsed
// through float to match).

#include "titan/core/Types.cpp"
#include "titan/game/json/LogicJSONNode.cpp"

#include <cstdint>

namespace titan {

class LogicJSONNumber : public LogicJSONNode {
public:
    explicit LogicJSONNumber(i64 value) : isFloat_(false), int_(value) {}
    explicit LogicJSONNumber(int value) : LogicJSONNumber(static_cast<i64>(value)) {}
    explicit LogicJSONNumber(double value) : isFloat_(true), dbl_(value) {}

    [[nodiscard]] Type type() const override { return Type::Number; }
    void writeToString(std::string& out) const override;
    [[nodiscard]] bool isFloat() const { return isFloat_; }
    [[nodiscard]] i64 intValue() const { return int_; }
    [[nodiscard]] double doubleValue() const {
        return isFloat_ ? dbl_ : static_cast<double>(int_);
    }

private:
    bool isFloat_ = false; // +12
    i64 int_ = 0; // +16
    double dbl_ = 0.0; // float @+8 in the binary
};

} // namespace titan
