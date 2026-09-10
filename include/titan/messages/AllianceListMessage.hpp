#pragma once

// AllianceListMessage (24310) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class AllianceListMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24310; }
    const char* getMessageTypeName() const override { return "AllianceListMessage"; }
    void encode() override;
    void decode() override;
    std::optional<std::string> searchQuery_;
    std::vector<std::unique_ptr<AllianceHeaderEntry>> headers_;
};

} // namespace titan
