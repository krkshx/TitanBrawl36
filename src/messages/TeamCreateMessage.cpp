// TeamCreateMessage bodies — split out of MsgBatch10.cpp; wire format unchanged.

#include "titan/messages/TeamCreateMessage.hpp"

namespace titan {

void TeamCreateMessage::encode() {
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

void TeamCreateMessage::decode() {
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
