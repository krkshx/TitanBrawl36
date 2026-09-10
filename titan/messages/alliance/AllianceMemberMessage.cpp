#pragma once

// AllianceMemberMessage (24308) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class AllianceMemberMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24308; }
    const char* getMessageTypeName() const override { return "AllianceMemberMessage"; }
    void encode() override;
    void decode() override;
    LogicLong avatarId_;
    std::unique_ptr<AllianceMemberEntry> entry_;
};

} // namespace titan

// AllianceMemberMessage bodies — split out of MsgBatch00.cpp; wire format unchanged.


namespace titan {

inline void AllianceMemberMessage::encode() {
PiranhaMessage::encode();
    avatarId_.encode(stream());
    if (!entry_) throw pending_reverse("AllianceMemberMessage needs AllianceMemberEntry");
    entry_->encode(stream());
}

inline void AllianceMemberMessage::decode() {
PiranhaMessage::decode();
    avatarId_ = LogicLong::decode(stream());
    entry_ = std::make_unique<AllianceMemberEntry>();
    entry_->decode(stream());
}


} // namespace titan
