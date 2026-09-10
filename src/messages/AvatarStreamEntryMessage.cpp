// AvatarStreamEntryMessage bodies — split out of MsgBatch02.cpp; wire format unchanged.

#include "titan/messages/AvatarStreamEntryMessage.hpp"

namespace titan {

void AvatarStreamEntryMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(entryType_);
    if (!entry_) throw pending_reverse("AvatarStreamEntryMessage needs AvatarStreamEntry");
    entry_->encode(stream());
}

void AvatarStreamEntryMessage::decode() {
    PiranhaMessage::decode();
    entryType_ = stream().readVInt();
    entry_ = std::make_unique<AvatarStreamEntry>();
    entry_->decode(stream());
}


} // namespace titan
