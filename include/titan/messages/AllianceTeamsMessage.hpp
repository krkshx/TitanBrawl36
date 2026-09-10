#pragma once

// AllianceTeamsMessage (24364) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/messages/pending/AllianceTeamEntry.hpp"

namespace titan {

class AllianceTeamsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24364; }
    const char* getMessageTypeName() const override { return "AllianceTeamsMessage"; }
    void encode() override;
    void decode() override;
    bool flag_ = false;
    std::vector<std::unique_ptr<AllianceTeamEntry>> teams_;
};

} // namespace titan
