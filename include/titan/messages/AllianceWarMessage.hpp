#pragma once

// AllianceWarMessage (24776) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/messages/pending/AllianceWarFaction.hpp"
#include "titan/messages/pending/AllianceWarNode.hpp"

namespace titan {

class AllianceWarMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24776; }
    const char* getMessageTypeName() const override { return "AllianceWarMessage"; }
    void encode() override;
    void decode() override;
    LogicLong allianceId_;
    i32 field144_ = 0;
    std::vector<std::unique_ptr<AllianceWarNode>> nodes_;
    std::vector<std::unique_ptr<AllianceWarFaction>> factions_;
};

} // namespace titan
