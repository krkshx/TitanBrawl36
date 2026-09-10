#pragma once

// FriendOnlineStatusEntryMessage (24555) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/messages/pending/FriendOnlineStatus.cpp"

namespace titan {

class FriendOnlineStatusEntryMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24555; }
    const char* getMessageTypeName() const override { return "FriendOnlineStatusEntryMessage"; }
    void encode() override;
    void decode() override;
    LogicLong avatarId_;
    bool hasStatus_ = false;
    std::unique_ptr<FriendOnlineStatus> status_;
};

} // namespace titan

// FriendOnlineStatusEntryMessage bodies — split out of MsgBatch04.cpp; wire format unchanged.


namespace titan {

inline void FriendOnlineStatusEntryMessage::encode() {
PiranhaMessage::encode();
    avatarId_.encode(stream());
    stream().writeBoolean(hasStatus_);
    if (hasStatus_) {
        if (!status_) throw pending_reverse("FriendOnlineStatusEntryMessage needs FriendOnlineStatus");
        status_->encode(stream());
    }
}

inline void FriendOnlineStatusEntryMessage::decode() {
PiranhaMessage::decode();
    avatarId_ = LogicLong::decode(stream());
    hasStatus_ = stream().readBoolean();
    if (hasStatus_) {
        status_ = std::make_unique<FriendOnlineStatus>();
        status_->decode(stream());
    }
}


} // namespace titan
