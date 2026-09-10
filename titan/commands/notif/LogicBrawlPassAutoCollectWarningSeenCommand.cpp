#pragma once

// LogicBrawlPassAutoCollectWarningSeenCommand (539) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.cpp"
#include "titan/core/DataReference.cpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicBrawlPassAutoCollectWarningSeenCommand : public LogicCommand {
public:
    int getCommandType() const override { return 539; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(v_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v_ = s.readVInt();
    }
    i32 v_ = 0;
};

} // namespace titan
