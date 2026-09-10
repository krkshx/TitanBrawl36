#pragma once

// CreatePlayerMapResponseMessage (22100) — split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/player/LogicPlayerMap.cpp"

namespace titan {

class CreatePlayerMapResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22100; }
    const char* getMessageTypeName() const override { return "CreatePlayerMapResponseMessage"; }
    void encode() override;
    void decode() override;
    i32 resultCode_ = 0;
    bool hasMap_ = false;
    std::unique_ptr<LogicPlayerMap> map_;
};

} // namespace titan

// CreatePlayerMapResponseMessage bodies — split out of MsgBatch03.cpp; wire format unchanged.


namespace titan {

inline void CreatePlayerMapResponseMessage::encode() {
PiranhaMessage::encode();
    stream().writeVInt(resultCode_);
    stream().writeBoolean(hasMap_);
    if (hasMap_) {
        if (!map_) throw pending_reverse("CreatePlayerMapResponseMessage needs LogicPlayerMap");
        map_->encode(stream());
    }
}

inline void CreatePlayerMapResponseMessage::decode() {
PiranhaMessage::decode();
    resultCode_ = stream().readVInt();
    hasMap_ = stream().readBoolean();
    if (hasMap_) {
        map_ = std::make_unique<LogicPlayerMap>();
        map_->decode(stream());
    }
}


} // namespace titan
