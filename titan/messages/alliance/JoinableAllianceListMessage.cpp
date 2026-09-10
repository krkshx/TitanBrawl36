#pragma once

// JoinableAllianceListMessage (24304) — split out of MsgBatch05.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class JoinableAllianceListMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24304; }
    const char* getMessageTypeName() const override { return "JoinableAllianceListMessage"; }
    void encode() override;
    void decode() override;
    std::vector<std::unique_ptr<AllianceHeaderEntry>> headers_;
    bool isNull_ = false;
};

} // namespace titan

// JoinableAllianceListMessage bodies — split out of MsgBatch05.cpp; wire format unchanged.


namespace titan {

inline void JoinableAllianceListMessage::encode() {
PiranhaMessage::encode();
    if (isNull_) {
        stream().writeVInt(-1);
        return;
    }
    stream().writeVInt(static_cast<i32>(headers_.size()));
    for (const auto& h : headers_) h->encode(stream());
}

inline void JoinableAllianceListMessage::decode() {
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
