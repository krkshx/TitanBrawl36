#pragma once

// SCIDLogoutAllDevicesResultMessage (23067) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class SCIDLogoutAllDevicesResultMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 23067; }
    const char* getMessageTypeName() const override { return "SCIDLogoutAllDevicesResultMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(result_);
    }
    void decode() override {
        PiranhaMessage::decode();
        result_ = stream().readVInt();
    }
    i32 result_ = 0;
};

} // namespace titan
