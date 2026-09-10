#pragma once

// RankedMatchStartedMessage (22150) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/battle/LogicRankedMatch.cpp"

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

// RankedMatchStartedMessage bodies — split out of MsgBatch08.cpp; wire format unchanged.


namespace titan {

inline void RankedMatchStartedMessage::encode() {
PiranhaMessage::encode();
    stream().writeBoolean(hasMatch_);
    if (hasMatch_) {
        if (!match_) throw pending_reverse("RankedMatchStartedMessage needs LogicRankedMatch");
        match_->encode(stream());
    }
}

inline void RankedMatchStartedMessage::decode() {
PiranhaMessage::decode();
    hasMatch_ = stream().readBoolean();
    if (hasMatch_) {
        match_ = std::make_unique<LogicRankedMatch>();
        match_->decode(stream());
    }
}


// Wire: string token, bool hasId, [logiclong], string name,
// LogicClientAvatar, string extra.

} // namespace titan
