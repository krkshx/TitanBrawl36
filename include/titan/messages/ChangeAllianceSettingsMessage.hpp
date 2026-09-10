#pragma once

// ChangeAllianceSettingsMessage (14316) — AllianceSettings shape (see Common.hpp).
// Split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/Common.hpp"

namespace titan {

class ChangeAllianceSettingsMessage : public AllianceSettingsBase {
public:
    int getMessageType() const override { return 14316; }
    const char *getMessageTypeName() const override { return "ChangeAllianceSettingsMessage"; }
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
