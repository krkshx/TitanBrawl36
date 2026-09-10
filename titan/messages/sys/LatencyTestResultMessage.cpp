#pragma once

// LatencyTestResultMessage (19001) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class LatencyTestResultMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 19001; }
    const char* getMessageTypeName() const override { return "LatencyTestResultMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        for (i32 x : vints_) stream().writeVInt(x);
        stream().writeBoolean(flag_);
        stream().writeString(tail_ ? &*tail_ : nullptr);
    }
    void decode() override {
        PiranhaMessage::decode();
        for (i32& x : vints_) x = stream().readVInt();
        flag_ = stream().readBoolean();
        tail_ = stream().readString();
    }
    i32 vints_[7] = {};
    bool flag_ = false;
    std::optional<std::string> tail_;
};

} // namespace titan
