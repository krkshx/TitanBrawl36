// PiranhaMessage — reversed from libg_decrypted.so (ARM64).

#include "titan/core/PiranhaMessage.hpp"

namespace titan {

PiranhaMessage::PiranhaMessage() = default;
PiranhaMessage::~PiranhaMessage() = default;

// @0x8f2e70 — PiranhaMessage::encode
// The binary verifies the return address lies inside libg's address range
// (_libg_begin_ptr.._libg_end_ptr) and calls AntiCheat::guard_callback(0x1F)
// otherwise. Semantically a no-op: no bytes are written here.
// We keep the hook point (TITAN_ANTITAMPER_HOOK) for ports that re-add it.
void PiranhaMessage::encode() {
#ifdef TITAN_ANTITAMPER_HOOK
    TITAN_ANTITAMPER_HOOK(0x1F);
#else
    (void)0;
#endif
}

void PiranhaMessage::decode() {
    // Base decode is empty in the binary (ResetAccountMessage::decode @0x93d3c8
    // calls PiranhaMessage::decode() then reads its own field).
}

} // namespace titan
