#pragma once

// VisionUpdateMessage (24109) — split out of MsgBatch12.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class VisionUpdateMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24109; }
    const char* getMessageTypeName() const override { return "VisionUpdateMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v1_);
        stream().writeVInt(v2_);
        stream().writeVInt(v3_);
        stream().writeVInt(v4_);
        stream().writeBoolean(b1_);
        stream().writeBoolean(hasExtra_);
        stream().writeVInt(hasExtra_ ? extra_ : 0);
        if (!payload_.empty()) {
            stream().writeBytesWithoutLength(payload_.data(),
                                             static_cast<i32>(payload_.size()));
        }
    }
    void decode() override {
        PiranhaMessage::decode();
        v1_ = stream().readVInt();
        v2_ = stream().readVInt();
        v3_ = stream().readVInt();
        v4_ = stream().readVInt();
        b1_ = stream().readBoolean();
        hasExtra_ = stream().readBoolean();
        extra_ = stream().readVInt();
        payload_ = stream().readRawBytes(stream().remaining());
    }
    i32 v1_ = 0, v2_ = 0, v3_ = 0, v4_ = 0;
    bool b1_ = false, hasExtra_ = false;
    i32 extra_ = 0;
    std::vector<u8> payload_; // BitStream blob, see titan/core/BitStream.hpp
};

} // namespace titan
