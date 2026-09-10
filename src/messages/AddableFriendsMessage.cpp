// AddableFriendsMessage bodies — split out of MsgBatch00.cpp; wire format unchanged.

#include "titan/messages/AddableFriendsMessage.hpp"

namespace titan {

void AddableFriendsMessage::encode() {
    PiranhaMessage::encode();
    stream().writeInt(static_cast<i32>(entries_.size()));
    for (const auto& e : entries_) e->encode(stream());
}

void AddableFriendsMessage::decode() {
    PiranhaMessage::decode();
    const i32 n = stream().readInt();
    entries_.clear();
    for (i32 i = 0; i < n; ++i) {
        auto e = std::make_unique<AddableFriendEntry>();
        e->decode(stream());
        entries_.push_back(std::move(e));
    }
}


} // namespace titan
