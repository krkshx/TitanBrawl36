#pragma once

// AllianceDataMessage (24301) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

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
