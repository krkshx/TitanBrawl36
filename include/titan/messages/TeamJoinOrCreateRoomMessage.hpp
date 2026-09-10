#pragma once

// TeamJoinOrCreateRoomMessage (14371) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class TeamJoinOrCreateRoomMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14371; }
    const char* getMessageTypeName() const override { return "TeamJoinOrCreateRoomMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeStringReference(roomCode_);
        stream().writeBoolean(create_);
        stream().writeInt(unk156_);
        DataReference::encodeNullable(stream(), eventRef_);
    }
    void decode() override {
        PiranhaMessage::decode();
        roomCode_ = stream().readStringReference();
        create_ = stream().readBoolean();
        unk156_ = stream().readInt();
        eventRef_ = DataReference::decodeNullable(stream());
    }
    std::string roomCode_;
    bool create_ = false;
    i32 unk156_ = 0;
    std::optional<DataReference> eventRef_;
};

} // namespace titan
