#pragma once

// TeamSetLocationMessage (14363) — split out of MsgBatch11.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class TeamSetLocationMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14363; }
    const char* getMessageTypeName() const override { return "TeamSetLocationMessage"; }
    void encode() override;
    void decode() override;
    std::optional<DataReference> locationRef_;
    i32 v144_ = 0;
    std::vector<i32> intList_; // vint count + vint items (verify on capture)
};

} // namespace titan

// TeamSetLocationMessage bodies — split out of MsgBatch11.cpp; wire format unchanged.


namespace titan {

inline void TeamSetLocationMessage::encode() {
PiranhaMessage::encode();
DataReference::encodeNullable(stream(), locationRef_);
    stream().writeVInt(v144_);
    encodeIntList(stream(), intList_);
}

inline void TeamSetLocationMessage::decode() {
PiranhaMessage::decode();
    locationRef_ = DataReference::decodeNullable(stream());
    v144_ = stream().readVInt();
    decodeIntList(stream(), intList_);
}


} // namespace titan
