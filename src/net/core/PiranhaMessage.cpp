#pragma once
#include "../../titan/core/ByteStream.cpp"

class PiranhaMessage {
public:
    explicit PiranhaMessage(std::int32_t version = 0) : version_(version) {}
    virtual ~PiranhaMessage() = default;
    virtual void encode() {}
    virtual void decode() {}
    virtual std::int32_t getServiceNodeType() const = 0;
    virtual std::int32_t getMessageType() const = 0;
    virtual const char *getMessageTypeName() const { return "PiranhaMessage"; }
    virtual void destruct() {}
    void setMessageVersion(std::int32_t v) { version_ = v; }
    std::int32_t getMessageVersion() const { return version_; }
    ByteStream *getByteStream() { return &stream_; }
    const ByteStream *getByteStream() const { return &stream_; }
    std::int32_t getEncodingLength() const { return stream_.getOffset(); }
    const char *getMessageBytes() const { return stream_.getByteArray(); }
private:
    ByteStream stream_;
    std::int32_t version_ = 0;
};
