// EndClientTurnMessage bodies — split out of MsgBatch04.cpp; wire format unchanged.

#include "titan/messages/EndClientTurnMessage.hpp"

namespace titan {

void EndClientTurnMessage::encode() {
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

void EndClientTurnMessage::decode() {
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
