#pragma once

// AskForAddableFriendsMessage (10503) — split out of MsgBatch01.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class AskForAddableFriendsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10503; }
    const char* getMessageTypeName() const override { return "AskForAddableFriendsMessage"; }
    void encode() override;
    void decode() override;
    std::vector<std::string> idsA_, idsB_;
    bool nullA_ = false, nullB_ = false; // -> writeInt(-1)
};

} // namespace titan

// AskForAddableFriendsMessage bodies — split out of MsgBatch01.cpp; wire format unchanged.



namespace titan {

static void writeIdList(ByteStream& s, const std::vector<std::string>& ids, bool nullFlag) {
    if (nullFlag) {
        s.writeInt(-1);
        return;
    }
    s.writeInt(static_cast<i32>(ids.size()));
    for (const auto& id : ids) s.writeString(&id);
}
static void readIdList(ByteStream& s, std::vector<std::string>& ids, bool& nullFlag) {
    const i32 n = s.readInt();
    ids.clear();
    nullFlag = (n < 0);
    for (i32 i = 0; i < n; ++i) ids.push_back(s.readString().value_or(""));
}


inline void AskForAddableFriendsMessage::encode() {
PiranhaMessage::encode();
    writeIdList(stream(), idsA_, nullA_);
    writeIdList(stream(), idsB_, nullB_);
}

inline void AskForAddableFriendsMessage::decode() {
PiranhaMessage::decode();
    readIdList(stream(), idsA_, nullA_);
    readIdList(stream(), idsB_, nullB_);
}


} // namespace titan
