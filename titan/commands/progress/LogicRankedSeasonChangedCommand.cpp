#pragma once

// LogicRankedSeasonChangedCommand (222) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.cpp"
#include "titan/game/player/LogicPlayerRankedSeasonData.cpp"
#include "titan/core/DataReference.cpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicRankedSeasonChangedCommand : public LogicCommand {
public:
    int getCommandType() const override { return 222; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeBoolean(static_cast<bool>(season_));
        if (season_) season_->encode(s);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        if (s.readBoolean()) {
            season_ = std::make_unique<LogicPlayerRankedSeasonData>();
            season_->decode(s);
        }
    }
    std::unique_ptr<LogicPlayerRankedSeasonData> season_;
};

} // namespace titan
