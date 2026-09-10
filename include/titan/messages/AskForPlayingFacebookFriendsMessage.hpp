#pragma once

// AskForPlayingFacebookFriendsMessage (10513) — AskForPlaying*Friends family (see Common.hpp).
// Split out of MsgBatch01.hpp; wire format unchanged.

#include "titan/messages/Common.hpp"

namespace titan {

class AskForPlayingFacebookFriendsMessage : public PlayingFriendsQuery {
public:
    int getMessageType() const override { return 10513; }
    const char *getMessageTypeName() const override { return "AskForPlayingFacebookFriendsMessage"; }
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
