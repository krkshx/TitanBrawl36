#pragma once

// FriendSuggestionsMessage (20199) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.cpp"
#include "titan/game/SuggestionEntry.hpp"

namespace titan {

class FriendSuggestionsMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 20199; }
    const char* getMessageTypeName() const override { return "FriendSuggestionsMessage"; }
    void encode() override;
    void decode() override;
    std::vector<std::unique_ptr<FriendSuggestionEntry>> suggestions_;
};

} // namespace titan

// FriendSuggestionsMessage bodies — split out of MsgBatch04.cpp; wire format unchanged.


namespace titan {

inline void FriendSuggestionsMessage::encode() {
PiranhaMessage::encode();
    stream().writeInt(static_cast<i32>(suggestions_.size()));
    for (const auto& s : suggestions_) s->encode(stream());
}

inline void FriendSuggestionsMessage::decode() {
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
