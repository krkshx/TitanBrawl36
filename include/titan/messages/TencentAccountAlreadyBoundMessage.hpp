#pragma once

// TencentAccountAlreadyBoundMessage (24221) — split out of MsgBatch11.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class TencentAccountAlreadyBoundMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24221; }
    const char* getMessageTypeName() const override { return "TencentAccountAlreadyBoundMessage"; }
    void encode() override;
    void decode() override;
    std::optional<std::string> id_, name_;
    bool hasAvatarId_ = false;
    LogicLong avatarId_;
    std::unique_ptr<LogicClientAvatar> avatar_;
};

} // namespace titan
