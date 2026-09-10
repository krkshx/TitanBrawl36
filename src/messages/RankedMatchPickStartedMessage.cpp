// RankedMatchPickStartedMessage bodies — split out of MsgBatch08.cpp; wire format unchanged.

#include "titan/messages/RankedMatchPickStartedMessage.hpp"

namespace titan {

void RankedMatchPickStartedMessage::encode() {
    PiranhaMessage::encode();
    encodeOptionalPlayer(stream(), hasPlayer_, player_);
    stream().writeVInt(v144_);
    stream().writeVInt(v148_);
    stream().writeVInt(v152_);
    id156_.encode(stream());
}

void RankedMatchPickStartedMessage::decode() {
    PiranhaMessage::decode();
    decodeOptionalPlayer(stream(), hasPlayer_, player_);
    v144_ = stream().readVInt();
    v148_ = stream().readVInt();
    v152_ = stream().readVInt();
    id156_ = LogicLong::decode(stream());
}


} // namespace titan
