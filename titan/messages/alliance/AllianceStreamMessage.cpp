#pragma once

// AllianceStreamMessage (24311) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/StreamEntry.hpp"

namespace titan {

class AllianceStreamMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24311; }
    const char* getMessageTypeName() const override { return "AllianceStreamMessage"; }
    void encode() override;
    void decode() override;
    // (entryType, entry) pairs; empty vector encodes count -1? No:
    // count -1 means null; we always write the real count.
    std::vector<std::pair<i32, std::unique_ptr<StreamEntry>>> entries_;
    bool isNull_ = false; // -> writeVInt(-1)
};

} // namespace titan

// AllianceStreamMessage bodies — split out of MsgBatch00.cpp; wire format unchanged.


namespace titan {

inline void AllianceStreamMessage::encode() {
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

inline void AllianceStreamMessage::decode() {
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
