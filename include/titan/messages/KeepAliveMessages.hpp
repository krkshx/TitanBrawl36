#pragma once

#include "titan/core/PiranhaMessage.hpp"

// KeepAlive ping pair — reversed from libg_decrypted.so.
//   KeepAliveMessage::getMessageType       @0x663228 -> 10108
//   KeepAliveMessage::encode              @0x7db92c -> base encode only
//   KeepAliveMessage::decode              @0x265844 -> empty
//   KeepAliveServerMessage::getMessageType @0x91db1c -> 20108
//   KeepAliveServerMessage::encode        @0x2e0090 -> base encode only
// Neither message carries fields.

namespace titan {

class KeepAliveMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 10108; }
    const char* getMessageTypeName() const override { return "KeepAliveMessage"; }
};

class KeepAliveServerMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20108; }
    const char* getMessageTypeName() const override { return "KeepAliveServerMessage"; }
};

} // namespace titan
