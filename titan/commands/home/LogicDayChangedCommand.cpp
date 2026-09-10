#pragma once

// LogicDayChangedCommand (204) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.cpp"
#include "titan/game/data/LogicConfData.cpp"
#include "titan/core/DataReference.cpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicDayChangedCommand : public LogicCommand {
public:
    int getCommandType() const override { return 204; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeBoolean(static_cast<bool>(conf_));
        if (conf_) conf_->encode(s);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        if (s.readBoolean()) {
            conf_ = std::make_unique<LogicConfData>();
            conf_->decode(s);
        }
    }
    std::unique_ptr<LogicConfData> conf_;
};

} // namespace titan
