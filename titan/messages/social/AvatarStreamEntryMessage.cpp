#pragma once

// AvatarStreamEntryMessage (24412) — split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/stream/AvatarStreamEntry.cpp"

namespace titan {

class AvatarStreamEntryMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24412; }
    const char* getMessageTypeName() const override { return "AvatarStreamEntryMessage"; }
    void encode() override;
    void decode() override;
    i32 entryType_ = 0;
    std::unique_ptr<AvatarStreamEntry> entry_;
};

} // namespace titan

// AvatarStreamEntryMessage bodies — split out of MsgBatch02.cpp; wire format unchanged.


namespace titan {

inline void AvatarStreamEntryMessage::encode() {
PiranhaMessage::encode();
    stream().writeVInt(entryType_);
    if (!entry_) throw pending_reverse("AvatarStreamEntryMessage needs AvatarStreamEntry");
    entry_->encode(stream());
}

inline void AvatarStreamEntryMessage::decode() {
PiranhaMessage::decode();
    entryType_ = stream().readVInt();
    entry_ = std::make_unique<AvatarStreamEntry>();
    entry_->decode(stream());
}


} // namespace titan
