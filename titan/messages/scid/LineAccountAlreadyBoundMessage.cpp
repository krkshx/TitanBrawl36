#pragma once

// LineAccountAlreadyBoundMessage (24205) — split out of MsgBatch06.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class LineAccountAlreadyBoundMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24205; }
    const char* getMessageTypeName() const override { return "LineAccountAlreadyBoundMessage"; }
    void encode() override;
    void decode() override;
    std::optional<std::string> id_, name_;
    bool hasAvatarId_ = false;
    LogicLong avatarId_;
    std::unique_ptr<LogicClientAvatar> avatar_;
};

} // namespace titan

// LineAccountAlreadyBoundMessage bodies — split out of MsgBatch06.cpp; wire format unchanged.


namespace titan {

inline void LineAccountAlreadyBoundMessage::encode() {
PiranhaMessage::encode();
    stream().writeString(id_ ? &*id_ : nullptr);
    stream().writeBoolean(hasAvatarId_);
    if (hasAvatarId_) avatarId_.encode(stream());
    stream().writeString(name_ ? &*name_ : nullptr);
    if (!avatar_) throw pending_reverse("LineAccountAlreadyBoundMessage needs LogicClientAvatar");
    avatar_->encode(stream());
}

inline void LineAccountAlreadyBoundMessage::decode() {
PiranhaMessage::decode();
    id_ = stream().readString();
    hasAvatarId_ = stream().readBoolean();
    if (hasAvatarId_) avatarId_ = LogicLong::decode(stream());
    name_ = stream().readString();
    avatar_ = std::make_unique<LogicClientAvatar>();
    avatar_->decode(stream());
}


} // namespace titan
