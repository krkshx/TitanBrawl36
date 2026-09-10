#pragma once

// AskForPlayingLineFriendsMessage (10516) — AskForPlaying*Friends family (see Common.hpp).
// Split out of MsgBatch01.hpp; wire format unchanged.

#include "titan/messages/Common.cpp"

namespace titan {

class AskForPlayingLineFriendsMessage : public PlayingFriendsQuery {
public:
    int getMessageType() const override { return 10516; }
    const char *getMessageTypeName() const override { return "AskForPlayingLineFriendsMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        encodeIds(false, false);
    }
    void decode() override {
        PiranhaMessage::decode();
        decodeIds(false, false);
    }
};

} // namespace titan
