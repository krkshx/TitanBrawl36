// RankedMatchHeroPickedMessage bodies — split out of MsgBatch08.cpp; wire format unchanged.

#include "titan/messages/RankedMatchHeroPickedMessage.hpp"

namespace titan {

void RankedMatchHeroPickedMessage::encode() {
    PiranhaMessage::encode();
    encodeOptionalPlayer(stream(), hasPlayer_, player_);
}

void RankedMatchHeroPickedMessage::decode() {
    PiranhaMessage::decode();
    decodeOptionalPlayer(stream(), hasPlayer_, player_);
}


} // namespace titan
