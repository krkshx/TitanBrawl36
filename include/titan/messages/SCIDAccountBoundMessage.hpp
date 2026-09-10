#pragma once

// SCIDAccountBoundMessage (25165) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class SCIDAccountBoundMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 25165; }
    const char* getMessageTypeName() const override { return "SCIDAccountBoundMessage"; }
    void encode() override;
    void decode() override;
    i32 result_ = 0;
    std::unique_ptr<LogicCompressedString> data_;
};

} // namespace titan
