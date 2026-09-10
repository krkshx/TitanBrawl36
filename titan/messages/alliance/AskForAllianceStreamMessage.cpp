#pragma once

// AskForAllianceStreamMessage (14304) — split out of MsgBatch01.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class AskForAllianceStreamMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14304; }
    const char* getMessageTypeName() const override { return "AskForAllianceStreamMessage"; }
};

} // namespace titan
