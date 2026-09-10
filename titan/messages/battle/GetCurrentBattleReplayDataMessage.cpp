#pragma once

// GetCurrentBattleReplayDataMessage (12905) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class GetCurrentBattleReplayDataMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12905; }
    const char* getMessageTypeName() const override { return "GetCurrentBattleReplayDataMessage"; }
};

} // namespace titan
