#pragma once

// LogicJSONArray::writeToString @0x611154: '[' + items joined by ',' + ']'.

#include "titan/game/json/LogicJSONNode.cpp"

#include <memory>
#include <vector>

namespace titan {

class LogicJSONArray : public LogicJSONNode {
public:
    [[nodiscard]] Type type() const override { return Type::Array; }
    void writeToString(std::string& out) const override {
        out += '[';
        bool first = true;
        for (const auto& e : items_) {
            if (!first) out += ',';
            first = false;
            e->writeToString(out);
        }
        out += ']';
    }
    void add(std::unique_ptr<LogicJSONNode> node) {
        items_.push_back(std::move(node));
    }
    [[nodiscard]] std::size_t size() const { return items_.size(); }
    [[nodiscard]] const LogicJSONNode* at(std::size_t i) const {
        return items_[i].get();
    }

private:
    std::vector<std::unique_ptr<LogicJSONNode>> items_;
};

} // namespace titan
