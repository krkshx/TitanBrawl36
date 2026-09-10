#pragma once

// TeamSetPlayerMapMessage (12110) — split out of MsgBatch11.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class TeamSetPlayerMapMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12110; }
    const char* getMessageTypeName() const override { return "TeamSetPlayerMapMessage"; }
    void encode() override;
    void decode() override;
    LogicLong mapId_;
    std::vector<i32> intList_; // vint count + vint items (verify on capture)
};

} // namespace titan

// TeamSetPlayerMapMessage bodies — split out of MsgBatch11.cpp; wire format unchanged.


namespace titan {

inline void TeamSetPlayerMapMessage::encode() {
PiranhaMessage::encode();
    mapId_.encode(stream());
    encodeIntList(stream(), intList_);
}

inline void TeamSetPlayerMapMessage::decode() {
PiranhaMessage::decode();
    mapId_ = LogicLong::decode(stream());
    decodeIntList(stream(), intList_);
}


} // namespace titan
