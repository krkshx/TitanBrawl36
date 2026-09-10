#pragma once

// AvailableServerCommandMessage (24111) — split out of MsgBatch01.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
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

// AvailableServerCommandMessage bodies — split out of MsgBatch01.cpp; wire format unchanged.


namespace titan {

inline void AvailableServerCommandMessage::encode() {
PiranhaMessage::encode();
    if (!command_) throw pending_reverse("AvailableServerCommandMessage: null command");
    encodeSingleCommand(stream(), command_);
}

inline void AvailableServerCommandMessage::decode() {
PiranhaMessage::decode();
    command_ = decodeSingleCommand(stream());
}

} // namespace titan
