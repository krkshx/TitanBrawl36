#pragma once

// TeamJoinMessage (14351) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class TeamJoinMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14351; }
    const char* getMessageTypeName() const override { return "TeamJoinMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v136_);
        stream().writeVInt(v140_);
        stream().writeVInt(slotId_);
        DataReference::encodeNullable(stream(), eventRef_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v136_ = stream().readVInt();
        v140_ = stream().readVInt();
        slotId_ = stream().readVInt();
        eventRef_ = DataReference::decodeNullable(stream());
    }
    i32 v136_ = 0, v140_ = 0, slotId_ = 0;
    std::optional<DataReference> eventRef_;
};

} // namespace titan
