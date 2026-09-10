#pragma once

// LogicDeleteNotificationCommand (514) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.hpp"
#include "titan/core/DataReference.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicDeleteNotificationCommand : public LogicCommand {
public:
    int getCommandType() const override { return 514; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(v1_);
        s.writeVInt(v2_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v1_ = s.readVInt();
        v2_ = s.readVInt();
    }
    i32 v1_ = 0, v2_ = 0;
};

} // namespace titan
