#pragma once

// LogicCooldownAddedCommand (223) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.hpp"
#include "titan/messages/pending/CooldownEntry.hpp"
#include "titan/core/DataReference.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicCooldownAddedCommand : public LogicCommand {
public:
    int getCommandType() const override { return 223; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        if (isNull_) {
            s.writeVInt(-1);
            return;
        }
        s.writeVInt(static_cast<i32>(entries_.size()));
        for (const auto& e : entries_) e->encode(s);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        const i32 n = s.readVInt();
        entries_.clear();
        isNull_ = (n < 0);
        for (i32 i = 0; i < n; ++i) {
            auto e = std::make_unique<CooldownEntry>();
            e->decode(s);
            entries_.push_back(std::move(e));
        }
    }
    std::vector<std::unique_ptr<CooldownEntry>> entries_;
    bool isNull_ = false;
};

} // namespace titan
