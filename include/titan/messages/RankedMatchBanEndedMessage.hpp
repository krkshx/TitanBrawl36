#pragma once

// RankedMatchBanEndedMessage (22153) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class RankedMatchBanEndedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22153; }
    const char* getMessageTypeName() const override { return "RankedMatchBanEndedMessage"; }
    void encode() override;
    void decode() override;
    // (key, charRef) pairs
    std::vector<std::pair<i32, DataReference>> bans_;
};

} // namespace titan
