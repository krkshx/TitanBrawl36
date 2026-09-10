#pragma once

// SuggestionEntry (the binary name behind FriendSuggestionEntry):
// encode @0x1985e0, decode @0x8d0e6c.
// Wire: int +0, FriendEntry +8. Split out of the entry wave.

#include "titan/game/social/FriendEntry.cpp"
#include "titan/messages/Nested.cpp"

#include <memory>

namespace titan {

class SuggestionEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeInt(v0_);
        if (!entry_) throw pending_reverse("SuggestionEntry needs FriendEntry");
        entry_->encode(s);
    }
    void decode(ByteStream& s) override {
        v0_ = s.readInt();
        entry_ = std::make_unique<FriendEntry>();
        entry_->decode(s);
    }
    i32 v0_ = 0;
    std::unique_ptr<FriendEntry> entry_;
};

// Alias used by FriendSuggestionsMessage.
using FriendSuggestionEntry = SuggestionEntry;

} // namespace titan
