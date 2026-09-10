#pragma once

// SCIDAccountAlreadyBoundMessage (28689) — split out of MsgBatch08.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class SCIDAccountAlreadyBoundMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 28689; }
    const char* getMessageTypeName() const override { return "SCIDAccountAlreadyBoundMessage"; }
    void encode() override;
    void decode() override;
    std::optional<std::string> token_, name_, extra_;
    bool hasPlayerId_ = false;
    LogicLong playerId_;
    std::unique_ptr<LogicClientAvatar> avatar_;
};

} // namespace titan

// SCIDAccountAlreadyBoundMessage bodies — split out of MsgBatch08.cpp; wire format unchanged.


namespace titan {

inline void SCIDAccountAlreadyBoundMessage::encode() {
PiranhaMessage::encode();
    stream().writeString(token_ ? &*token_ : nullptr);
    stream().writeBoolean(hasPlayerId_);
    if (hasPlayerId_) playerId_.encode(stream());
    stream().writeString(name_ ? &*name_ : nullptr);
    if (!avatar_) throw pending_reverse("SCIDAccountAlreadyBoundMessage needs LogicClientAvatar");
    avatar_->encode(stream());
    stream().writeString(extra_ ? &*extra_ : nullptr);
}

inline void SCIDAccountAlreadyBoundMessage::decode() {
PiranhaMessage::decode();
    token_ = stream().readString();
    hasPlayerId_ = stream().readBoolean();
    if (hasPlayerId_) playerId_ = LogicLong::decode(stream());
    name_ = stream().readString();
    avatar_ = std::make_unique<LogicClientAvatar>();
    avatar_->decode(stream());
    extra_ = stream().readString();
}


} // namespace titan
