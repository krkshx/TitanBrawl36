#pragma once

// LogicPurchaseBrawlPassCommand (534) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.cpp"
#include "titan/core/DataReference.cpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicPurchaseBrawlPassCommand : public LogicCommand {
public:
    int getCommandType() const override { return 534; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeVInt(v_);
        s.writeBoolean(b_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        v_ = s.readVInt();
        b_ = s.readBoolean();
    }
    i32 v_ = 0;
    bool b_ = false;
};

} // namespace titan
