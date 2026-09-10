#pragma once

// Shared scaffolding for not-yet-reversed nested entries.
// Every nested class (AllianceHeaderEntry, LogicCommand, ...) is declared
// via TITAN_PENDING_ENTRY as a NestedEntry whose encode/decode throw
// pending_reverse until its own reverse wave replaces the body.
// Empty arrays (count 0) round-trip bit-exact regardless.

#include "titan/core/ByteStream.cpp"

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace titan {

struct pending_reverse : std::logic_error {
    explicit pending_reverse(const std::string& what) : std::logic_error(what) {}
};

class NestedEntry {
public:
    virtual ~NestedEntry() = default;
    virtual void encode(ByteStream& s) const = 0;
    virtual void decode(ByteStream& s) = 0;
};

#define TITAN_PENDING_ENTRY(Name)                                              \
    class Name : public NestedEntry {                                          \
    public:                                                                    \
        void encode(ByteStream&) const override {                              \
            throw pending_reverse(#Name " pending reverse");                   \
        }                                                                      \
        void decode(ByteStream&) override {                                    \
            throw pending_reverse(#Name " pending reverse");                   \
        }                                                                      \
    }

} // namespace titan
