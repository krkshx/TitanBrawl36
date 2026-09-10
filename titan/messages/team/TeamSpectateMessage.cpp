#pragma once

// TeamSpectateMessage (14358) — split out of MsgBatch11.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class TeamSpectateMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14358; }
    const char* getMessageTypeName() const override { return "TeamSpectateMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(v132_);
        stream().writeVInt(v136_);
        stream().writeVInt(v140_);
    }
    void decode() override {
        PiranhaMessage::decode();
        v132_ = stream().readVInt();
        v136_ = stream().readVInt();
        v140_ = stream().readVInt();
    }
    i32 v132_ = 0, v136_ = 0, v140_ = 0;
};

} // namespace titan
