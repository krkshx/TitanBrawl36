// AvailableServerCommandMessage bodies — split out of MsgBatch01.cpp; wire format unchanged.

#include "titan/messages/AvailableServerCommandMessage.hpp"

namespace titan {

void AvailableServerCommandMessage::encode() {
    PiranhaMessage::encode();
    if (!command_) throw pending_reverse("AvailableServerCommandMessage: null command");
    encodeSingleCommand(stream(), command_);
}

void AvailableServerCommandMessage::decode() {
    PiranhaMessage::decode();
    command_ = decodeSingleCommand(stream());
}

} // namespace titan
