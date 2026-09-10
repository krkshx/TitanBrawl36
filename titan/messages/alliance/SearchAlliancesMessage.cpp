#pragma once

// SearchAlliancesMessage (14324) — split out of MsgBatch09.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"

namespace titan {

class SearchAlliancesMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 14324; }
    const char* getMessageTypeName() const override { return "SearchAlliancesMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        stream().writeString(text_ ? &*text_ : nullptr);
        for (i32 x : ints_) stream().writeInt(x);
        stream().writeBoolean(flag_);
        stream().writeInt(p168_);
        stream().writeInt(p160_);
    }
    void decode() override {
        PiranhaMessage::decode();
        text_ = stream().readString();
        for (i32& x : ints_) x = stream().readInt();
        flag_ = stream().readBoolean();
        p168_ = stream().readInt();
        p160_ = stream().readInt();
    }
    std::optional<std::string> text_;
    i32 ints_[4] = {};
    bool flag_ = false;
    i32 p168_ = 0, p160_ = 0;
};

} // namespace titan
