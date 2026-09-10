// JoinableAllianceListMessage bodies — split out of MsgBatch05.cpp; wire format unchanged.

#include "titan/messages/JoinableAllianceListMessage.hpp"

namespace titan {

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
