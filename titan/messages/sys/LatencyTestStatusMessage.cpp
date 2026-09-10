#pragma once

// LatencyTestStatusMessage (29003) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/util/LatencyData.cpp"

namespace titan {

class LatencyTestStatusMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 29003; }
    const char* getMessageTypeName() const override { return "LatencyTestStatusMessage"; }
    void encode() override;
    void decode() override;
    std::vector<std::unique_ptr<LatencyData>> entries_;
};

} // namespace titan

// LatencyTestStatusMessage bodies — split out of MsgBatch06.cpp; wire format unchanged.


namespace titan {

inline void LatencyTestStatusMessage::encode() {
PiranhaMessage::encode();
    stream().writeVInt(static_cast<i32>(entries_.size()));
    for (const auto& e : entries_) e->encode(stream());
}

inline void LatencyTestStatusMessage::decode() {
PiranhaMessage::decode();
    const i32 n = stream().readVInt();
    entries_.clear();
    for (i32 i = 0; i < n; ++i) {
        auto e = std::make_unique<LatencyData>();
        e->decode(stream());
        entries_.push_back(std::move(e));
    }
}


} // namespace titan
