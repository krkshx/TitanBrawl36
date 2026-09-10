#pragma once

// TencentAccountAlreadyBoundMessage (24221) — split out of MsgBatch11.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class TencentAccountAlreadyBoundMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24221; }
    const char* getMessageTypeName() const override { return "TencentAccountAlreadyBoundMessage"; }
    void encode() override;
    void decode() override;
    std::optional<std::string> id_, name_;
    bool hasAvatarId_ = false;
    LogicLong avatarId_;
    std::unique_ptr<LogicClientAvatar> avatar_;
};

} // namespace titan

// TencentAccountAlreadyBoundMessage bodies — split out of MsgBatch11.cpp; wire format unchanged.


namespace titan {

inline void TencentAccountAlreadyBoundMessage::encode() {
PiranhaMessage::encode();
    stream().writeString(id_ ? &*id_ : nullptr);
    stream().writeBoolean(hasAvatarId_);
    if (hasAvatarId_) avatarId_.encode(stream());
    stream().writeString(name_ ? &*name_ : nullptr);
    if (!avatar_) throw pending_reverse("TencentAccountAlreadyBoundMessage needs LogicClientAvatar");
    avatar_->encode(stream());
}

inline void TencentAccountAlreadyBoundMessage::decode() {
PiranhaMessage::decode();
    id_ = stream().readString();
    hasAvatarId_ = stream().readBoolean();
    if (hasAvatarId_) avatarId_ = LogicLong::decode(stream());
    name_ = stream().readString();
    avatar_ = std::make_unique<LogicClientAvatar>();
    avatar_->decode(stream());
}


} // namespace titan
