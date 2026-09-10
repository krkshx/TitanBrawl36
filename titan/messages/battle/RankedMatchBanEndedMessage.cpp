#pragma once

// RankedMatchBanEndedMessage (22153) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class RankedMatchBanEndedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22153; }
    const char* getMessageTypeName() const override { return "RankedMatchBanEndedMessage"; }
    void encode() override;
    void decode() override;
    // (key, charRef) pairs
    std::vector<std::pair<i32, DataReference>> bans_;
};

} // namespace titan

// RankedMatchBanEndedMessage bodies — split out of MsgBatch07.cpp; wire format unchanged.


namespace titan {

inline void RankedMatchBanEndedMessage::encode() {
PiranhaMessage::encode();
    stream().writeVInt(static_cast<i32>(bans_.size()));
    for (const auto& [key, ref] : bans_) {
        stream().writeVInt(key);
        ref.encode(stream());
    }
}

inline void RankedMatchBanEndedMessage::decode() {
PiranhaMessage::decode();
    const i32 n = stream().readVInt();
    bans_.clear();
    for (i32 i = 0; i < n; ++i) {
        const i32 key = stream().readVInt();
        DataReference ref;
        ref.classId = stream().readVInt();
        ref.instanceId = stream().readVInt();
        bans_.emplace_back(key, ref);
    }
}

} // namespace titan
