// DailyEventsMessage bodies — split out of MsgBatch03.cpp; wire format unchanged.

#include "titan/messages/DailyEventsMessage.hpp"

namespace titan {

void DailyEventsMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(f132_);
    stream().writeVInt(f136_);
    stream().writeVInt(static_cast<i32>(events_.size()));
    for (const auto& e : events_) e->encode(stream());
}

void DailyEventsMessage::decode() {
    PiranhaMessage::decode();
    f132_ = stream().readVInt();
    f136_ = stream().readVInt();
    const i32 n = stream().readVInt();
    events_.clear();
    for (i32 i = 0; i < n; ++i) {
        auto e = std::make_unique<EventData>();
        e->decode(stream());
        events_.push_back(std::move(e));
    }
}

} // namespace titan
