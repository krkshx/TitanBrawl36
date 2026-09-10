// LobbyInfoMessage bodies — split out of MsgBatch06.cpp; wire format unchanged.

#include "titan/messages/LobbyInfoMessage.hpp"

namespace titan {

void LobbyInfoMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(v132_);
    stream().writeString(name_ ? &*name_ : nullptr);
    stream().writeVInt(static_cast<i32>(entries_.size()));
    for (const auto& e : entries_) e->encode(stream());
}

void LobbyInfoMessage::decode() {
    PiranhaMessage::decode();
    v132_ = stream().readVInt();
    name_ = stream().readString();
    const i32 n = stream().readVInt();
    entries_.clear();
    for (i32 i = 0; i < n; ++i) {
        auto e = std::make_unique<LobbyInfoEntry>();
        e->decode(stream());
        entries_.push_back(std::move(e));
    }
}


} // namespace titan
