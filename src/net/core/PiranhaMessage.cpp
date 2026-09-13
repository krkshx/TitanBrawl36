#pragma once
#include "../../titan/core/ByteStream.cpp"
#include <cstdint>

class PiranhaMessage {
public:
    PiranhaMessage() = default;
    virtual ~PiranhaMessage() = default;
    virtual std::int32_t id() const { return 0; }
    virtual void encode() {}
    virtual void decode() {}
    ByteStream &stream() { return stream_; }
    const ByteStream &stream() const { return stream_; }
    std::int32_t version() const { return version_; }
    void setVersion(std::int32_t v) { version_ = v; }
private:
    ByteStream stream_;
    std::int32_t version_ = 0;
};
