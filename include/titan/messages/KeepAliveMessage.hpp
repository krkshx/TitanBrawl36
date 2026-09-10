#pragma once

// KeepAliveMessage (10108) — reversed from libg_decrypted.so.
//   getMessageType @0x663228 -> 10108
//   encode         @0x7db92c -> base encode only
//   decode         @0x265844 -> empty
// Split out of KeepAliveMessages.hpp; wire format unchanged.

#include "titan/core/PiranhaMessage.hpp"

namespace titan {

class KeepAliveMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10108; }
    const char* getMessageTypeName() const override { return "KeepAliveMessage"; }
};

} // namespace titan
