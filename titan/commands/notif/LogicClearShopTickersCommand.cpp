#pragma once

// LogicClearShopTickersCommand (515) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.cpp"
#include "titan/core/DataReference.cpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicClearShopTickersCommand : public LogicCommand {
public:
    int getCommandType() const override { return 515; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        for (i32 x : v_) s.writeVInt(x);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        for (i32& x : v_) x = s.readVInt();
    }
    i32 v_[4] = {};
};

} // namespace titan
