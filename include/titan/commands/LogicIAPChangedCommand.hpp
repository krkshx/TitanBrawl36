#pragma once

// LogicIAPChangedCommand (210) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.hpp"
#include "titan/game/TimedOffer.hpp"
#include "titan/core/DataReference.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicIAPChangedCommand : public LogicCommand {
public:
    int getCommandType() const override { return 210; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(v_);
        s.writeBoolean(static_cast<bool>(offer_));
        if (offer_) offer_->encode(s);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v_ = s.readVInt();
        if (s.readBoolean()) {
            offer_ = std::make_unique<TimedOffer>();
            offer_->decode(s);
        }
    }
    i32 v_ = 0;
    std::unique_ptr<TimedOffer> offer_;
};

} // namespace titan
