// Message batch 05 bodies — reversed from libg_decrypted.so (ARM64).

#include "titan/messages/MsgBatch05.hpp"

namespace titan {

// ---- 14105 HomeLogicStoppedMessage ----
void HomeLogicStoppedMessage::encode() {
    PiranhaMessage::encode();
    stream().writeVInt(tick_);
    stream().writeVInt(unknown_);
    stream().writeVInt(static_cast<i32>(commands_.size()));
    for (const auto& c : commands_) c->encode(stream());
}
void HomeLogicStoppedMessage::decode() {
    PiranhaMessage::decode();
    tick_ = stream().readVInt();
    unknown_ = stream().readVInt();
    i32 n = stream().readVInt();
    if (n > 0x201) n = 0x201; // binary caps the count
    commands_.clear();
    for (i32 i = 0; i < n; ++i) {
        auto c = std::make_unique<LogicCommand>();
        c->decode(stream());
        commands_.push_back(std::move(c));
    }
}

// ---- 24304 JoinableAllianceListMessage ----
void JoinableAllianceListMessage::encode() {
    PiranhaMessage::encode();
    if (isNull_) {
        stream().writeVInt(-1);
        return;
    }
    stream().writeVInt(static_cast<i32>(headers_.size()));
    for (const auto& h : headers_) h->encode(stream());
}
void JoinableAllianceListMessage::decode() {
    PiranhaMessage::decode();
    const i32 n = stream().readVInt();
    headers_.clear();
    isNull_ = (n < 0);
    for (i32 i = 0; i < n; ++i) {
        auto h = std::make_unique<AllianceHeaderEntry>();
        h->decode(stream());
        headers_.push_back(std::move(h));
    }
}

} // namespace titan
