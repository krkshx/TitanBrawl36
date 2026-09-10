#pragma once

// UpdatePlayerMapMessage (12103) — split out of MsgBatch12.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class UpdatePlayerMapMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12103; }
    const char* getMessageTypeName() const override { return "UpdatePlayerMapMessage"; }
    void encode() override;
    void decode() override;
    LogicLong mapId_;
    std::unique_ptr<LogicCompressedString> mapData_;
};

} // namespace titan

// UpdatePlayerMapMessage bodies — split out of MsgBatch12.cpp; wire format unchanged.


namespace titan {

inline void UpdatePlayerMapMessage::encode() {
PiranhaMessage::encode();
    mapId_.encode(stream());
    if (!mapData_) throw pending_reverse("UpdatePlayerMapMessage needs LogicCompressedString");
    mapData_->encode(stream());
}

inline void UpdatePlayerMapMessage::decode() {
PiranhaMessage::decode();
    mapId_ = LogicLong::decode(stream());
    mapData_ = std::make_unique<LogicCompressedString>();
    mapData_->decode(stream());
}

} // namespace titan
