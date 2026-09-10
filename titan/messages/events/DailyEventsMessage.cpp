#pragma once

// DailyEventsMessage (24123) — split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/home/EventData.cpp"

namespace titan {

class DailyEventsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24123; }
    const char* getMessageTypeName() const override { return "DailyEventsMessage"; }
    void encode() override;
    void decode() override;
    i32 f132_ = 0, f136_ = 0;
    std::vector<std::unique_ptr<EventData>> events_;
};

} // namespace titan

// DailyEventsMessage bodies — split out of MsgBatch03.cpp; wire format unchanged.


namespace titan {

inline void DailyEventsMessage::encode() {
PiranhaMessage::encode();
    stream().writeVInt(f132_);
    stream().writeVInt(f136_);
    stream().writeVInt(static_cast<i32>(events_.size()));
    for (const auto& e : events_) e->encode(stream());
}

inline void DailyEventsMessage::decode() {
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
