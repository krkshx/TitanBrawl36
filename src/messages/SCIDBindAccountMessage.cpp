// SCIDBindAccountMessage bodies — split out of MsgBatch08.cpp; wire format unchanged.

#include "titan/messages/SCIDBindAccountMessage.hpp"

namespace titan {

void SCIDBindAccountMessage::encode() {
    PiranhaMessage::encode();
    encodeCompressed(stream(), f1_, "SCIDBindAccountMessage needs LogicCompressedString");
    encodeCompressed(stream(), f2_, "SCIDBindAccountMessage needs LogicCompressedString");
}

void SCIDBindAccountMessage::decode() {
    PiranhaMessage::decode();
    decodeCompressed(stream(), f1_);
    decodeCompressed(stream(), f2_);
}


} // namespace titan
