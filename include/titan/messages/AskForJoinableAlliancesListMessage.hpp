#pragma once

// AskForJoinableAlliancesListMessage (14303) — split out of MsgBatch01.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class AskForJoinableAlliancesListMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14303; }
    const char* getMessageTypeName() const override { return "AskForJoinableAlliancesListMessage"; }
};

} // namespace titan
