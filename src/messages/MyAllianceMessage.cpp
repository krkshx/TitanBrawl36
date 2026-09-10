// MyAllianceMessage bodies — split out of MsgBatch07.cpp; wire format unchanged.

#include "titan/messages/MyAllianceMessage.hpp"

namespace titan {

void MyAllianceMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(f132_);
    stream().writeBoolean(hasAlliance_);
    if (hasAlliance_) {
        DataReference::encodeNullable(stream(), allianceRef_);
        if (!header_) throw pending_reverse("MyAllianceMessage needs AllianceHeaderEntry");
        header_->encode(stream());
    }
}

void MyAllianceMessage::decode() {
    PiranhaMessage::decode();
    f132_ = stream().readVInt();
    hasAlliance_ = stream().readBoolean();
    if (hasAlliance_) {
        allianceRef_ = DataReference::decodeNullable(stream());
        header_ = std::make_unique<AllianceHeaderEntry>();
        header_->decode(stream());
    }
}


} // namespace titan
