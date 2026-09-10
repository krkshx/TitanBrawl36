#pragma once

// AllianceListMessage (24310) — split out of MsgBatch00.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class AllianceListMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24310; }
    const char* getMessageTypeName() const override { return "AllianceListMessage"; }
    void encode() override;
    void decode() override;
    std::optional<std::string> searchQuery_;
    std::vector<std::unique_ptr<AllianceHeaderEntry>> headers_;
};

} // namespace titan

// AllianceListMessage bodies — split out of MsgBatch00.cpp; wire format unchanged.


namespace titan {

inline void AllianceListMessage::encode() {
PiranhaMessage::encode();
    stream().writeString(searchQuery_ ? &*searchQuery_ : nullptr);
    stream().writeVInt(static_cast<i32>(headers_.size()));
    for (const auto& h : headers_) h->encode(stream());
}

inline void AllianceListMessage::decode() {
PiranhaMessage::decode();
    searchQuery_ = stream().readString();
    const i32 n = stream().readVInt();
    headers_.clear();
    for (i32 i = 0; i < n; ++i) {
        auto h = std::make_unique<AllianceHeaderEntry>();
        h->decode(stream());
        headers_.push_back(std::move(h));
    }
}


} // namespace titan
