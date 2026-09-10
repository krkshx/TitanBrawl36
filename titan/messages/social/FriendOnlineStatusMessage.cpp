#pragma once

// FriendOnlineStatusMessage (20109) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class FriendOnlineStatusMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20109; }
    const char* getMessageTypeName() const override { return "FriendOnlineStatusMessage"; }
    void encode() override;
    void decode() override;
    std::vector<LogicLong> avatarIds_;
    bool isNull_ = false;
};

} // namespace titan

// FriendOnlineStatusMessage bodies — split out of MsgBatch04.cpp; wire format unchanged.


namespace titan {

inline void FriendOnlineStatusMessage::encode() {
PiranhaMessage::encode();
    if (isNull_) {
        stream().writeInt(-1);
        return;
    }
    stream().writeInt(static_cast<i32>(avatarIds_.size()));
    for (const auto& id : avatarIds_) id.encode(stream());
}

inline void FriendOnlineStatusMessage::decode() {
PiranhaMessage::decode();
    const i32 n = stream().readInt();
    avatarIds_.clear();
    isNull_ = (n < 0);
    for (i32 i = 0; i < n; ++i) avatarIds_.push_back(LogicLong::decode(stream()));
}


} // namespace titan
