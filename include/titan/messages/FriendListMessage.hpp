#pragma once

// FriendListMessage (20105) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/messages/pending/FriendEntry.hpp"

namespace titan {

class FriendListMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20105; }
    const char* getMessageTypeName() const override { return "FriendListMessage"; }
    void encode() override;
    void decode() override;
    i32 unknown_ = 0;
    bool flag1_ = false, flag2_ = false;
    std::vector<std::unique_ptr<FriendEntry>> friends_;
    bool isNull_ = false; // -> writeInt(-1)
};

} // namespace titan
