#pragma once

// AnalyticEventMessage (10110) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/AnalyticEvent.hpp"

namespace titan {

class AnalyticEventMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10110; }
    const char* getMessageTypeName() const override { return "AnalyticEventMessage"; }
    void encode() override;
    void decode() override;
    std::unique_ptr<AnalyticEvent> event_;
};

} // namespace titan

// AnalyticEventMessage bodies — split out of MsgBatch00.cpp; wire format unchanged.


namespace titan {

inline void AnalyticEventMessage::encode() {
PiranhaMessage::encode();
    if (!event_) throw pending_reverse("AnalyticEventMessage needs AnalyticEvent");
    event_->encode(stream());
}

inline void AnalyticEventMessage::decode() {
PiranhaMessage::decode();
    event_ = std::make_unique<AnalyticEvent>();
    event_->decode(stream());
}

} // namespace titan
