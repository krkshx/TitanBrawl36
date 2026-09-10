#pragma once

// SCIDBindAccountMessage (10636) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class SCIDBindAccountMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10636; }
    const char* getMessageTypeName() const override { return "SCIDBindAccountMessage"; }
    void encode() override;
    void decode() override;
    std::unique_ptr<LogicCompressedString> f1_, f2_;
};

} // namespace titan
