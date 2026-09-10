#pragma once

// AllianceMemberMessage (24308) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class AllianceMemberMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24308; }
    const char* getMessageTypeName() const override { return "AllianceMemberMessage"; }
    void encode() override;
    void decode() override;
    LogicLong avatarId_;
    std::unique_ptr<AllianceMemberEntry> entry_;
};

} // namespace titan
