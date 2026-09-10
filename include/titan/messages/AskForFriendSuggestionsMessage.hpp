#pragma once

// AskForFriendSuggestionsMessage (10599) — split out of MsgBatch14.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class AskForFriendSuggestionsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10599; }
    const char* getMessageTypeName() const override { return "AskForFriendSuggestionsMessage"; }
};

} // namespace titan
