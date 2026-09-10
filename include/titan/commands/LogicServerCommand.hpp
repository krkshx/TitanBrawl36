#pragma once

// LogicServerCommand (205) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.hpp"
#include "titan/core/DataReference.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicServerCommand : public LogicCommand {
public:
    int getCommandType() const override { return 205; }
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
