// AllianceDataMessage bodies — split out of MsgBatch00.cpp; wire format unchanged.

#include "titan/messages/AllianceDataMessage.hpp"

namespace titan {

void AllianceDataMessage::encode() {
    PiranhaMessage::encode();
    stream().writeBoolean(isMyAlliance_);
    if (!entry_) throw pending_reverse("AllianceDataMessage needs AllianceFullEntry");
    entry_->encode(stream());
}

void AllianceDataMessage::decode() {
    PiranhaMessage::decode();
    isMyAlliance_ = stream().readBoolean();
    entry_ = std::make_unique<AllianceFullEntry>();
    entry_->decode(stream());
}


} // namespace titan
