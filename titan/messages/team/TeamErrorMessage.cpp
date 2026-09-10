#pragma once

// TeamErrorMessage (24129) — split out of MsgBatch10.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class TeamErrorMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24129; }
    const char* getMessageTypeName() const override { return "TeamErrorMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(errorCode_);
        stream().writeVInt(unk136_);
    }
    void decode() override {
        PiranhaMessage::decode();
        errorCode_ = stream().readVInt();
        unk136_ = stream().readVInt();
    }
    i32 errorCode_ = 0, unk136_ = 0;
};

} // namespace titan
