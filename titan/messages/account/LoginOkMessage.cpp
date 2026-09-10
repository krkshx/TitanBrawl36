#pragma once

// LoginOkMessage (20104) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class LoginOkMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20104; }
    const char* getMessageTypeName() const override { return "LoginOkMessage"; }
    void encode() override;
    void decode() override;
    LogicLong accountId_, homeId_;
    std::optional<std::string> passToken_, f160_, f168_;
    i32 f192_ = 0, f196_ = 0, f200_ = 0;
    std::optional<std::string> f208_;
    i32 f216_ = 0, f220_ = 0, f224_ = 0;
    std::optional<std::string> f184_;
    // isAtEnd-guarded tail:
    std::optional<std::string> f232_, f240_;
    i32 f248_ = 0;
    bool hasTail1_ = false;
    std::optional<std::string> f176_, f256_, f264_;
    i32 f312_ = 0;
    bool hasTail2_ = false;
    std::optional<std::string> f272_;
    std::vector<std::string> list296_, list304_; // capped at 100
    bool hasLists_ = false;
    i32 f332_ = 0;
    bool hasF332_ = false;
    std::unique_ptr<LogicCompressedString> compressed_;
    bool hasCompressed_ = false;
    bool f328_ = false, f329not_ = false;
    bool hasFlags_ = false;
    std::optional<std::string> f280_, f336_, f288_, f344_, f352_;
    bool f360_ = false;
    bool hasTail3_ = false;
};

} // namespace titan

// LoginOkMessage bodies — split out of MsgBatch06.cpp; wire format unchanged.



namespace titan {

static std::vector<std::string> readCappedStringList(ByteStream& s) {
    const i32 n = s.readInt();
    std::vector<std::string> out;
    for (i32 i = 0; i < n && i < 100; ++i) out.push_back(s.readString().value_or(""));
    return out;
}


inline void LoginOkMessage::encode() {
PiranhaMessage::encode();
    accountId_.encode(stream());
    homeId_.encode(stream());
    stream().writeString(passToken_ ? &*passToken_ : nullptr);
    stream().writeString(f160_ ? &*f160_ : nullptr);
    stream().writeString(f168_ ? &*f168_ : nullptr);
    stream().writeInt(f192_);
    stream().writeInt(f196_);
    stream().writeInt(f200_);
    stream().writeString(f208_ ? &*f208_ : nullptr);
    stream().writeInt(f216_);
    stream().writeInt(f220_);
    stream().writeInt(f224_);
    stream().writeString(f184_ ? &*f184_ : nullptr);
    // Guarded tail is always written by encode; decode tolerates absence.
    stream().writeString(f232_ ? &*f232_ : nullptr);
    stream().writeString(f240_ ? &*f240_ : nullptr);
    stream().writeInt(f248_);
    stream().writeString(f176_ ? &*f176_ : nullptr);
    stream().writeString(f256_ ? &*f256_ : nullptr);
    stream().writeString(f264_ ? &*f264_ : nullptr);
    stream().writeInt(f312_);
    stream().writeString(f272_ ? &*f272_ : nullptr);
    stream().writeInt(static_cast<i32>(list296_.size()));
    for (const auto& s : list296_) stream().writeString(&s);
    stream().writeInt(static_cast<i32>(list304_.size()));
    for (const auto& s : list304_) stream().writeString(&s);
    stream().writeVInt(f332_);
    if (!compressed_) throw pending_reverse("LoginOkMessage needs LogicCompressedString");
    compressed_->encode(stream());
    stream().writeBoolean(f328_);
    stream().writeBoolean(!f329not_);
    stream().writeString(f280_ ? &*f280_ : nullptr);
    stream().writeString(f336_ ? &*f336_ : nullptr);
    stream().writeString(f288_ ? &*f288_ : nullptr);
    stream().writeString(f344_ ? &*f344_ : nullptr);
    stream().writeString(f352_ ? &*f352_ : nullptr);
    stream().writeBoolean(f360_);
}

inline void LoginOkMessage::decode() {
PiranhaMessage::decode();
    accountId_ = LogicLong::decode(stream());
    homeId_ = LogicLong::decode(stream());
    passToken_ = stream().readString();
    f160_ = stream().readString();
    f168_ = stream().readString();
    f192_ = stream().readInt();
    f196_ = stream().readInt();
    f200_ = stream().readInt();
    f208_ = stream().readString();
    f216_ = stream().readInt();
    f220_ = stream().readInt();
    f224_ = stream().readInt();
    f184_ = stream().readString();
    if (stream().isAtEnd()) return;
    f232_ = stream().readString();
    f240_ = stream().readString();
    f248_ = stream().readInt();
    hasTail1_ = true;
    if (stream().isAtEnd()) return;
    f176_ = stream().readString();
    f256_ = stream().readString();
    f264_ = stream().readString();
    f312_ = stream().readInt();
    hasTail2_ = true;
    if (stream().isAtEnd()) return;
    f272_ = stream().readString();
    list296_ = readCappedStringList(stream());
    list304_ = readCappedStringList(stream());
    hasLists_ = true;
    if (stream().isAtEnd()) return;
    f332_ = stream().readVInt();
    hasF332_ = true;
    if (stream().isAtEnd()) return;
    compressed_ = std::make_unique<LogicCompressedString>();
    compressed_->decode(stream());
    hasCompressed_ = true;
    if (stream().isAtEnd()) return;
    f328_ = stream().readBoolean();
    f329not_ = !stream().readBoolean();
    hasFlags_ = true;
    if (stream().isAtEnd()) return;
    f280_ = stream().readString();
    f336_ = stream().readString();
    f288_ = stream().readString();
    f344_ = stream().readString();
    f352_ = stream().readString();
    f360_ = stream().readBoolean();
    hasTail3_ = true;
}

} // namespace titan
