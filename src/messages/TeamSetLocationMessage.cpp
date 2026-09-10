// TeamSetLocationMessage bodies — split out of MsgBatch11.cpp; wire format unchanged.

#include "titan/messages/TeamSetLocationMessage.hpp"

namespace titan {

void TeamSetLocationMessage::encode() {
    PiranhaMessage::encode();
    DataReference::encodeNullable(stream(), locationRef_);
    stream().writeVInt(v144_);
    encodeIntList(stream(), intList_);
}

void TeamSetLocationMessage::decode() {
    PiranhaMessage::decode();
    locationRef_ = DataReference::decodeNullable(stream());
    v144_ = stream().readVInt();
    decodeIntList(stream(), intList_);
}


} // namespace titan
