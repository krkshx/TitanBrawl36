#pragma once

// LogicSetSupportedCreatorCommand (215) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.hpp"
#include "titan/core/DataReference.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicSetSupportedCreatorCommand : public LogicCommand {
public:
    int getCommandType() const override { return 215; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeBoolean(hasStr_);
        if (hasStr_) s.writeString(str_ ? &*str_ : nullptr);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        hasStr_ = s.readBoolean();
        if (hasStr_) str_ = s.readString();
    }
    bool hasStr_ = false;
    std::optional<std::string> str_;
};

} // namespace titan
