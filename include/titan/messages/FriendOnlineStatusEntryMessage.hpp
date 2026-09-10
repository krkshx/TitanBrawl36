#pragma once

// FriendOnlineStatusEntryMessage (24555) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/messages/pending/FriendOnlineStatus.hpp"

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
