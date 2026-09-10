// AllianceStreamMessage bodies — split out of MsgBatch00.cpp; wire format unchanged.

#include "titan/messages/AllianceStreamMessage.hpp"

namespace titan {

void AllianceStreamMessage::encode() {
    PiranhaMessage::encode();
    if (isNull_) {
        stream().writeVInt(-1);
        return;
    }
    stream().writeVInt(static_cast<i32>(entries_.size()));
    for (const auto& [type, e] : entries_) {
        stream().writeVInt(type);
        e->encode(stream());
    }
}

void AllianceStreamMessage::decode() {
    PiranhaMessage::decode();
    const i32 n = stream().readVInt();
    entries_.clear();
    isNull_ = (n < 0);
    for (i32 i = 0; i < n; ++i) {
        const i32 type = stream().readVInt();
        auto e = std::make_unique<StreamEntry>();
        e->decode(stream());
        entries_.emplace_back(type, std::move(e));
    }
}


} // namespace titan
