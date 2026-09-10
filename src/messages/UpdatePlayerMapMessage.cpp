// UpdatePlayerMapMessage bodies — split out of MsgBatch12.cpp; wire format unchanged.

#include "titan/messages/UpdatePlayerMapMessage.hpp"

namespace titan {

void UpdatePlayerMapMessage::encode() {
    PiranhaMessage::encode();
    mapId_.encode(stream());
    if (!mapData_) throw pending_reverse("UpdatePlayerMapMessage needs LogicCompressedString");
    mapData_->encode(stream());
}

void UpdatePlayerMapMessage::decode() {
    PiranhaMessage::decode();
    mapId_ = LogicLong::decode(stream());
    mapData_ = std::make_unique<LogicCompressedString>();
    mapData_->decode(stream());
}

} // namespace titan
