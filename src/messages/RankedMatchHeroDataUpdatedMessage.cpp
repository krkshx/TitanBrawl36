// RankedMatchHeroDataUpdatedMessage bodies — split out of MsgBatch08.cpp; wire format unchanged.

#include "titan/messages/RankedMatchHeroDataUpdatedMessage.hpp"

namespace titan {

void RankedMatchHeroDataUpdatedMessage::encode() {
    PiranhaMessage::encode();
    encodeOptionalPlayer(stream(), hasPlayer_, player_);
}

void RankedMatchHeroDataUpdatedMessage::decode() {
    PiranhaMessage::decode();
    decodeOptionalPlayer(stream(), hasPlayer_, player_);
}


} // namespace titan
