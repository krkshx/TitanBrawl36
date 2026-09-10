#pragma once

// FriendOnlineStatusMessage (20109) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class FriendOnlineStatusMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20109; }
    const char* getMessageTypeName() const override { return "FriendOnlineStatusMessage"; }
    void encode() override;
    void decode() override;
    std::vector<LogicLong> avatarIds_;
    bool isNull_ = false;
};

} // namespace titan
