#pragma once

// FriendListUpdateMessage (20106) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/social/FriendEntry.cpp"

namespace titan {

class FriendListUpdateMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20106; }
    const char* getMessageTypeName() const override { return "FriendListUpdateMessage"; }
    void encode() override;
    void decode() override;
    bool flag_ = false;
    std::unique_ptr<FriendEntry> entry_;
};

} // namespace titan

// FriendListUpdateMessage bodies — split out of MsgBatch04.cpp; wire format unchanged.


namespace titan {

inline void FriendListUpdateMessage::encode() {
PiranhaMessage::encode();
    stream().writeBoolean(flag_);
    if (!entry_) throw pending_reverse("FriendListUpdateMessage needs FriendEntry");
    entry_->encode(stream());
}

inline void FriendListUpdateMessage::decode() {
PiranhaMessage::decode();
    flag_ = stream().readBoolean();
    entry_ = std::make_unique<FriendEntry>();
    entry_->decode(stream());
}


} // namespace titan
