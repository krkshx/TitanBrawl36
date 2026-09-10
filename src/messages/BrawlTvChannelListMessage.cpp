// BrawlTvChannelListMessage bodies — split out of MsgBatch02.cpp; wire format unchanged.

#include "titan/messages/BrawlTvChannelListMessage.hpp"

namespace titan {

void BrawlTvChannelListMessage::encode() {
    PiranhaMessage::encode();
    encodeEntryArray(stream(), channels_);
}

void BrawlTvChannelListMessage::decode() {
    PiranhaMessage::decode();
    decodeEntryArray(stream(), channels_,
                     [] { return std::make_unique<BrawlTvChannelInfo>(); });
}

} // namespace titan
