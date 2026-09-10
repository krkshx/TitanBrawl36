#pragma once

// EndClientTurnMessage (14102) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/commands/LogicCommand.cpp"
#include "titan/gen/LogicCommands.cpp"

namespace titan {

class EndClientTurnMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14102; }
    const char* getMessageTypeName() const override { return "EndClientTurnMessage"; }
    void encode() override;
    void decode() override;
    bool flag_ = false;
    i32 tick_ = 0, checksum_ = 0;
    std::vector<std::unique_ptr<LogicCommand>> commands_;
    std::optional<std::vector<u8>> payload_; // bytes, -1=null
};

} // namespace titan

// EndClientTurnMessage bodies — split out of MsgBatch04.cpp; wire format unchanged.


namespace titan {

inline void EndClientTurnMessage::encode() {
PiranhaMessage::encode();
    stream().writeBoolean(flag_);
    stream().writeVInt(tick_);
    stream().writeVInt(checksum_);
    encodeCommandList(stream(), commands_);
    if (payload_) {
        const auto& p = *payload_;
        stream().writeBytes(p.data(), static_cast<i32>(p.size()));
    } else {
        stream().writeBytes(nullptr, 0);
    }
}

inline void EndClientTurnMessage::decode() {
PiranhaMessage::decode();
    flag_ = stream().readBoolean();
    tick_ = stream().readVInt();
    checksum_ = stream().readVInt();
    decodeCommandList(stream(), commands_);
    payload_ = stream().readBytesNullable();
}

// Wire: string id, bool hasAvatar, [logiclong], string name,
// [bool flag], LogicClientAvatar, [string extra].

} // namespace titan
