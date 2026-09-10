#pragma once

// AskForAddableFriendsMessage (10503) — split out of MsgBatch01.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class AskForAddableFriendsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10503; }
    const char* getMessageTypeName() const override { return "AskForAddableFriendsMessage"; }
    void encode() override;
    void decode() override;
    std::vector<std::string> idsA_, idsB_;
    bool nullA_ = false, nullB_ = false; // -> writeInt(-1)
};

} // namespace titan
