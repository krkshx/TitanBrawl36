#pragma once

// AllianceOnlineStatusUpdatedMessage (20207) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/util/StatusChangeEntry.cpp"

namespace titan {

class AllianceOnlineStatusUpdatedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20207; }
    const char* getMessageTypeName() const override { return "AllianceOnlineStatusUpdatedMessage"; }
    void encode() override;
    void decode() override;
    i32 statusType_ = 0;
    std::vector<std::unique_ptr<StatusChangeEntry>> changes_;
};

} // namespace titan

// AllianceOnlineStatusUpdatedMessage bodies — split out of MsgBatch00.cpp; wire format unchanged.


namespace titan {

inline void AllianceOnlineStatusUpdatedMessage::encode() {
PiranhaMessage::encode();
    stream().writeVInt(statusType_);
    stream().writeVInt(static_cast<i32>(changes_.size()));
    for (const auto& c : changes_) c->encode(stream());
}

inline void AllianceOnlineStatusUpdatedMessage::decode() {
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
