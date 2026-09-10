#pragma once

// RankedMatchHeroDataUpdatedMessage (22157) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/game/LogicRankedMatchPlayer.hpp"

namespace titan {

class RankedMatchHeroDataUpdatedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22157; }
    const char* getMessageTypeName() const override { return "RankedMatchHeroDataUpdatedMessage"; }
    void encode() override;
    void decode() override;
    bool hasPlayer_ = false;
    std::unique_ptr<LogicRankedMatchPlayer> player_;
};

} // namespace titan
