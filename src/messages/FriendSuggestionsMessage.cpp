// FriendSuggestionsMessage bodies — split out of MsgBatch04.cpp; wire format unchanged.

#include "titan/messages/FriendSuggestionsMessage.hpp"

namespace titan {

void FriendSuggestionsMessage::encode() {
    PiranhaMessage::encode();
    stream().writeInt(static_cast<i32>(suggestions_.size()));
    for (const auto& s : suggestions_) s->encode(stream());
}

void FriendSuggestionsMessage::decode() {
    PiranhaMessage::decode();
    const i32 n = stream().readInt();
    suggestions_.clear();
    for (i32 i = 0; i < n; ++i) {
        auto s = std::make_unique<FriendSuggestionEntry>();
        s->decode(stream());
        suggestions_.push_back(std::move(s));
    }
}


} // namespace titan
