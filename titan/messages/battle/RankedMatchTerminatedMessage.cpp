#pragma once

// RankedMatchTerminatedMessage (22159) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class RankedMatchTerminatedMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22159; }
    const char* getMessageTypeName() const override { return "RankedMatchTerminatedMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v_);
        stream().writeVInt(static_cast<i32>(names_.size()));
        for (const auto& n : names_) stream().writeString(&n);
    }
    void decode() override {
        PiranhaMessage::decode();
        v_ = stream().readVInt();
        const i32 n = stream().readVInt();
        names_.clear();
        for (i32 i = 0; i < n; ++i) names_.push_back(stream().readString().value_or(""));
    }
    i32 v_ = 0;
    std::vector<std::string> names_;
};

} // namespace titan
