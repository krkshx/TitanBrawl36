#pragma once

// SupercellIdNotificationMessage (29900) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class SupercellIdNotificationMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 29900; }
    const char* getMessageTypeName() const override { return "SupercellIdNotificationMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(payload_ ? &*payload_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        payload_ = stream().readString();
    }
    std::optional<std::string> payload_;
};

} // namespace titan
