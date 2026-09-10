#pragma once

// RankedMatchPickStartedMessage (22154) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
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

// RankedMatchPickStartedMessage bodies — split out of MsgBatch08.cpp; wire format unchanged.


namespace titan {

inline void RankedMatchPickStartedMessage::encode() {
PiranhaMessage::encode();
    encodeOptionalPlayer(stream(), hasPlayer_, player_);
    stream().writeVInt(v144_);
    stream().writeVInt(v148_);
    stream().writeVInt(v152_);
    id156_.encode(stream());
}

inline void RankedMatchPickStartedMessage::decode() {
PiranhaMessage::decode();
    decodeOptionalPlayer(stream(), hasPlayer_, player_);
    v144_ = stream().readVInt();
    v148_ = stream().readVInt();
    v152_ = stream().readVInt();
    id156_ = LogicLong::decode(stream());
}


} // namespace titan
