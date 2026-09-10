// LoginMessage bodies — split out of MsgBatch14.cpp; wire format unchanged.

#include "titan/messages/LoginMessage.hpp"

namespace titan {

void LoginMessage::encode() {
    PiranhaMessage::encode();
    accountId_.encode(stream());
    stream().writeString(s1_ ? &*s1_ : nullptr);
    stream().writeInt(i1_);
    stream().writeInt(i2_);
    stream().writeInt(i3_);
    stream().writeString(s2_ ? &*s2_ : nullptr);
    stream().writeString(s3_ ? &*s3_ : nullptr);
    DataReference::encodeNullable(stream(), ref_);
    stream().writeString(s4_ ? &*s4_ : nullptr);
    stream().writeString(s5_ ? &*s5_ : nullptr);
    stream().writeBoolean(b1_);
    stream().writeStringReference(r1_);
    stream().writeStringReference(r2_);
    stream().writeBoolean(b2_);
    stream().writeString(s6_ ? &*s6_ : nullptr);
    stream().writeInt(i4_);
    stream().writeVInt(v1_);
    stream().writeStringReference(r3_);
    stream().writeStringReference(r4_);
    stream().writeStringReference(r5_);
    stream().writeVInt(v2_);
    stream().writeStringReference(r6_);
    stream().writeStringReference(r7_);
    stream().writeStringReference(r8_);
    if (payload_) {
        const auto& p = *payload_;
        stream().writeBytes(p.data(), static_cast<i32>(p.size()));
    } else {
        stream().writeBytes(nullptr, 0);
    }
    stream().writeBoolean(b3_);
    stream().writeStringReference(r9_);
    stream().writeStringReference(r10_);
}

void LoginMessage::decode() {
    PiranhaMessage::decode();
    accountId_ = LogicLong::decode(stream());
    s1_ = stream().readString();
    i1_ = stream().readInt();
    i2_ = stream().readInt();
    i3_ = stream().readInt();
    s2_ = stream().readString();
    s3_ = stream().readString();
    ref_ = DataReference::decodeNullable(stream());
    s4_ = stream().readString();
    s5_ = stream().readString();
    b1_ = stream().readBoolean();
    r1_ = stream().readStringReference();
    r2_ = stream().readStringReference();
    b2_ = stream().readBoolean();
    s6_ = stream().readString();
    i4_ = stream().readInt();
    v1_ = stream().readVInt();
    r3_ = stream().readStringReference();
    r4_ = stream().readStringReference();
    r5_ = stream().readStringReference();
    v2_ = stream().readVInt();
    r6_ = stream().readStringReference();
    r7_ = stream().readStringReference();
    r8_ = stream().readStringReference();
    payload_ = stream().readBytesNullable();
    b3_ = stream().readBoolean();
    r9_ = stream().readStringReference();
    r10_ = stream().readStringReference();
}


} // namespace titan
