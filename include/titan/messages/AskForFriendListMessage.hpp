#pragma once

// AskForFriendListMessage (10504) — split out of MsgBatch14.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class AskForFriendListMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10504; }
    const char* getMessageTypeName() const override { return "AskForFriendListMessage"; }
};

} // namespace titan
