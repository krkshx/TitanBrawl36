#pragma once

// LogicTransactionsRevokedCommand (208) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.cpp"
#include "titan/core/DataReference.cpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicTransactionsRevokedCommand : public LogicCommand {
public:
    int getCommandType() const override { return 208; }
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
