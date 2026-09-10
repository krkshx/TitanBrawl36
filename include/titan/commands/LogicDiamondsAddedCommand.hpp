#pragma once

// LogicDiamondsAddedCommand (202) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.hpp"
#include "titan/core/DataReference.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicDiamondsAddedCommand : public LogicCommand {
public:
    int getCommandType() const override { return 202; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeBoolean(b_);
        s.writeInt(i1_);
        s.writeInt(i2_);
        s.writeInt(i3_);
        s.writeString(str_ ? &*str_ : nullptr);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        b_ = s.readBoolean();
        i1_ = s.readInt();
        i2_ = s.readInt();
        i3_ = s.readInt();
        str_ = s.readString();
    }
    bool b_ = false;
    i32 i1_ = 0, i2_ = 0, i3_ = 0;
    std::optional<std::string> str_;
};

} // namespace titan
