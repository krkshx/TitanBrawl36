#pragma once

// AddableFriendsMessage (20107) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/AddableFriendEntry.hpp"

namespace titan {

class AddableFriendsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20107; }
    const char* getMessageTypeName() const override { return "AddableFriendsMessage"; }
    void encode() override;
    void decode() override;
    std::vector<std::unique_ptr<AddableFriendEntry>> entries_;
};

} // namespace titan

// AddableFriendsMessage bodies — split out of MsgBatch00.cpp; wire format unchanged.


namespace titan {

inline void AddableFriendsMessage::encode() {
PiranhaMessage::encode();
    stream().writeInt(static_cast<i32>(entries_.size()));
    for (const auto& e : entries_) e->encode(stream());
}

inline void AddableFriendsMessage::decode() {
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
