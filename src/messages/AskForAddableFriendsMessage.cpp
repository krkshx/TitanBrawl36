// AskForAddableFriendsMessage bodies — split out of MsgBatch01.cpp; wire format unchanged.

#include "titan/messages/AskForAddableFriendsMessage.hpp"


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


void AskForAddableFriendsMessage::encode() {
    PiranhaMessage::encode();
    writeIdList(stream(), idsA_, nullA_);
    writeIdList(stream(), idsB_, nullB_);
}

void AskForAddableFriendsMessage::decode() {
    PiranhaMessage::decode();
    readIdList(stream(), idsA_, nullA_);
    readIdList(stream(), idsB_, nullB_);
}


} // namespace titan
