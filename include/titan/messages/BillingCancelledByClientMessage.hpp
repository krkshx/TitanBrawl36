#pragma once

// BillingCancelledByClientMessage (10160) — split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class BillingCancelledByClientMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10160; }
    const char* getMessageTypeName() const override { return "BillingCancelledByClientMessage"; }
    void encode() override;
    void decode() override;
    std::vector<std::string> productIds_;
    bool isNull_ = false;
};

} // namespace titan
