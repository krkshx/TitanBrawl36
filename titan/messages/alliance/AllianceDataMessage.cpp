#pragma once

// AllianceDataMessage (24301) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class AllianceDataMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24301; }
    const char* getMessageTypeName() const override { return "AllianceDataMessage"; }
    void encode() override;
    void decode() override;
    bool isMyAlliance_ = false;
    std::unique_ptr<AllianceFullEntry> entry_;
};

} // namespace titan

// AllianceDataMessage bodies — split out of MsgBatch00.cpp; wire format unchanged.


namespace titan {

inline void AllianceDataMessage::encode() {
PiranhaMessage::encode();
    stream().writeBoolean(isMyAlliance_);
    if (!entry_) throw pending_reverse("AllianceDataMessage needs AllianceFullEntry");
    entry_->encode(stream());
}

inline void AllianceDataMessage::decode() {
PiranhaMessage::decode();
    isMyAlliance_ = stream().readBoolean();
    entry_ = std::make_unique<AllianceFullEntry>();
    entry_->decode(stream());
}


} // namespace titan
