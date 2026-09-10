#pragma once

// FriendSuggestionsMessage (20199) — split out of MsgBatch04.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"
#include "titan/messages/pending/FriendSuggestionEntry.hpp"

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
