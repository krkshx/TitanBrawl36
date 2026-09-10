#pragma once

// AskForPlayingTencentFriendsMessage (10515) — AskForPlaying*Friends family (see Common.hpp).
// Split out of MsgBatch01.hpp; wire format unchanged.

#include "titan/messages/Common.cpp"

namespace titan {

class AskForPlayingTencentFriendsMessage : public PlayingFriendsQuery {
public:
    int getMessageType() const override { return 10515; }
    const char *getMessageTypeName() const override { return "AskForPlayingTencentFriendsMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        encodeIds(true, true);
    }
    void decode() override {
        PiranhaMessage::decode();
        decodeIds(true, true);
    }
};

} // namespace titan
