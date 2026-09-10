#pragma once

// MyAllianceMessage (24399) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class MyAllianceMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24399; }
    const char* getMessageTypeName() const override { return "MyAllianceMessage"; }
    void encode() override;
    void decode() override;
    i32 f132_ = 0;
    bool hasAlliance_ = false;
    std::optional<DataReference> allianceRef_;
    std::unique_ptr<AllianceHeaderEntry> header_;
};

} // namespace titan

// MyAllianceMessage bodies — split out of MsgBatch07.cpp; wire format unchanged.


namespace titan {

inline void MyAllianceMessage::encode() {
PiranhaMessage::encode();
    stream().writeVInt(f132_);
    stream().writeBoolean(hasAlliance_);
    if (hasAlliance_) {
DataReference::encodeNullable(stream(), allianceRef_);
        if (!header_) throw pending_reverse("MyAllianceMessage needs AllianceHeaderEntry");
        header_->encode(stream());
    }
}

inline void MyAllianceMessage::decode() {
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
