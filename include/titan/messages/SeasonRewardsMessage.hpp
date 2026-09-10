#pragma once

// SeasonRewardsMessage (24123) — split out of MsgBatch09.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/messages/pending/LogicGemOffer.hpp"
#include "titan/messages/pending/RewardEntry.hpp"

namespace titan {

class SeasonRewardsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24123; }
    const char* getMessageTypeName() const override { return "SeasonRewardsMessage"; }
    void encode() override;
    void decode() override;
    i32 v132_ = 0;
    std::vector<std::unique_ptr<RewardEntry>> rewards_;
    bool hasOffer_ = false;
    std::unique_ptr<LogicGemOffer> offer_;
};

} // namespace titan
