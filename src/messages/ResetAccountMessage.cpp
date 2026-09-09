// ResetAccountMessage — reversed from libg_decrypted.so (ARM64).

#include "titan/messages/ResetAccountMessage.hpp"

namespace titan {

ResetAccountMessage::ResetAccountMessage() = default;
ResetAccountMessage::~ResetAccountMessage() = default;

// @0x4465cc — ResetAccountMessage::encode
void ResetAccountMessage::encode() {
    PiranhaMessage::encode();
    stream().writeInt(accountPreset_);
}

// @0x93d3c8 — ResetAccountMessage::decode
void ResetAccountMessage::decode() {
    PiranhaMessage::decode();
    accountPreset_ = stream().readInt();
}

} // namespace titan
