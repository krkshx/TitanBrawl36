#pragma once

// FriendListUpdateMessage (20106) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/messages/pending/FriendEntry.hpp"

namespace titan {

class FriendListUpdateMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20106; }
    const char* getMessageTypeName() const override { return "FriendListUpdateMessage"; }
    void encode() override;
    void decode() override;
    bool flag_ = false;
    std::unique_ptr<FriendEntry> entry_;
};

} // namespace titan
