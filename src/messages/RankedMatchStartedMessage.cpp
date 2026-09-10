// RankedMatchStartedMessage bodies — split out of MsgBatch08.cpp; wire format unchanged.

#include "titan/messages/RankedMatchStartedMessage.hpp"

namespace titan {

void RankedMatchStartedMessage::encode() {
    PiranhaMessage::encode();
    stream().writeBoolean(hasMatch_);
    if (hasMatch_) {
        if (!match_) throw pending_reverse("RankedMatchStartedMessage needs LogicRankedMatch");
        match_->encode(stream());
    }
}

void RankedMatchStartedMessage::decode() {
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
