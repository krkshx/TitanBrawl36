#pragma once

// SCIDLogoutAllDevicesMessage (11736) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class SCIDLogoutAllDevicesMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 11736; }
    const char* getMessageTypeName() const override { return "SCIDLogoutAllDevicesMessage"; }
    void encode() override;
    void decode() override;
    std::unique_ptr<LogicCompressedString> token_;
};

} // namespace titan
