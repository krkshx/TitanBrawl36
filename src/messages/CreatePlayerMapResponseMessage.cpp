// CreatePlayerMapResponseMessage bodies — split out of MsgBatch03.cpp; wire format unchanged.

#include "titan/messages/CreatePlayerMapResponseMessage.hpp"

namespace titan {

void CreatePlayerMapResponseMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(resultCode_);
    stream().writeBoolean(hasMap_);
    if (hasMap_) {
        if (!map_) throw pending_reverse("CreatePlayerMapResponseMessage needs LogicPlayerMap");
        map_->encode(stream());
    }
}

void CreatePlayerMapResponseMessage::decode() {
    PiranhaMessage::decode();
    resultCode_ = stream().readVInt();
    hasMap_ = stream().readBoolean();
    if (hasMap_) {
        map_ = std::make_unique<LogicPlayerMap>();
        map_->decode(stream());
    }
}


} // namespace titan
