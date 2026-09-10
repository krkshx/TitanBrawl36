#pragma once

// BrawlTvChannelListMessage (24700) — split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/social/BrawlTvChannelInfo.cpp"

namespace titan {

class BrawlTvChannelListMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24700; }
    const char* getMessageTypeName() const override { return "BrawlTvChannelListMessage"; }
    void encode() override;
    void decode() override;
    std::vector<std::unique_ptr<BrawlTvChannelInfo>> channels_;
};

} // namespace titan

// BrawlTvChannelListMessage bodies — split out of MsgBatch02.cpp; wire format unchanged.


namespace titan {

inline void BrawlTvChannelListMessage::encode() {
PiranhaMessage::encode();
    encodeEntryArray(stream(), channels_);
}

inline void BrawlTvChannelListMessage::decode() {
PiranhaMessage::decode();
    decodeEntryArray(stream(), channels_,
                     [] { return std::make_unique<BrawlTvChannelInfo>(); });
}

} // namespace titan
