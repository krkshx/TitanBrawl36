// LatencyTestStatusMessage bodies — split out of MsgBatch06.cpp; wire format unchanged.

#include "titan/messages/LatencyTestStatusMessage.hpp"

namespace titan {

void LatencyTestStatusMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(static_cast<i32>(entries_.size()));
    for (const auto& e : entries_) e->encode(stream());
}

void LatencyTestStatusMessage::decode() {
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
