#pragma once

// AvatarStreamMessage (24411) — split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/AvatarStreamEntry.hpp"

namespace titan {

class AvatarStreamMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24411; }
    const char* getMessageTypeName() const override { return "AvatarStreamMessage"; }
    void encode() override;
    void decode() override;
    std::vector<std::pair<i32, std::unique_ptr<AvatarStreamEntry>>> entries_;
    bool isNull_ = false;
};

} // namespace titan

// AvatarStreamMessage bodies — split out of MsgBatch02.cpp; wire format unchanged.


namespace titan {

inline void AvatarStreamMessage::encode() {
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

inline void AvatarStreamMessage::decode() {
PiranhaMessage::decode();
    const i32 n = stream().readVInt();
    entries_.clear();
    isNull_ = (n < 0);
    for (i32 i = 0; i < n; ++i) {
        const i32 type = stream().readVInt();
        auto e = std::make_unique<AvatarStreamEntry>();
        e->decode(stream());
        entries_.emplace_back(type, std::move(e));
    }
}



} // namespace titan
