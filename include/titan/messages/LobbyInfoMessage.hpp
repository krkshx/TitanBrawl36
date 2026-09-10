#pragma once

// LobbyInfoMessage (23457) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/game/LobbyInfoEntry.hpp"

namespace titan {

class LobbyInfoMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 23457; }
    const char* getMessageTypeName() const override { return "LobbyInfoMessage"; }
    void encode() override;
    void decode() override;
    i32 v132_ = 0;
    std::optional<std::string> name_;
    std::vector<std::unique_ptr<LobbyInfoEntry>> entries_;
};

} // namespace titan
