#pragma once

// FriendListMessage (20105) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/FriendEntry.hpp"

namespace titan {

class FriendListMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20105; }
    const char* getMessageTypeName() const override { return "FriendListMessage"; }
    void encode() override;
    void decode() override;
    i32 unknown_ = 0;
    bool flag1_ = false, flag2_ = false;
    std::vector<std::unique_ptr<FriendEntry>> friends_;
    bool isNull_ = false; // -> writeInt(-1)
};

} // namespace titan

// FriendListMessage bodies — split out of MsgBatch04.cpp; wire format unchanged.


namespace titan {

inline void FriendListMessage::encode() {
PiranhaMessage::encode();
    stream().writeInt(unknown_);
    stream().writeBoolean(flag1_);
    stream().writeBoolean(flag2_);
    if (isNull_) {
        stream().writeInt(-1);
        return;
    }
    stream().writeInt(static_cast<i32>(friends_.size()));
    for (const auto& f : friends_) f->encode(stream());
}

inline void FriendListMessage::decode() {
PiranhaMessage::decode();
    unknown_ = stream().readInt();
    flag1_ = stream().readBoolean();
    flag2_ = stream().readBoolean();
    const i32 n = stream().readInt();
    friends_.clear();
    isNull_ = (n < 0);
    for (i32 i = 0; i < n; ++i) {
        auto f = std::make_unique<FriendEntry>();
        f->decode(stream());
        friends_.push_back(std::move(f));
    }
}


} // namespace titan
