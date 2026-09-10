#pragma once

// AnalyticEventMessage (10110) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/messages/pending/AnalyticEvent.hpp"

namespace titan {

class AnalyticEventMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10110; }
    const char* getMessageTypeName() const override { return "AnalyticEventMessage"; }
    void encode() override;
    void decode() override;
    std::unique_ptr<AnalyticEvent> event_;
};

} // namespace titan
