#pragma once

// TeamSetLocationMessage (14363) — split out of MsgBatch11.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class TeamSetLocationMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14363; }
    const char* getMessageTypeName() const override { return "TeamSetLocationMessage"; }
    void encode() override;
    void decode() override;
    std::optional<DataReference> locationRef_;
    i32 v144_ = 0;
    std::vector<i32> intList_; // vint count + vint items (verify on capture)
};

} // namespace titan
