#pragma once

// OwnHomeDataMessage (24101) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/messages/pending/LogicClientHome.hpp"

namespace titan {

class OwnHomeDataMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24101; }
    const char* getMessageTypeName() const override { return "OwnHomeDataMessage"; }
    void encode() override;
    void decode() override;
    std::unique_ptr<LogicClientHome> home_;
    std::unique_ptr<LogicClientAvatar> avatar_;
    i32 f152_ = 0;
};

} // namespace titan
