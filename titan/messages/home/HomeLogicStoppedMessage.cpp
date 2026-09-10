#pragma once

// HomeLogicStoppedMessage (14105) — split out of MsgBatch05.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
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

// HomeLogicStoppedMessage bodies — split out of MsgBatch05.cpp; wire format unchanged.


namespace titan {

inline void HomeLogicStoppedMessage::encode() {
PiranhaMessage::encode();
    stream().writeVInt(tick_);
    stream().writeVInt(unknown_);
    encodeCommandList(stream(), commands_);
}

inline void HomeLogicStoppedMessage::decode() {
PiranhaMessage::decode();
    tick_ = stream().readVInt();
    unknown_ = stream().readVInt();
    i32 n = stream().readVInt();
    if (n > 0x201) n = 0x201; // binary caps the count
    commands_.clear();
    for (i32 i = 0; i < n; ++i) commands_.push_back(decodeSingleCommand(stream()));
}


} // namespace titan
