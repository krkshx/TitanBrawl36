#pragma once

// TeamToggleMemberSideMessage (14357) — split out of MsgBatch11.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class TeamToggleMemberSideMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14357; }
    const char* getMessageTypeName() const override { return "TeamToggleMemberSideMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeVInt(static_cast<i32>(memberIds_.size()));
        for (const auto& id : memberIds_) id.encode(stream());
        stream().writeVInt(v152_);
    }
    void decode() override {
        PiranhaMessage::decode();
        const i32 n = stream().readVInt();
        memberIds_.clear();
        for (i32 i = 0; i < n; ++i) memberIds_.push_back(LogicLong::decode(stream()));
        v152_ = stream().readVInt();
    }
    std::vector<LogicLong> memberIds_;
    i32 v152_ = 0;
};

} // namespace titan
