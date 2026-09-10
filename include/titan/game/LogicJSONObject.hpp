#pragma once

// LogicJSONObject::writeToString @0x648efc: '{' + "key":value joined by
// ',' + '}'. Key order is insertion order (binary keeps parallel key/value
// arrays; see LogicJSONObject::put).

#include "titan/game/LogicJSONNode.hpp"

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace titan {

class LogicJSONObject : public LogicJSONNode {
public:
    [[nodiscard]] Type type() const override { return Type::Object; }
    void writeToString(std::string& out) const override;
    void put(std::string key, std::unique_ptr<LogicJSONNode> value) {
        members_.emplace_back(std::move(key), std::move(value));
    }
    [[nodiscard]] std::size_t size() const { return members_.size(); }
    [[nodiscard]] const LogicJSONNode* get(const std::string& key) const {
        for (const auto& [k, v] : members_) {
            if (k == key) return v.get();
        }
        return nullptr;
    }
    [[nodiscard]] const std::vector<std::pair<std::string, std::unique_ptr<LogicJSONNode>>>&
    members() const {
        return members_;
    }

private:
    std::vector<std::pair<std::string, std::unique_ptr<LogicJSONNode>>> members_;
};

} // namespace titan
