#pragma once

// UdpBigMessageFragmentMessage (20101) — split out of MsgBatch12.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class UdpBigMessageFragmentMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20101; }
    const char* getMessageTypeName() const override { return "UdpBigMessageFragmentMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeByte(fragId_);
        stream().writeVInt(msgId_);
        stream().writeByte(index_);
        stream().writeByte(count_);
        stream().writeVInt(static_cast<i32>(payload_.size()));
        if (!payload_.empty()) {
            stream().writeBytesWithoutLength(payload_.data(),
                                             static_cast<i32>(payload_.size()));
        }
    }
    void decode() override {
        PiranhaMessage::decode();
        fragId_ = stream().readByte();
        msgId_ = stream().readVInt();
        index_ = stream().readByte();
        count_ = stream().readByte();
        const i32 n = stream().readVInt();
        payload_ = (n > 900000) ? std::vector<u8>{} : stream().readRawBytes(n);
    }
    i8 fragId_ = 0, index_ = 0, count_ = 0;
    i32 msgId_ = 0;
    std::vector<u8> payload_;
};

} // namespace titan
