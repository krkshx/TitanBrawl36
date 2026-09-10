#pragma once

// AskForBattleEndMessage (14110) — split out of MsgBatch01.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/messages/pending/HeroDataEntry.hpp"

namespace titan {

class AskForBattleEndMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14110; }
    const char* getMessageTypeName() const override { return "AskForBattleEndMessage"; }
    void encode() override;
    void decode() override;
    i32 v1_ = 0, v2_ = 0, v3_ = 0;
    std::optional<DataReference> location_;
    std::vector<std::unique_ptr<HeroDataEntry>> heroes_;
};

} // namespace titan
