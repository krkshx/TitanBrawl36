#pragma once

// LogicSetESportsHubNotificationCommand (224) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.hpp"
#include "titan/core/DataReference.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicSetESportsHubNotificationCommand : public LogicCommand {
public:
    int getCommandType() const override { return 224; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeInt(v_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v_ = s.readInt();
    }
    i32 v_ = 0;
};

} // namespace titan
