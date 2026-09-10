#pragma once

// EnterMapEditorMessage (12107) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class EnterMapEditorMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12107; }
    const char* getMessageTypeName() const override { return "EnterMapEditorMessage"; }
};

} // namespace titan
