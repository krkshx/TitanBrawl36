#pragma once

// AskForAvatarStreamMessage (14405) — split out of MsgBatch01.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class AskForAvatarStreamMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14405; }
    const char* getMessageTypeName() const override { return "AskForAvatarStreamMessage"; }
};

} // namespace titan
