#pragma once

// AskApiTokenMessage (16939) — split out of MsgBatch01.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class AskApiTokenMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 16939; }
    const char* getMessageTypeName() const override { return "AskApiTokenMessage"; }
};

} // namespace titan
