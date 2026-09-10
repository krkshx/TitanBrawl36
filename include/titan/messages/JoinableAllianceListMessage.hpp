#pragma once

// JoinableAllianceListMessage (24304) — split out of MsgBatch05.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class JoinableAllianceListMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24304; }
    const char* getMessageTypeName() const override { return "JoinableAllianceListMessage"; }
    void encode() override;
    void decode() override;
    std::vector<std::unique_ptr<AllianceHeaderEntry>> headers_;
    bool isNull_ = false;
};

} // namespace titan
