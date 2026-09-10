#pragma once

// HomeLogicStoppedMessage (14105) — split out of MsgBatch05.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/commands/LogicCommand.hpp"
#include "titan/gen/LogicCommands.hpp"

namespace titan {

class HomeLogicStoppedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14105; }
    const char* getMessageTypeName() const override { return "HomeLogicStoppedMessage"; }
    void encode() override;
    void decode() override; // decode caps count at 0x201 per binary
    i32 tick_ = 0, unknown_ = 0;
    std::vector<std::unique_ptr<LogicCommand>> commands_;
};

} // namespace titan
