#pragma once

// MyAllianceMessage (24399) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class MyAllianceMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24399; }
    const char* getMessageTypeName() const override { return "MyAllianceMessage"; }
    void encode() override;
    void decode() override;
    i32 f132_ = 0;
    bool hasAlliance_ = false;
    std::optional<DataReference> allianceRef_;
    std::unique_ptr<AllianceHeaderEntry> header_;
};

} // namespace titan
