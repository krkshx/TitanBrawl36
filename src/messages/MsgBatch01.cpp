// Message batch 01 bodies — reversed from libg_decrypted.so (ARM64).

#include "titan/messages/MsgBatch01.hpp"

namespace titan {

// ---- 10503 AskForAddableFriendsMessage ----
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

// ---- 14110 AskForBattleEndMessage ----
void AskForBattleEndMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(v1_);
    stream().writeVInt(v2_);
    stream().writeVInt(v3_);
    DataReference::encodeNullable(stream(), location_);
    stream().writeVInt(static_cast<i32>(heroes_.size()));
    for (const auto& h : heroes_) h->encode(stream());
}
void AskForBattleEndMessage::decode() {
    PiranhaMessage::decode();
    v1_ = stream().readVInt();
    v2_ = stream().readVInt();
    v3_ = stream().readVInt();
    location_ = DataReference::decodeNullable(stream());
    const i32 n = stream().readVInt();
    heroes_.clear();
    for (i32 i = 0; i < n; ++i) {
        auto h = std::make_unique<HeroDataEntry>();
        h->decode(stream());
        heroes_.push_back(std::move(h));
    }
}

// ---- PlayingFriendsQuery family ----
void PlayingFriendsQuery::encodeIds(bool useVInt, bool useRef) {
    if (null_) {
        if (useVInt) stream().writeVInt(-1);
        else stream().writeInt(-1);
        return;
    }
    if (useVInt) stream().writeVInt(static_cast<i32>(ids_.size()));
    else stream().writeInt(static_cast<i32>(ids_.size()));
    for (const auto& id : ids_) {
        if (useRef) stream().writeStringReference(id);
        else stream().writeString(&id);
    }
}
void PlayingFriendsQuery::decodeIds(bool useVInt, bool useRef) {
    const i32 n = useVInt ? stream().readVInt() : stream().readInt();
    ids_.clear();
    null_ = (n < 0);
    for (i32 i = 0; i < n; ++i) {
        ids_.push_back(useRef ? stream().readStringReference()
                              : stream().readString().value_or(""));
    }
}

// ---- 30000 AttributionMessage ----
void AttributionMessage::encode() {
    PiranhaMessage::encode();
    for (const auto& s : head_) stream().writeStringReference(s);
    stream().writeVInt(vintField_);
    stream().writeLongLong(longA_);
    stream().writeLongLong(longB_);
    for (bool f : flags_) stream().writeBoolean(f);
    for (const auto& s : tail_) stream().writeStringReference(s);
    stream().writeStringReference(last_);
}
void AttributionMessage::decode() {
    PiranhaMessage::decode();
    for (auto& s : head_) s = stream().readStringReference();
    vintField_ = stream().readVInt();
    longA_ = stream().readLongLong();
    longB_ = stream().readLongLong();
    for (bool& f : flags_) f = stream().readBoolean();
    for (auto& s : tail_) s = stream().readStringReference();
    last_ = stream().readStringReference();
}

// ---- 24111 AvailableServerCommandMessage ----
void AvailableServerCommandMessage::encode() {
    PiranhaMessage::encode();
    if (!command_) throw pending_reverse("AvailableServerCommandMessage: null command");
    encodeSingleCommand(stream(), command_);
}
void AvailableServerCommandMessage::decode() {
    PiranhaMessage::decode();
    command_ = decodeSingleCommand(stream());
}

} // namespace titan
