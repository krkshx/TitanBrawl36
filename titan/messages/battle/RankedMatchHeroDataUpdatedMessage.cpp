#pragma once

// RankedMatchHeroDataUpdatedMessage (22157) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/battle/LogicRankedMatchPlayer.cpp"

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

// RankedMatchHeroDataUpdatedMessage bodies — split out of MsgBatch08.cpp; wire format unchanged.


namespace titan {

inline void RankedMatchHeroDataUpdatedMessage::encode() {
PiranhaMessage::encode();
    encodeOptionalPlayer(stream(), hasPlayer_, player_);
}

inline void RankedMatchHeroDataUpdatedMessage::decode() {
PiranhaMessage::decode();
    decodeOptionalPlayer(stream(), hasPlayer_, player_);
}


} // namespace titan
