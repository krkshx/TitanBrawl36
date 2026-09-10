// AllianceListMessage bodies — split out of MsgBatch00.cpp; wire format unchanged.

#include "titan/messages/AllianceListMessage.hpp"

namespace titan {

void AllianceListMessage::encode() {
    PiranhaMessage::encode();
    stream().writeString(searchQuery_ ? &*searchQuery_ : nullptr);
    stream().writeVInt(static_cast<i32>(headers_.size()));
    for (const auto& h : headers_) h->encode(stream());
}

void AllianceListMessage::decode() {
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
