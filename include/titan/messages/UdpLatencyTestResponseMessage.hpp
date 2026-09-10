#pragma once

// UdpLatencyTestResponseMessage (29002) — split out of MsgBatch12.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class UdpLatencyTestResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 29002; }
    const char* getMessageTypeName() const override { return "UdpLatencyTestResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v1_);
        if (payload_) {
            const auto& p = *payload_;
            stream().writeBytes(p.data(), static_cast<i32>(p.size()));
        } else {
            stream().writeBytes(nullptr, 0);
        }
    }
    void decode() override {
        PiranhaMessage::decode();
        v1_ = stream().readVInt();
        payload_ = stream().readBytesNullable();
    }
    i32 v1_ = 0;
    std::optional<std::vector<u8>> payload_;
};

} // namespace titan
