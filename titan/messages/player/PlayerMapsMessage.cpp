#pragma once

// PlayerMapsMessage (22102) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/player/LogicPlayerMap.cpp"

namespace titan {

class PlayerMapsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 22102; }
    const char* getMessageTypeName() const override { return "PlayerMapsMessage"; }
    void encode() override;
    void decode() override;
    std::vector<std::unique_ptr<LogicPlayerMap>> maps_;
    bool isNull_ = false;
};

} // namespace titan

// PlayerMapsMessage bodies — split out of MsgBatch07.cpp; wire format unchanged.


namespace titan {

inline void PlayerMapsMessage::encode() {
PiranhaMessage::encode();
    if (isNull_) {
        stream().writeVInt(-1);
        return;
    }
    stream().writeVInt(static_cast<i32>(maps_.size()));
    for (const auto& m : maps_) m->encode(stream());
}

inline void PlayerMapsMessage::decode() {
PiranhaMessage::decode();
    const i32 n = stream().readVInt();
    maps_.clear();
    isNull_ = (n < 0);
    if (isNull_) return; // binary treats null as empty
    for (i32 i = 0; i < n; ++i) {
        auto m = std::make_unique<LogicPlayerMap>();
        m->decode(stream());
        maps_.push_back(std::move(m));
    }
}


} // namespace titan
