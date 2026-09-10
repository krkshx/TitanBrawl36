#pragma once

// LineAccountAlreadyBoundMessage (24205) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class LineAccountAlreadyBoundMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24205; }
    const char* getMessageTypeName() const override { return "LineAccountAlreadyBoundMessage"; }
    void encode() override;
    void decode() override;
    std::optional<std::string> id_, name_;
    bool hasAvatarId_ = false;
    LogicLong avatarId_;
    std::unique_ptr<LogicClientAvatar> avatar_;
};

} // namespace titan
