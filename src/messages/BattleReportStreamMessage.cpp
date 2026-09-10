// BattleReportStreamMessage bodies — split out of MsgBatch02.cpp; wire format unchanged.

#include "titan/messages/BattleReportStreamMessage.hpp"

namespace titan {

void BattleReportStreamMessage::encode() {
    PiranhaMessage::encode();
    ownerId_.encode(stream());
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

void BattleReportStreamMessage::decode() {
    PiranhaMessage::decode();
    ownerId_ = LogicLong::decode(stream());
    const i32 n = stream().readVInt();
    entries_.clear();
    isNull_ = (n < 0);
    for (i32 i = 0; i < n; ++i) {
        const i32 type = stream().readVInt();
        auto e = std::make_unique<BattleReportStreamEntry>();
        e->decode(stream());
        entries_.emplace_back(type, std::move(e));
    }
}


} // namespace titan
