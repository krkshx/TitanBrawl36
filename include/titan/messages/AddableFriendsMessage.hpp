#pragma once

// AddableFriendsMessage (20107) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/game/AddableFriendEntry.hpp"

namespace titan {

class AddableFriendsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20107; }
    const char* getMessageTypeName() const override { return "AddableFriendsMessage"; }
    void encode() override;
    void decode() override;
    std::vector<std::unique_ptr<AddableFriendEntry>> entries_;
};

} // namespace titan
