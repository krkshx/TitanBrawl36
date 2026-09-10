#pragma once

// KeepAliveServerMessage (20108) — reversed from libg_decrypted.so.
//   getMessageType @0x91db1c -> 20108
//   encode         @0x2e0090 -> base encode only
// Split out of KeepAliveMessages.hpp; wire format unchanged.

#include "titan/core/PiranhaMessage.cpp"

namespace titan {

class KeepAliveServerMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20108; }
    const char* getMessageTypeName() const override { return "KeepAliveServerMessage"; }
};

} // namespace titan
