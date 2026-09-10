#pragma once

// BattleReportStreamMessage (24413) — split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/game/BattleReportStreamEntry.hpp"

namespace titan {

class BattleReportStreamMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24413; }
    const char* getMessageTypeName() const override { return "BattleReportStreamMessage"; }
    void encode() override;
    void decode() override;
    LogicLong ownerId_;
    std::vector<std::pair<i32, std::unique_ptr<BattleReportStreamEntry>>> entries_;
    bool isNull_ = false;
};

} // namespace titan
