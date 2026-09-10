#pragma once

// TeamPremadeChatMessage (14369) — split out of MsgBatch11.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class TeamPremadeChatMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14369; }
    const char* getMessageTypeName() const override { return "TeamPremadeChatMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        DataReference::encodeNullable(stream(), dataRef_);
        stream().writeBoolean(hasPlayerId_);
        if (hasPlayerId_) playerId_.encode(stream());
        stream().writeVInt(v156_);
        stream().writeVInt(v152_);
        stream().writeVInt(v160_);
    }
    void decode() override {
        PiranhaMessage::decode();
        dataRef_ = DataReference::decodeNullable(stream());
        hasPlayerId_ = stream().readBoolean();
        if (hasPlayerId_) playerId_ = LogicLong::decode(stream());
        v156_ = stream().readVInt();
        v152_ = stream().readVInt();
        hasV160_ = !stream().isAtEnd();
        if (hasV160_) v160_ = stream().readVInt();
    }
    std::optional<DataReference> dataRef_;
    bool hasPlayerId_ = false;
    LogicLong playerId_;
    i32 v156_ = 0, v152_ = 0, v160_ = 0;
    bool hasV160_ = false;
};

} // namespace titan
