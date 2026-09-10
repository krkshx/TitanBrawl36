#pragma once

// RankedMatchHeroPickedMessage (22156) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/messages/pending/LogicRankedMatchPlayer.hpp"

namespace titan {

class RankedMatchHeroPickedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22156; }
    const char* getMessageTypeName() const override { return "RankedMatchHeroPickedMessage"; }
    void encode() override;
    void decode() override;
    bool hasPlayer_ = false;
    std::unique_ptr<LogicRankedMatchPlayer> player_;
};

} // namespace titan
