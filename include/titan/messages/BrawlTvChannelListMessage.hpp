#pragma once

// BrawlTvChannelListMessage (24700) — split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/messages/pending/BrawlTvChannelInfo.hpp"

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
