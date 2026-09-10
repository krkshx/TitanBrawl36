#pragma once

// TeamToggleSettingsMessage (14372) — split out of MsgBatch11.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class TeamToggleSettingsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14372; }
    const char* getMessageTypeName() const override { return "TeamToggleSettingsMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeBoolean(v132_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v132_ = stream().readBoolean();
    }
    bool v132_ = false;
};

} // namespace titan
