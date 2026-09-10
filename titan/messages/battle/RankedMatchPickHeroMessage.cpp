#pragma once

// RankedMatchPickHeroMessage (12155) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class RankedMatchPickHeroMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12155; }
    const char* getMessageTypeName() const override { return "RankedMatchPickHeroMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        DataReference::encodeNullable(stream(), hero_);
        stream().writeBoolean(flag_);
    }
    void decode() override {
        PiranhaMessage::decode();
        hero_ = DataReference::decodeNullable(stream());
        flag_ = stream().readBoolean();
    }
    std::optional<DataReference> hero_;
    bool flag_ = false;
};

} // namespace titan
