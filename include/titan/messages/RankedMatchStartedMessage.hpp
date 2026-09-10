#pragma once

// RankedMatchStartedMessage (22150) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/game/LogicRankedMatch.hpp"

namespace titan {

class RankedMatchStartedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22150; }
    const char* getMessageTypeName() const override { return "RankedMatchStartedMessage"; }
    void encode() override;
    void decode() override;
    bool hasMatch_ = false;
    std::unique_ptr<LogicRankedMatch> match_;
};

} // namespace titan
