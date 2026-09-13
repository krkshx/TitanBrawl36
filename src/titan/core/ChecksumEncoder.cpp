#pragma once
#include <cstdint>
#include <string>

struct LogicLong {
    std::int32_t high = 0;
    std::int32_t low = 0;
};

class ChecksumEncoder {
public:
    ChecksumEncoder() = default;
    virtual ~ChecksumEncoder() = default;
    virtual void destruct() {}
    virtual bool isCheckSumOnlyMode() const { return true; }
    virtual void writeStringReference(const std::string &v) {
        checksum_ = static_cast<std::int32_t>(static_cast<std::uint32_t>(static_cast<std::int32_t>(v.length()) + rotate(checksum_)) + 38u);
    }
    virtual void writeFilteredStringReference(const std::string &v) {
        writeStringReference(v);
    }
    virtual void writeFilteredString(const std::string *v) {
        writeString(v);
    }
    virtual void writeString(const std::string *v) {
        std::uint32_t base = static_cast<std::uint32_t>(rotate(checksum_));
        if (v == nullptr) {
            checksum_ = static_cast<std::int32_t>(base + 27u);
        } else {
            checksum_ = static_cast<std::int32_t>(base + static_cast<std::uint32_t>(v->length()) + 28u);
        }
    }
    virtual bool writeBoolean(bool v) {
        checksum_ = static_cast<std::int32_t>(static_cast<std::uint32_t>(rotate(checksum_)) + static_cast<std::uint32_t>(v ? 13 : 7));
        return v;
    }
    virtual void writeInt(std::int32_t v) {
        checksum_ = static_cast<std::int32_t>(static_cast<std::uint32_t>(v) + static_cast<std::uint32_t>(rotate(checksum_)) + 9u);
    }
    virtual void writeInt8(std::int32_t v) {
        checksum_ = static_cast<std::int32_t>(static_cast<std::uint32_t>(v) + static_cast<std::uint32_t>(rotate(checksum_)) + 11u);
    }
    virtual void writeInt16(std::int32_t v) {
        checksum_ = static_cast<std::int32_t>(static_cast<std::uint32_t>(v) + static_cast<std::uint32_t>(rotate(checksum_)) + 19u);
    }
    virtual void writeInt24(std::int32_t v) {
        checksum_ = static_cast<std::int32_t>(static_cast<std::uint32_t>(v) + static_cast<std::uint32_t>(rotate(checksum_)) + 21u);
    }
    virtual void writeBytes(const char *v, std::int32_t n) {
        std::uint32_t base = static_cast<std::uint32_t>(rotate(checksum_));
        if (v == nullptr) {
            checksum_ = static_cast<std::int32_t>(base + 37u);
        } else {
            checksum_ = static_cast<std::int32_t>(base + static_cast<std::uint32_t>(n) + 38u);
        }
    }
    virtual void writeByte(char v) {
        checksum_ = static_cast<std::int32_t>(static_cast<std::uint32_t>(v) + static_cast<std::uint32_t>(rotate(checksum_)) + 11u);
    }
    virtual void writeShort(std::int32_t v) {
        checksum_ = static_cast<std::int32_t>(static_cast<std::uint32_t>(v) + static_cast<std::uint32_t>(rotate(checksum_)) + 19u);
    }
    virtual void writeVInt(std::int32_t v) {
        checksum_ = static_cast<std::int32_t>(static_cast<std::uint32_t>(v) + static_cast<std::uint32_t>(rotate(checksum_)) + 33u);
    }
    virtual void writeVLong(std::int64_t v) {
        std::int32_t high = static_cast<std::int32_t>(v >> 32);
        std::int32_t low = static_cast<std::int32_t>(v & 0xFFFFFFFFll);
        writeVInt(high);
        writeVInt(low);
        checksum_ = static_cast<std::int32_t>(static_cast<std::uint32_t>(high) + static_cast<std::uint32_t>(rotate(checksum_)) + 65u);
        checksum_ = static_cast<std::int32_t>(static_cast<std::uint32_t>(low) + static_cast<std::uint32_t>(rotate(checksum_)) + 88u);
    }
    virtual void writeLongLong(std::int64_t v) {
        std::int32_t high = static_cast<std::int32_t>(v >> 32);
        std::int32_t low = static_cast<std::int32_t>(v & 0xFFFFFFFFll);
        checksum_ = static_cast<std::int32_t>(static_cast<std::uint32_t>(high) + static_cast<std::uint32_t>(rotate(checksum_)) + 67u);
        checksum_ = static_cast<std::int32_t>(static_cast<std::uint32_t>(low) + static_cast<std::uint32_t>(rotate(checksum_)) + 91u);
    }
    virtual void writeLong(const LogicLong *v) {
        writeInt(v->high);
        writeInt(v->low);
    }
    virtual bool isByteStream() const { return false; }
    std::int32_t getChecksum() const { return checksum_; }
    void setChecksum(std::int32_t v) { checksum_ = v; }
private:
    static std::int32_t rotate(std::int32_t v) {
        return static_cast<std::int32_t>(static_cast<std::uint32_t>(v) >> 31);
    }
    std::int32_t checksum_ = 0;
};
