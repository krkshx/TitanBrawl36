// TeamSetPlayerMapMessage bodies — split out of MsgBatch11.cpp; wire format unchanged.

#include "titan/messages/TeamSetPlayerMapMessage.hpp"

namespace titan {

void TeamSetPlayerMapMessage::encode() {
    PiranhaMessage::encode();
    mapId_.encode(stream());
    encodeIntList(stream(), intList_);
}

void TeamSetPlayerMapMessage::decode() {
    PiranhaMessage::decode();
    mapId_ = LogicLong::decode(stream());
    decodeIntList(stream(), intList_);
}


} // namespace titan
