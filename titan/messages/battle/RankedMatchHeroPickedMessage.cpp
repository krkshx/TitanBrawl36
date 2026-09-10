#pragma once

// RankedMatchHeroPickedMessage (22156) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/LogicRankedMatchPlayer.hpp"

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

// RankedMatchHeroPickedMessage bodies — split out of MsgBatch08.cpp; wire format unchanged.


namespace titan {

inline void RankedMatchHeroPickedMessage::encode() {
PiranhaMessage::encode();
    encodeOptionalPlayer(stream(), hasPlayer_, player_);
}

inline void RankedMatchHeroPickedMessage::decode() {
PiranhaMessage::decode();
    decodeOptionalPlayer(stream(), hasPlayer_, player_);
}


} // namespace titan
