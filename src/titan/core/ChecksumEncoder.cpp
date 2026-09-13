#pragma once
#include <cstdint>

class ChecksumEncoder {
public:
    ChecksumEncoder() = default;
    virtual ~ChecksumEncoder() = default;
    virtual void writeInt(std::int32_t value) {
        std::uint32_t s = static_cast<std::uint32_t>(checksum_);
        checksum_ = value + static_cast<std::int32_t>(s >> 31) + 9;
    }
    virtual void writeVInt(std::int32_t value) {
        std::uint32_t s = static_cast<std::uint32_t>(checksum_);
        checksum_ = value + static_cast<std::int32_t>(s >> 31) + 33;
    }
    virtual bool writeBoolean(bool value) {
        std::uint32_t s = static_cast<std::uint32_t>(checksum_);
        std::int32_t add = value ? 13 : 7;
        checksum_ = static_cast<std::int32_t>(s >> 31) + add;
        return value;
    }
    std::int32_t checksum() const { return checksum_; }
    void setChecksum(std::int32_t v) { checksum_ = v; }
private:
    std::int32_t checksum_ = 0;
};
