#pragma once

// UnbotifyReportMessage (17337) — split out of MsgBatch12.hpp; wire format unchanged.

#include "titan/messages/MessagePrelude.hpp"

namespace titan {

class UnbotifyReportMessage : public PiranhaMessage {
public:
    int getMessageType() const override { return 17337; }
    const char* getMessageTypeName() const override { return "UnbotifyReportMessage"; }
    void encode() override {
        PiranhaMessage::encode();
        if (report_) {
            const auto& p = *report_;
            stream().writeBytes(p.data(), static_cast<i32>(p.size()));
        } else {
            stream().writeBytes(nullptr, 0);
        }
    }
    void decode() override {
        PiranhaMessage::decode();
        report_ = stream().readBytesNullable();
    }
    std::optional<std::vector<u8>> report_;
};

} // namespace titan
