// AllianceStreamEntryMessage bodies — split out of MsgBatch00.cpp; wire format unchanged.

#include "titan/messages/AllianceStreamEntryMessage.hpp"

namespace titan {

void AllianceStreamEntryMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(entryType_);
    if (!entry_) throw pending_reverse("AllianceStreamEntryMessage needs StreamEntry");
    entry_->encode(stream());
}

void AllianceStreamEntryMessage::decode() {
    PiranhaMessage::decode();
    entryType_ = stream().readVInt();
    entry_ = std::make_unique<StreamEntry>();
    entry_->decode(stream());
}


} // namespace titan
