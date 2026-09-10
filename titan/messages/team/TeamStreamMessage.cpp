#pragma once

// TeamStreamMessage (24131) — split out of MsgBatch11.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/stream/StreamEntry.cpp"

namespace titan {

class TeamStreamMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24131; }
    const char* getMessageTypeName() const override { return "TeamStreamMessage"; }
    void encode() override;
    void decode() override;
    i32 v132_ = 0, v136_ = 0;
    std::vector<std::pair<i32, std::unique_ptr<StreamEntry>>> entries_;
    bool isNull_ = false;
};

} // namespace titan

// TeamStreamMessage bodies — split out of MsgBatch11.cpp; wire format unchanged.


namespace titan {

inline void TeamStreamMessage::encode() {
PiranhaMessage::encode();
    stream().writeVInt(v132_);
    stream().writeVInt(v136_);
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

inline void TeamStreamMessage::decode() {
PiranhaMessage::decode();
    v132_ = stream().readVInt();
    v136_ = stream().readVInt();
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
