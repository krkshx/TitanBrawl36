#pragma once

// AvailableServerCommandMessage (24111) — split out of MsgBatch01.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/commands/LogicCommand.hpp"
#include "titan/gen/LogicCommands.hpp"

namespace titan {

class AvailableServerCommandMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24111; }
    const char* getMessageTypeName() const override { return "AvailableServerCommandMessage"; }
    void encode() override;
    void decode() override;
    std::unique_ptr<LogicCommand> command_;
};

} // namespace titan
