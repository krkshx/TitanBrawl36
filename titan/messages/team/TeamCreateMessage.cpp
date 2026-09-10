#pragma once

// TeamCreateMessage (14350) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class TeamCreateMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14350; }
    const char* getMessageTypeName() const override { return "TeamCreateMessage"; }
    void encode() override;
    void decode() override;
    i32 v136_ = 0, v140_ = 0, teamType_ = 0;
    bool unk144_ = false, hasInvite_ = false;
    LogicLong inviteId_;
    i32 unk160_ = 0;
    std::optional<DataReference> eventRef_;
    LogicLong roomId_;
};

} // namespace titan

// TeamCreateMessage bodies — split out of MsgBatch10.cpp; wire format unchanged.


namespace titan {

inline void TeamCreateMessage::encode() {
PiranhaMessage::encode();
    stream().writeVInt(v136_);
    stream().writeVInt(v140_);
    stream().writeVInt(teamType_);
    stream().writeBoolean(unk144_);
    stream().writeBoolean(hasInvite_);
    if (hasInvite_) {
        inviteId_.encode(stream());
        stream().writeVInt(unk160_);
DataReference::encodeNullable(stream(), eventRef_);
    }
    roomId_.encode(stream());
}

inline void TeamCreateMessage::decode() {
PiranhaMessage::decode();
    v136_ = stream().readVInt();
    v140_ = stream().readVInt();
    teamType_ = stream().readVInt();
    unk144_ = stream().readBoolean();
    hasInvite_ = stream().readBoolean();
    if (hasInvite_) {
        inviteId_ = LogicLong::decode(stream());
        unk160_ = stream().readVInt();
        eventRef_ = DataReference::decodeNullable(stream());
    }
    roomId_ = LogicLong::decode(stream());
}


} // namespace titan
