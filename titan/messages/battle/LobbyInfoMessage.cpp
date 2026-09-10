#pragma once

// LobbyInfoMessage (23457) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/LobbyInfoEntry.hpp"

namespace titan {

class LobbyInfoMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 23457; }
    const char* getMessageTypeName() const override { return "LobbyInfoMessage"; }
    void encode() override;
    void decode() override;
    i32 v132_ = 0;
    std::optional<std::string> name_;
    std::vector<std::unique_ptr<LobbyInfoEntry>> entries_;
};

} // namespace titan

// LobbyInfoMessage bodies — split out of MsgBatch06.cpp; wire format unchanged.


namespace titan {

inline void LobbyInfoMessage::encode() {
PiranhaMessage::encode();
    stream().writeVInt(v132_);
    stream().writeString(name_ ? &*name_ : nullptr);
    stream().writeVInt(static_cast<i32>(entries_.size()));
    for (const auto& e : entries_) e->encode(stream());
}

inline void LobbyInfoMessage::decode() {
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
