#pragma once

// StartMissionMessage (14104) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class StartMissionMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14104; }
    const char* getMessageTypeName() const override { return "StartMissionMessage"; }
};

} // namespace titan
