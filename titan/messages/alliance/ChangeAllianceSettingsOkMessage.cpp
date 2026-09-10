#pragma once

// ChangeAllianceSettingsOkMessage (24313) — split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class ChangeAllianceSettingsOkMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24313; }
    const char* getMessageTypeName() const override { return "ChangeAllianceSettingsOkMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        if (!entry_) throw pending_reverse("ChangeAllianceSettingsOkMessage needs AllianceFullEntry");
        entry_->encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        entry_ = std::make_unique<AllianceFullEntry>();
        entry_->decode(stream());
    }
    std::unique_ptr<AllianceFullEntry> entry_;
};

} // namespace titan
