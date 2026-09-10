// AnalyticEventMessage bodies — split out of MsgBatch00.cpp; wire format unchanged.

#include "titan/messages/AnalyticEventMessage.hpp"

namespace titan {

void AnalyticEventMessage::encode() {
    PiranhaMessage::encode();
    if (!event_) throw pending_reverse("AnalyticEventMessage needs AnalyticEvent");
    event_->encode(stream());
}

void AnalyticEventMessage::decode() {
    PiranhaMessage::decode();
    event_ = std::make_unique<AnalyticEvent>();
    event_->decode(stream());
}

} // namespace titan
