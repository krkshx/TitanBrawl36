#pragma once

// LogicChangeAvatarNameCommand (201) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.hpp"
#include "titan/core/DataReference.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicChangeAvatarNameCommand : public LogicCommand {
public:
    int getCommandType() const override { return 201; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeStringReference(name_);
        s.writeVInt(v_);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        name_ = s.readStringReference();
        v_ = s.readVInt();
    }
    std::string name_;
    i32 v_ = 0;
};

} // namespace titan
