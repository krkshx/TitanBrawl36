// AllianceOnlineStatusUpdatedMessage bodies — split out of MsgBatch00.cpp; wire format unchanged.

#include "titan/messages/AllianceOnlineStatusUpdatedMessage.hpp"

namespace titan {

void AllianceOnlineStatusUpdatedMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(statusType_);
    stream().writeVInt(static_cast<i32>(changes_.size()));
    for (const auto& c : changes_) c->encode(stream());
}

void AllianceOnlineStatusUpdatedMessage::decode() {
    PiranhaMessage::decode();
    statusType_ = stream().readVInt();
    const i32 n = stream().readVInt();
    changes_.clear();
    for (i32 i = 0; i < n; ++i) {
        auto c = std::make_unique<StatusChangeEntry>();
        c->decode(stream());
        changes_.push_back(std::move(c));
    }
}


} // namespace titan
