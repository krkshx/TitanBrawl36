#pragma once

// RankedMatchPickStartedMessage (22154) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/game/LogicRankedMatchPlayer.hpp"

namespace titan {

class RankedMatchPickStartedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22154; }
    const char* getMessageTypeName() const override { return "RankedMatchPickStartedMessage"; }
    void encode() override;
    void decode() override;
    bool hasPlayer_ = false;
    std::unique_ptr<LogicRankedMatchPlayer> player_;
    i32 v144_ = 0, v148_ = 0, v152_ = 0;
    LogicLong id156_;
};

} // namespace titan
