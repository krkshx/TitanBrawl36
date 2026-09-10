#pragma once

// SCIDAccountAlreadyBoundMessage (28689) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class SCIDAccountAlreadyBoundMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 28689; }
    const char* getMessageTypeName() const override { return "SCIDAccountAlreadyBoundMessage"; }
    void encode() override;
    void decode() override;
    std::optional<std::string> token_, name_, extra_;
    bool hasPlayerId_ = false;
    LogicLong playerId_;
    std::unique_ptr<LogicClientAvatar> avatar_;
};

} // namespace titan
