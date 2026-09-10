#pragma once

// LatencyTestStatusMessage (29003) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/messages/pending/LatencyData.hpp"

namespace titan {

class LatencyTestStatusMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 29003; }
    const char* getMessageTypeName() const override { return "LatencyTestStatusMessage"; }
    void encode() override;
    void decode() override;
    std::vector<std::unique_ptr<LatencyData>> entries_;
};

} // namespace titan
