#pragma once

// GoHomeFromMapEditorMessage (12108) — split out of MsgBatch05.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class GoHomeFromMapEditorMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 12108; }
    const char* getMessageTypeName() const override { return "GoHomeFromMapEditorMessage"; }
};

} // namespace titan
