#pragma once

// ClientInputMessage (10555) — split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class ClientInputMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10555; }
    const char* getMessageTypeName() const override { return "ClientInputMessage"; }
    void encode() override;
    void decode() override;
    i32 f132_ = 0, f136_ = 0, f140_ = 0, f144_ = 0, f148_ = 0;
    std::vector<ClientInput> inputs_;
};

} // namespace titan
