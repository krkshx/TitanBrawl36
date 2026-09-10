#pragma once

// LoginOkMessage (20104) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class LoginOkMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20104; }
    const char* getMessageTypeName() const override { return "LoginOkMessage"; }
    void encode() override;
    void decode() override;
    LogicLong accountId_, homeId_;
    std::optional<std::string> passToken_, f160_, f168_;
    i32 f192_ = 0, f196_ = 0, f200_ = 0;
    std::optional<std::string> f208_;
    i32 f216_ = 0, f220_ = 0, f224_ = 0;
    std::optional<std::string> f184_;
    // isAtEnd-guarded tail:
    std::optional<std::string> f232_, f240_;
    i32 f248_ = 0;
    bool hasTail1_ = false;
    std::optional<std::string> f176_, f256_, f264_;
    i32 f312_ = 0;
    bool hasTail2_ = false;
    std::optional<std::string> f272_;
    std::vector<std::string> list296_, list304_; // capped at 100
    bool hasLists_ = false;
    i32 f332_ = 0;
    bool hasF332_ = false;
    std::unique_ptr<LogicCompressedString> compressed_;
    bool hasCompressed_ = false;
    bool f328_ = false, f329not_ = false;
    bool hasFlags_ = false;
    std::optional<std::string> f280_, f336_, f288_, f344_, f352_;
    bool f360_ = false;
    bool hasTail3_ = false;
};

} // namespace titan
