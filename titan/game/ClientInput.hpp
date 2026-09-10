#pragma once

// ClientInput — per-tick player input, reversed from libg_decrypted.so.
// BitStream sequence (encode addresses in comments):
//   +0  writePositiveInt(v,15)   @0x38b274 (clamp 0..32767)
//   +4  writePositiveInt(v,4)    @0x64492c (input type; 9 or 11 select below)
//   +8  writeInt(v,15)           @0x898250 (clamp +-32767)
//   +12 writeInt(v,15)           @0x898250
//   +20 writeBoolean             @0x23ca7c
//   +32 writeBoolean             @0x23ca7c
//   type==11: +28 writePositiveVInt(v,3) @0x645450 (else skip when !+20)
//   type==9:  +24 writePositiveInt(v,3)  @0x75d9f4
//   if +20: +21 writeBoolean; if +21: +16 instance clamp(0,16383)
//            writePositiveInt(v,14) @0x7b9100
// Decode mirrors exactly (read fns @0x59fab0/@0x36415c/@0x94b6f8/
// @0x54dc78/@0x2a55f0/@0x190230/@0x642a18).

#include "titan/core/BitStream.hpp"

namespace titan {

struct ClientInput {
    i32 f0_ = 0;        // +0, 15 bits
    i32 inputType_ = 0; // +4, 4 bits (9 or 11)
    i32 f8_ = 0;        // +8, 15-bit signed
    i32 f12_ = 0;       // +12, 15-bit signed
    bool b20_ = false;  // +20
    bool b32_ = false;  // +32
    i32 v28_ = 0;       // +28, VInt prefix 3 (type 11 only)
    i32 v24_ = 0;       // +24, 3 bits (type 9 only)
    bool b21_ = false;  // +21 (if +20)
    i32 targetId_ = 0;  // +16 GlobalID instance, 14 bits (if +21)

    void encode(BitStream& s) const {
        s.writePositiveInt(f0_, 15);
        s.writePositiveInt(inputType_, 4);
        s.writeInt(f8_, 15);
        s.writeInt(f12_, 15);
        s.writeBoolean(b20_);
        s.writeBoolean(b32_);
        if (inputType_ == 11) {
            if (!b20_) return; // early return in the binary
            s.writePositiveVInt(v28_, 3);
        } else if (inputType_ == 9) {
            s.writePositiveInt(v24_, 3);
        }
        if (b20_) {
            s.writeBoolean(b21_);
            if (b21_) s.writePositiveInt(targetId_, 14);
        }
    }
    void decode(BitStream& s) {
        f0_ = s.readPositiveInt(15);
        inputType_ = s.readPositiveInt(4);
        // Binary read side uses unrolled readPositiveIntMaxN helpers.
        f8_ = readSigned(s, 15);
        f12_ = readSigned(s, 15);
        b20_ = s.readBoolean();
        b32_ = s.readBoolean();
        if (inputType_ == 11) {
            if (!b20_) return;
            v28_ = s.readPositiveVInt(3);
        } else if (inputType_ == 9) {
            v24_ = s.readPositiveInt(3);
        }
        if (b20_) {
            b21_ = s.readBoolean();
            if (b21_) targetId_ = s.readPositiveInt(14);
        }
    }

private:
    static i32 readSigned(BitStream& s, int bits) {
        const bool nonNeg = s.readBoolean();
        i32 m = 0;
        for (int i = 0; i < bits; ++i) {
            if (s.readBoolean()) m |= (1 << i);
        }
        return nonNeg ? m : -m;
    }
};

} // namespace titan
