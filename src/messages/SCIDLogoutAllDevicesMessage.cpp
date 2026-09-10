// SCIDLogoutAllDevicesMessage bodies — split out of MsgBatch08.cpp; wire format unchanged.

#include "titan/messages/SCIDLogoutAllDevicesMessage.hpp"

namespace titan {

void SCIDLogoutAllDevicesMessage::encode() {
    PiranhaMessage::encode();
    encodeCompressed(stream(), token_, "SCIDLogoutAllDevicesMessage needs LogicCompressedString");
}

void SCIDLogoutAllDevicesMessage::decode() {
    PiranhaMessage::decode();
    decodeCompressed(stream(), token_);
}

} // namespace titan
