#pragma once

// GetTokenFriendMessage (15793) — split out of MsgBatch14.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class GetTokenFriendMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 15793; }
    const char* getMessageTypeName() const override { return "GetTokenFriendMessage"; }
};

} // namespace titan
