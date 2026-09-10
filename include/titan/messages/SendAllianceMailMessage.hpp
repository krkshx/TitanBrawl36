#pragma once

// SendAllianceMailMessage (14330) — split out of MsgBatch09.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class SendAllianceMailMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14330; }
    const char* getMessageTypeName() const override { return "SendAllianceMailMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeInt(mode_);
        stream().writeString(content_ ? &*content_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        mode_ = stream().readInt();
        content_ = stream().readString();
    }
    i32 mode_ = 0;
    std::optional<std::string> content_;
};

} // namespace titan
