// PlayerProfileMessage bodies — split out of MsgBatch07.cpp; wire format unchanged.

#include "titan/messages/PlayerProfileMessage.hpp"

namespace titan {

void PlayerProfileMessage::encode() {
    PiranhaMessage::encode();
    if (!profile_) throw pending_reverse("PlayerProfileMessage needs PlayerProfile");
    profile_->encode(stream());
    stream().writeBoolean(hasAlliance_);
    if (hasAlliance_) {
        if (!alliance_) throw pending_reverse("PlayerProfileMessage needs AllianceHeaderEntry");
        alliance_->encode(stream());
    }
    DataReference::encodeNullable(stream(), ref152_);
}

void PlayerProfileMessage::decode() {
    PiranhaMessage::decode();
    profile_ = std::make_unique<PlayerProfile>();
    profile_->decode(stream());
    hasAlliance_ = stream().readBoolean();
    if (hasAlliance_) {
        alliance_ = std::make_unique<AllianceHeaderEntry>();
        alliance_->decode(stream());
    }
    ref152_ = DataReference::decodeNullable(stream());
}


} // namespace titan
