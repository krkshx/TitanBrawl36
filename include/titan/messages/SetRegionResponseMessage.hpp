#pragma once

// SetRegionResponseMessage (24177) — split out of MsgBatch09.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class SetRegionResponseMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24177; }
    const char* getMessageTypeName() const override { return "SetRegionResponseMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(result_);
        stream().writeVInt(region_);
    }
    void decode() override {
        PiranhaMessage::decode();
        result_ = stream().readVInt();
        region_ = stream().readVInt();
    }
    i32 result_ = 0, region_ = 0;
};

} // namespace titan
