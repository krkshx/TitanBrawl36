#pragma once

// FriendEntry::encode @0x7c4e4c, decode @0x397ae8.
// Wire: logiclong, 6 strings, 5 ints, optional club block
// (bool + logiclong + int + string + int + int), string, 2 ints,
// optional PlayerDisplayData. Split out of the entry wave.

#include "titan/core/LogicLong.hpp"
#include "titan/game/PlayerDisplayData.hpp"
#include "titan/messages/Nested.hpp"

#include <memory>
#include <optional>
#include <string>

namespace titan {

class FriendEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        id_.encode(s);
        for (const auto* str : {&s1_, &s2_, &s3_, &s4_, &s5_, &s6_})
            s.writeString(*str ? &**str : nullptr);
        for (i32 x : ints_) s.writeInt(x);
        s.writeBoolean(hasClub_);
        if (hasClub_) {
            clubId_.encode(s);
            s.writeInt(clubV96_);
            s.writeString(clubName_ ? &*clubName_ : nullptr);
            s.writeInt(clubV112_);
            s.writeInt(clubV116_);
        }
        s.writeString(s120_ ? &*s120_ : nullptr);
        s.writeInt(v76_);
        s.writeInt(v80_);
        s.writeBoolean(hasDisplay_);
        if (hasDisplay_) {
            if (!display_) throw pending_reverse("FriendEntry needs PlayerDisplayData");
            display_->encode(s);
        }
    }
    void decode(ByteStream& s) override {
        id_ = LogicLong::decode(s);
        s1_ = s.readString();
        s2_ = s.readString();
        s3_ = s.readString();
        s4_ = s.readString();
        s5_ = s.readString();
        s6_ = s.readString();
        for (i32& x : ints_) x = s.readInt();
        hasClub_ = s.readBoolean();
        if (hasClub_) {
            clubId_ = LogicLong::decode(s);
            clubV96_ = s.readInt();
            clubName_ = s.readString();
            clubV112_ = s.readInt();
            clubV116_ = s.readInt();
        }
        s120_ = s.readString();
        v76_ = s.readInt();
        v80_ = s.readInt();
        hasDisplay_ = s.readBoolean();
        if (hasDisplay_) {
            display_ = std::make_unique<PlayerDisplayData>();
            display_->decode(s);
        }
    }
    LogicLong id_, clubId_;
    std::optional<std::string> s1_, s2_, s3_, s4_, s5_, s6_;
    i32 ints_[5] = {};
    bool hasClub_ = false;
    i32 clubV96_ = 0, clubV112_ = 0, clubV116_ = 0;
    std::optional<std::string> clubName_, s120_;
    i32 v76_ = 0, v80_ = 0;
    bool hasDisplay_ = false;
    std::unique_ptr<PlayerDisplayData> display_;
};

} // namespace titan
