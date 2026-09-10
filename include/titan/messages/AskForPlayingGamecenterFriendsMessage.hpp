#pragma once

// AskForPlayingGamecenterFriendsMessage (10512) — AskForPlaying*Friends family (see Common.hpp).
// Split out of MsgBatch01.hpp; wire format unchanged.

#include "titan/messages/Common.hpp"

namespace titan {

class AskForPlayingGamecenterFriendsMessage : public PlayingFriendsQuery {
public:
    int getMessageType() const override { return 10512; }
    const char *getMessageTypeName() const override { return "AskForPlayingGamecenterFriendsMessage"; }
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
