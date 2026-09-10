#pragma once

// LogicInviteBlockingChangedCommand (213) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.cpp"
#include "titan/core/DataReference.cpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicInviteBlockingChangedCommand : public LogicCommand {
public:
    int getCommandType() const override { return 213; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeBoolean(b_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        b_ = s.readBoolean();
    }
    bool b_ = false;
};

} // namespace titan
