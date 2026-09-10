#pragma once

// CreateAllianceMessage (14301) — AllianceSettings shape (see Common.hpp).
// Split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/Common.hpp"

namespace titan {

class CreateAllianceMessage : public AllianceSettingsBase {
public:
    int getMessageType() const override { return 14301; }
    const char *getMessageTypeName() const override { return "CreateAllianceMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        encodeSettings();
    }
    void decode() override {
        PiranhaMessage::decode();
        decodeSettings();
    }
};

} // namespace titan
