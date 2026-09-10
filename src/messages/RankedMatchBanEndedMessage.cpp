// RankedMatchBanEndedMessage bodies — split out of MsgBatch07.cpp; wire format unchanged.

#include "titan/messages/RankedMatchBanEndedMessage.hpp"

namespace titan {

void RankedMatchBanEndedMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(static_cast<i32>(bans_.size()));
    for (const auto& [key, ref] : bans_) {
        stream().writeVInt(key);
        ref.encode(stream());
    }
}

void RankedMatchBanEndedMessage::decode() {
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
