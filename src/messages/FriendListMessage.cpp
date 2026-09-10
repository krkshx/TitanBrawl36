// FriendListMessage bodies — split out of MsgBatch04.cpp; wire format unchanged.

#include "titan/messages/FriendListMessage.hpp"

namespace titan {

void FriendListMessage::encode() {
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

void FriendListMessage::decode() {
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
