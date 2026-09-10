#pragma once

// BattleReportStreamMessage (24413) — split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/BattleReportStreamEntry.hpp"

namespace titan {

class BattleReportStreamMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24413; }
    const char* getMessageTypeName() const override { return "BattleReportStreamMessage"; }
    void encode() override;
    void decode() override;
    LogicLong ownerId_;
    std::vector<std::pair<i32, std::unique_ptr<BattleReportStreamEntry>>> entries_;
    bool isNull_ = false;
};

} // namespace titan

// BattleReportStreamMessage bodies — split out of MsgBatch02.cpp; wire format unchanged.


namespace titan {

inline void BattleReportStreamMessage::encode() {
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

inline void BattleReportStreamMessage::decode() {
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
