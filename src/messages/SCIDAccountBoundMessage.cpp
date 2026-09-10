// SCIDAccountBoundMessage bodies — split out of MsgBatch08.cpp; wire format unchanged.

#include "titan/messages/SCIDAccountBoundMessage.hpp"

namespace titan {

void SCIDAccountBoundMessage::encode() {
    PiranhaMessage::encode();
    stream().writeInt(result_);
    encodeCompressed(stream(), data_, "SCIDAccountBoundMessage needs LogicCompressedString");
}

void SCIDAccountBoundMessage::decode() {
    PiranhaMessage::decode();
    result_ = stream().readInt();
    decodeCompressed(stream(), data_);
}


} // namespace titan
