#pragma once

// LogicHeroWinQuestsChangedCommand (220) — split out; wire format unchanged.

#include "titan/commands/LogicCommand.cpp"
#include "titan/game/home/LogicQuests.cpp"
#include "titan/core/DataReference.cpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace titan {

class LogicHeroWinQuestsChangedCommand : public LogicCommand {
public:
    int getCommandType() const override { return 220; }
    void encode(ByteStream& s) const override {
        LogicCommand::encode(s);
        s.writeBoolean(static_cast<bool>(quests_));
        if (quests_) quests_->encode(s);
    }
    void decode(ByteStream& s) override {
        LogicCommand::decode(s);
        if (s.readBoolean()) {
            quests_ = std::make_unique<LogicQuests>();
            quests_->decode(s);
        }
    }
    std::unique_ptr<LogicQuests> quests_;
};

} // namespace titan
