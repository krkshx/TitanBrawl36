#pragma once

// YoozooOrderAvailableMessage (20523) — split out of MsgBatch12.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class YoozooOrderAvailableMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20523; }
    const char* getMessageTypeName() const override { return "YoozooOrderAvailableMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(static_cast<i32>(orders_.size()));
        for (const auto& o : orders_) stream().writeStringReference(o);
    }
    void decode() override {
        PiranhaMessage::decode();
        const i32 n = stream().readVInt();
        orders_.clear();
        for (i32 i = 0; i < n; ++i) orders_.push_back(stream().readStringReference());
    }
    std::vector<std::string> orders_;
};

} // namespace titan
