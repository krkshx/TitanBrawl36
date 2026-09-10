#pragma once

// GetBattleLogMessage (14114) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class GetBattleLogMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14114; }
    const char* getMessageTypeName() const override { return "GetBattleLogMessage"; }
};

} // namespace titan
