#pragma once

// BaseNotification::encode @0x7beae0, decode @0x605478.
// Wire: int @+8, bool @+12, int @+16, string (nullable String* @+24).
// getNotificationType @0x7680bc is virtual; concrete subclasses return
// their factory id (e.g. FreeTextNotification -> 81 @0x9fcd44).
// Our port carries the id in type_ (not wired — the id travels as the
// ClientHome pair key, exactly like the binary).

#include "titan/messages/Nested.hpp"

#include <optional>
#include <string>

namespace titan {

class BaseNotification : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeInt(v8_);
        s.writeBoolean(b12_);
        s.writeInt(v16_);
        s.writeString(s24_.has_value() ? &s24_.value() : nullptr);
    }
    void decode(ByteStream& s) override {
        v8_ = s.readInt();
        b12_ = s.readBoolean();
        v16_ = s.readInt();
        s24_ = s.readString();
    }
    [[nodiscard]] virtual int notificationType() const { return type_; }

    i32 v8_ = 0;
    bool b12_ = false;
    i32 v16_ = 0;
    std::optional<std::string> s24_; // +24

protected:
    int type_ = 0; // factory id, not wired
};

} // namespace titan
