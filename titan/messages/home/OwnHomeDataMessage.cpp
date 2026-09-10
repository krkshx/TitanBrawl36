#pragma once

// OwnHomeDataMessage (24101) — split out of MsgBatch07.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/home/Home.cpp"

namespace titan {

class OwnHomeDataMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 24101; }
    const char* getMessageTypeName() const override { return "OwnHomeDataMessage"; }
    void encode() override;
    void decode() override;
    std::unique_ptr<LogicClientHome> home_;
    std::unique_ptr<LogicClientAvatar> avatar_;
    i32 f152_ = 0;
};

} // namespace titan

// OwnHomeDataMessage bodies — split out of MsgBatch07.cpp; wire format unchanged.


namespace titan {

inline void OwnHomeDataMessage::encode() {
PiranhaMessage::encode();
    if (!home_) throw pending_reverse("OwnHomeDataMessage needs LogicClientHome");
    home_->encode(stream());
    if (!avatar_) throw pending_reverse("OwnHomeDataMessage needs LogicClientAvatar");
    avatar_->encode(stream());
    stream().writeVInt(f152_);
}

inline void OwnHomeDataMessage::decode() {
PiranhaMessage::decode();
    home_ = std::make_unique<LogicClientHome>();
    home_->decode(stream());
    avatar_ = std::make_unique<LogicClientAvatar>();
    avatar_->decode(stream());
    f152_ = stream().readVInt();
}


} // namespace titan
