// HomeLogicStoppedMessage bodies — split out of MsgBatch05.cpp; wire format unchanged.

#include "titan/messages/HomeLogicStoppedMessage.hpp"

namespace titan {

void HomeLogicStoppedMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(tick_);
    stream().writeVInt(unknown_);
    encodeCommandList(stream(), commands_);
}

void HomeLogicStoppedMessage::decode() {
    PiranhaMessage::decode();
    tick_ = stream().readVInt();
    unknown_ = stream().readVInt();
    i32 n = stream().readVInt();
    if (n > 0x201) n = 0x201; // binary caps the count
    commands_.clear();
    for (i32 i = 0; i < n; ++i) commands_.push_back(decodeSingleCommand(stream()));
}


} // namespace titan
