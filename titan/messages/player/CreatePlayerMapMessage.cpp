#pragma once

// CreatePlayerMapMessage (12100) — split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class CreatePlayerMapMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12100; }
    const char* getMessageTypeName() const override { return "CreatePlayerMapMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(mapName_ ? &*mapName_ : nullptr);
        stream().writeVInt(v_);
        DataReference::encodeNullable(stream(), gameMode_);
    }
    void decode() override {
        PiranhaMessage::decode();
        mapName_ = stream().readString();
        v_ = stream().readVInt();
        gameMode_ = DataReference::decodeNullable(stream());
    }
    std::optional<std::string> mapName_;
    i32 v_ = 0;
    std::optional<DataReference> gameMode_;
};

} // namespace titan
