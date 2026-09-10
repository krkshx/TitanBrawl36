#pragma once

// StartLatencyTestRequestMessage (29001) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/game/LatencyTestConfiguration.hpp"

namespace titan {

class StartLatencyTestRequestMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 29001; }
    const char* getMessageTypeName() const override { return "StartLatencyTestRequestMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        if (!config_) throw pending_reverse("StartLatencyTestRequestMessage needs LatencyTestConfiguration");
        config_->encode(stream());
    }
    void decode() override {
        PiranhaMessage::decode();
        config_ = std::make_unique<LatencyTestConfiguration>();
        config_->decode(stream());
    }
    std::unique_ptr<LatencyTestConfiguration> config_;
};

} // namespace titan
