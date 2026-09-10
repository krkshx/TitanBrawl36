// LoginFailedMessage bodies — split out of MsgBatch14.cpp; wire format unchanged.

#include "titan/messages/LoginFailedMessage.hpp"

namespace titan {

void LoginFailedMessage::encode() {
    PiranhaMessage::encode();
    stream().writeInt(code_);
    stream().writeString(s1_ ? &*s1_ : nullptr);
    stream().writeString(s2_ ? &*s2_ : nullptr);
    stream().writeString(s3_ ? &*s3_ : nullptr);
    stream().writeString(s4_ ? &*s4_ : nullptr);
    stream().writeString(s5_ ? &*s5_ : nullptr);
    stream().writeInt(i1_);
    stream().writeBoolean(b1_);
    if (payload_) {
        const auto& p = *payload_;
        stream().writeBytes(p.data(), static_cast<i32>(p.size()));
    } else {
        stream().writeBytes(nullptr, 0);
    }
    if (listNull_) {
        stream().writeInt(-1);
    } else {
        stream().writeInt(static_cast<i32>(list_.size()));
        for (const auto& s : list_) stream().writeString(&s);
    }
    stream().writeInt(i2_);
    stream().writeInt(i3_);
    stream().writeString(s6_ ? &*s6_ : nullptr);
    stream().writeInt(i4_);
    stream().writeBoolean(b2_);
    stream().writeBoolean(b3_);
    stream().writeStringReference(r1_);
    stream().writeVInt(v1_);
    stream().writeStringReference(r2_);
    stream().writeBoolean(b4_);
    tailId_.encode(stream());
}

void LoginFailedMessage::decode() {
    PiranhaMessage::decode();
    code_ = stream().readInt();
    s1_ = stream().readString();
    s2_ = stream().readString();
    s3_ = stream().readString();
    s4_ = stream().readString();
    s5_ = stream().readString();
    i1_ = stream().readInt();
    b1_ = stream().readBoolean();
    payload_ = stream().readBytesNullable();
    const i32 n = stream().readInt();
    list_.clear();
    listNull_ = (n < 0);
    for (i32 i = 0; i < n; ++i) list_.push_back(stream().readString().value_or(""));
    i2_ = stream().readInt();
    i3_ = stream().readInt();
    s6_ = stream().readString();
    i4_ = stream().readInt();
    b2_ = stream().readBoolean();
    b3_ = stream().readBoolean();
    if (stream().isAtEnd()) return;
    r1_ = stream().readStringReference();
    v1_ = stream().readVInt();
    r2_ = stream().readStringReference();
    b4_ = stream().readBoolean();
    tailId_ = LogicLong::decode(stream());
    hasTail_ = true;
}

} // namespace titan
