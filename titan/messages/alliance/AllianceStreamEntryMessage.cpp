#pragma once

// AllianceStreamEntryMessage (24312) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/stream/StreamEntry.cpp"

namespace titan {

class AllianceStreamEntryMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24312; }
    const char* getMessageTypeName() const override { return "AllianceStreamEntryMessage"; }
    void encode() override;
    void decode() override;
    i32 entryType_ = 0;
    std::unique_ptr<StreamEntry> entry_;
};

} // namespace titan

// AllianceStreamEntryMessage bodies — split out of MsgBatch00.cpp; wire format unchanged.


namespace titan {

inline void AllianceStreamEntryMessage::encode() {
PiranhaMessage::encode();
    stream().writeVInt(entryType_);
    if (!entry_) throw pending_reverse("AllianceStreamEntryMessage needs StreamEntry");
    entry_->encode(stream());
}

inline void AllianceStreamEntryMessage::decode() {
PiranhaMessage::decode();
    entryType_ = stream().readVInt();
    entry_ = std::make_unique<StreamEntry>();
    entry_->decode(stream());
}


} // namespace titan
