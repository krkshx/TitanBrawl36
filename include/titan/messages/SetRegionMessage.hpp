#pragma once

// SetRegionMessage (14867) — split out of MsgBatch09.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class SetRegionMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14867; }
    const char* getMessageTypeName() const override { return "SetRegionMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(regionId_);
    }
    void decode() override {
        PiranhaMessage::decode();
        regionId_ = stream().readVInt();
    }
    i32 regionId_ = 0;
};

} // namespace titan
