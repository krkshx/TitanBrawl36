#pragma once
#include "ChecksumEncoder.cpp"
#include <cstdint>
#include <string>
#include <vector>

class ByteStream : public ChecksumEncoder {
public:
    explicit ByteStream(std::size_t capacity = 128) { buffer_.reserve(capacity); }
    ByteStream(const char *data, std::int32_t length) { setByteArray(data, length); }

    void destruct() override {
        buffer_.clear();
        buffer_.shrink_to_fit();
        length_ = 0;
        offset_ = 0;
        bitOffset_ = 0;
    }

    bool isCheckSumOnlyMode() const override { return false; }

    void writeStringReference(const std::string &v) override {
        ChecksumEncoder::writeStringReference(v);
        writeRaw(v.data(), static_cast<std::int32_t>(v.size()));
    }

    void writeFilteredStringReference(const std::string &v) override {
        writeStringReference(v);
    }

    void writeFilteredString(const std::string *v) override {
        writeString(v);
    }

    void writeString(const std::string *v) override {
        ChecksumEncoder::writeString(v);
        bitOffset_ = 0;
        if (v == nullptr || v->size() > 900000) {
            writeIntToByteArray(-1);
            return;
        }
        writeRaw(v->data(), static_cast<std::int32_t>(v->size()));
    }

    bool writeBoolean(bool v) override {
        ChecksumEncoder::writeBoolean(v);
        if (bitOffset_ == 0) {
            ensure(1);
            buffer_.push_back(0);
        }
        if (v) {
            buffer_.back() = static_cast<std::uint8_t>(buffer_.back() | (1 << bitOffset_));
        }
        bitOffset_ = (bitOffset_ + 1) & 7;
        length_ = static_cast<std::int32_t>(buffer_.size());
        return v;
    }

    void writeInt(std::int32_t v) override {
        ChecksumEncoder::writeInt(v);
        bitOffset_ = 0;
        writeIntToByteArray(v);
    }

    void writeInt8(std::int32_t v) override {
        ChecksumEncoder::writeInt8(v);
        bitOffset_ = 0;
        ensure(1);
        buffer_.push_back(static_cast<std::uint8_t>(v & 0xFF));
        length_ = static_cast<std::int32_t>(buffer_.size());
    }

    void writeInt16(std::int32_t v) override {
        ChecksumEncoder::writeInt16(v);
        bitOffset_ = 0;
        ensure(2);
        buffer_.push_back(static_cast<std::uint8_t>((v >> 8) & 0xFF));
        buffer_.push_back(static_cast<std::uint8_t>(v & 0xFF));
        length_ = static_cast<std::int32_t>(buffer_.size());
    }

    void writeInt24(std::int32_t v) override {
        ChecksumEncoder::writeInt24(v);
        bitOffset_ = 0;
        ensure(3);
        buffer_.push_back(static_cast<std::uint8_t>((v >> 16) & 0xFF));
        buffer_.push_back(static_cast<std::uint8_t>((v >> 8) & 0xFF));
        buffer_.push_back(static_cast<std::uint8_t>(v & 0xFF));
        length_ = static_cast<std::int32_t>(buffer_.size());
    }

    void writeBytes(const char *v, std::int32_t n) override {
        ChecksumEncoder::writeBytes(v, n);
        bitOffset_ = 0;
        if (v == nullptr) {
            writeIntToByteArray(-1);
            return;
        }
        writeRaw(v, n);
    }

    void writeByte(char v) override {
        ChecksumEncoder::writeByte(v);
        bitOffset_ = 0;
        ensure(1);
        buffer_.push_back(static_cast<std::uint8_t>(v));
        length_ = static_cast<std::int32_t>(buffer_.size());
    }

    void writeShort(std::int32_t v) override {
        ChecksumEncoder::writeShort(v);
        bitOffset_ = 0;
        ensure(2);
        buffer_.push_back(static_cast<std::uint8_t>((v >> 8) & 0xFF));
        buffer_.push_back(static_cast<std::uint8_t>(v & 0xFF));
        length_ = static_cast<std::int32_t>(buffer_.size());
    }

    void writeVInt(std::int32_t v) override {
        ChecksumEncoder::writeVInt(v);
        ensure(5);
        bitOffset_ = 0;
        writeVIntRaw(static_cast<std::uint32_t>(v), v < 0);
    }

    void writeVLong(std::int64_t v) override {
        writeVInt(static_cast<std::int32_t>(v >> 32));
        writeVInt(static_cast<std::int32_t>(v & 0xFFFFFFFFll));
        ChecksumEncoder::writeVLong(v);
    }

    void writeLongLong(std::int64_t v) override {
        ChecksumEncoder::writeLongLong(v);
        bitOffset_ = 0;
        ensure(8);
        std::uint64_t u = static_cast<std::uint64_t>(v);
        for (int i = 7; i >= 0; i--) {
            buffer_.push_back(static_cast<std::uint8_t>((u >> (i * 8)) & 0xFF));
        }
        length_ = static_cast<std::int32_t>(buffer_.size());
    }

    void writeLong(const LogicLong *v) override {
        writeInt(v->high);
        writeInt(v->low);
    }

    bool isByteStream() const override { return true; }

    virtual std::string readStringReference(std::int32_t maxLength) {
        bitOffset_ = 0;
        std::int32_t n = readIntFromByteArray();
        if (n < 0) {
            return std::string();
        }
        return readRawString(n, maxLength);
    }

    virtual std::string readString(std::int32_t maxLength) {
        bitOffset_ = 0;
        std::int32_t n = readIntFromByteArray();
        if (n == -1) {
            return std::string();
        }
        if (n < -1) {
            return std::string();
        }
        return readRawString(n, maxLength);
    }

    virtual std::string readString(std::string &v, std::int32_t maxLength) {
        v = readString(maxLength);
        return v;
    }

    virtual std::string readFilteredString(std::string &v, std::int32_t maxLength) {
        return readString(v, maxLength);
    }

    virtual std::string readFilteredStringReference(std::int32_t maxLength) {
        return readStringReference(maxLength);
    }

    virtual bool readBoolean() {
        if (bitOffset_ == 0) {
            if (offset_ >= length_ || offset_ >= static_cast<std::int32_t>(buffer_.size())) {
                return false;
            }
            offset_++;
        }
        if (offset_ <= 0 || offset_ > length_ || offset_ > static_cast<std::int32_t>(buffer_.size())) {
            return false;
        }
        bool v = (buffer_[static_cast<std::size_t>(offset_ - 1)] & (1 << bitOffset_)) != 0;
        bitOffset_ = (bitOffset_ + 1) & 7;
        return v;
    }

    virtual std::int32_t readInt() {
        bitOffset_ = 0;
        return readIntFromByteArray();
    }

    virtual std::int32_t readInt8() {
        bitOffset_ = 0;
        if (!canRead(1)) {
            return 0;
        }
        return static_cast<std::int8_t>(buffer_[static_cast<std::size_t>(offset_++)]);
    }

    virtual std::int32_t readInt16() {
        bitOffset_ = 0;
        if (!canRead(2)) {
            return 0;
        }
        std::int32_t b0 = buffer_[static_cast<std::size_t>(offset_++)];
        std::int32_t b1 = buffer_[static_cast<std::size_t>(offset_++)];
        return static_cast<std::int16_t>((b0 << 8) | b1);
    }

    virtual std::int32_t readInt24() {
        bitOffset_ = 0;
        if (!canRead(3)) {
            return 0;
        }
        std::int32_t b0 = buffer_[static_cast<std::size_t>(offset_++)];
        std::int32_t b1 = buffer_[static_cast<std::size_t>(offset_++)];
        std::int32_t b2 = buffer_[static_cast<std::size_t>(offset_++)];
        std::int32_t v = (b0 << 16) | (b1 << 8) | b2;
        if (v & 0x800000) {
            v |= ~0xFFFFFF;
        }
        return v;
    }

    virtual std::int32_t readShort() {
        bitOffset_ = 0;
        if (!canRead(2)) {
            return 0;
        }
        std::int32_t b0 = buffer_[static_cast<std::size_t>(offset_++)];
        std::int32_t b1 = buffer_[static_cast<std::size_t>(offset_++)];
        return static_cast<std::int16_t>((b0 << 8) | b1);
    }

    virtual std::int32_t readByte() {
        bitOffset_ = 0;
        if (!canRead(1)) {
            return 0;
        }
        return static_cast<std::int32_t>(buffer_[static_cast<std::size_t>(offset_++)]);
    }

    virtual std::int32_t readVInt() {
        bitOffset_ = 0;
        if (!canRead(1)) {
            return 0;
        }
        std::uint8_t b0 = buffer_[static_cast<std::size_t>(offset_++)];
        std::int32_t r = b0 & 0x3F;
        if ((b0 & 0x40) != 0) {
            if ((b0 & 0x80) != 0) {
                if (!canRead(1)) {
                    return static_cast<std::int32_t>(static_cast<std::uint32_t>(r) | 0xFFFFE000u);
                }
                std::uint8_t b1 = buffer_[static_cast<std::size_t>(offset_++)];
                r = (r & 0xFFFFE03F) | ((b1 & 0x7F) << 6);
                if ((b1 & 0x80) != 0) {
                    if (!canRead(1)) {
                        return static_cast<std::int32_t>(static_cast<std::uint32_t>(r) | 0xFFFFE000u);
                    }
                    std::uint8_t b2 = buffer_[static_cast<std::size_t>(offset_++)];
                    std::uint32_t t = (static_cast<std::uint32_t>(r) & 0xFFF01FFF) | static_cast<std::uint32_t>((b2 & 0x7F) << 13);
                    if ((b2 & 0x80) != 0) {
                        if (!canRead(1)) {
                            return static_cast<std::int32_t>(t | 0xFFF00000u);
                        }
                        std::uint8_t b3 = buffer_[static_cast<std::size_t>(offset_++)];
                        std::uint32_t t2 = (t & 0xF80FFFFF) | static_cast<std::uint32_t>((b3 & 0x7F) << 20);
                        if ((b3 & 0x80) != 0) {
                            if (!canRead(1)) {
                                return static_cast<std::int32_t>(t2 | 0xF8000000u);
                            }
                            std::uint8_t b4 = buffer_[static_cast<std::size_t>(offset_++)];
                            return static_cast<std::int32_t>((t2 & 0x7FFFFFF) | (static_cast<std::uint32_t>(b4) << 27) | 0x80000000u);
                        }
                        return static_cast<std::int32_t>(t2 | 0xF8000000u);
                    }
                    return static_cast<std::int32_t>(t | 0xFFF00000u);
                }
                return static_cast<std::int32_t>(static_cast<std::uint32_t>(r) | 0xFFFFE000u);
            }
            return static_cast<std::int32_t>(static_cast<std::uint32_t>(b0) | 0xFFFFFFC0u);
        }
        if ((b0 & 0x80) != 0) {
            if (!canRead(1)) {
                return r;
            }
            std::uint8_t b1 = buffer_[static_cast<std::size_t>(offset_++)];
            r = (r & 0xFFFFE03F) | ((b1 & 0x7F) << 6);
            if ((b1 & 0x80) != 0) {
                if (!canRead(1)) {
                    return r;
                }
                std::uint8_t b2 = buffer_[static_cast<std::size_t>(offset_++)];
                r = (r & 0xFFF01FFF) | ((b2 & 0x7F) << 13);
                if ((b2 & 0x80) != 0) {
                    if (!canRead(1)) {
                        return r;
                    }
                    std::uint8_t b3 = buffer_[static_cast<std::size_t>(offset_++)];
                    r = (r & 0xF80FFFFF) | ((b3 & 0x7F) << 20);
                    if ((b3 & 0x80) != 0) {
                        if (!canRead(1)) {
                            return r;
                        }
                        std::uint8_t b4 = buffer_[static_cast<std::size_t>(offset_++)];
                        r = (r & 0x7FFFFFF) | (b4 << 27);
                    }
                }
            }
        }
        return r;
    }

    virtual std::int64_t readVLong() {
        std::int64_t high = readVInt();
        std::int64_t low = static_cast<std::uint32_t>(readVInt());
        return (high << 32) | low;
    }

    virtual std::int64_t readLongLong() {
        std::vector<std::uint8_t> bytes = readBytes(8, 8);
        std::uint64_t u = 0;
        for (std::size_t i = 0; i < bytes.size(); i++) {
            u = (u << 8) | bytes[i];
        }
        return static_cast<std::int64_t>(u);
    }

    virtual std::int64_t readLong() {
        return readLongLong();
    }

    virtual void readLong(LogicLong *v) {
        v->high = readInt();
        v->low = readInt();
    }

    virtual void writeBytesWithoutLength(const char *v, std::int32_t n) {
        bitOffset_ = 0;
        ensure(n);
        for (std::int32_t i = 0; i < n; i++) {
            buffer_.push_back(static_cast<std::uint8_t>(v[i]));
        }
        length_ = static_cast<std::int32_t>(buffer_.size());
    }

    virtual std::int32_t readBytesLength() {
        bitOffset_ = 0;
        return readIntFromByteArray();
    }

    virtual std::vector<std::uint8_t> readBytes(std::int32_t length, std::int32_t maxLength) {
        bitOffset_ = 0;
        std::vector<std::uint8_t> out;
        if (length < 0 || length > maxLength || !canRead(length)) {
            return out;
        }
        out.reserve(static_cast<std::size_t>(length));
        for (std::int32_t i = 0; i < length; i++) {
            out.push_back(buffer_[static_cast<std::size_t>(offset_++)]);
        }
        return out;
    }

    void setByteArray(const char *data, std::int32_t length) {
        offset_ = 0;
        bitOffset_ = 0;
        buffer_.assign(data, data + length);
        length_ = length;
    }

    void reset() {
        offset_ = 0;
        bitOffset_ = 0;
    }

    void clear(std::int32_t capacity) {
        buffer_.clear();
        buffer_.reserve(static_cast<std::size_t>(capacity));
        length_ = 0;
        offset_ = 0;
        bitOffset_ = 0;
    }

    std::int32_t getLength() const {
        if (offset_ < length_) {
            return length_;
        }
        return offset_;
    }

    std::int32_t getOffset() const { return offset_; }

    const char *getByteArray() const {
        return reinterpret_cast<const char *>(buffer_.data());
    }

    bool isAtEnd() const { return offset_ >= length_; }

    static const char *CHARSET;

private:
    bool canRead(std::int32_t n) const {
        if (n < 0 || offset_ < 0) {
            return false;
        }
        std::int64_t end = static_cast<std::int64_t>(offset_) + n;
        std::int64_t have = static_cast<std::int64_t>(buffer_.size());
        if (length_ < have) {
            have = length_;
        }
        return end <= have;
    }
    std::int32_t readIntFromByteArray() {
        if (!canRead(4)) {
            return 0;
        }
        std::uint32_t b0 = buffer_[static_cast<std::size_t>(offset_++)];
        std::uint32_t b1 = buffer_[static_cast<std::size_t>(offset_++)];
        std::uint32_t b2 = buffer_[static_cast<std::size_t>(offset_++)];
        std::uint32_t b3 = buffer_[static_cast<std::size_t>(offset_++)];
        return static_cast<std::int32_t>((b0 << 24) | (b1 << 16) | (b2 << 8) | b3);
    }

    std::string readRawString(std::int32_t n, std::int32_t maxLength) {
        if (n < 0 || n > maxLength || !canRead(n)) {
            return std::string();
        }
        std::string s;
        s.resize(static_cast<std::size_t>(n));
        for (std::int32_t i = 0; i < n; i++) {
            s[static_cast<std::size_t>(i)] = static_cast<char>(buffer_[static_cast<std::size_t>(offset_++)]);
        }
        return s;
    }

    void writeIntToByteArray(std::int32_t v) {
        ensure(4);
        std::uint32_t u = static_cast<std::uint32_t>(v);
        buffer_.push_back(static_cast<std::uint8_t>((u >> 24) & 0xFF));
        buffer_.push_back(static_cast<std::uint8_t>((u >> 16) & 0xFF));
        buffer_.push_back(static_cast<std::uint8_t>((u >> 8) & 0xFF));
        buffer_.push_back(static_cast<std::uint8_t>(u & 0xFF));
        length_ = static_cast<std::int32_t>(buffer_.size());
    }

    void writeRaw(const char *data, std::int32_t n) {
        bitOffset_ = 0;
        writeIntToByteArray(n);
        ensure(n);
        for (std::int32_t i = 0; i < n; i++) {
            buffer_.push_back(static_cast<std::uint8_t>(data[i]));
        }
        length_ = static_cast<std::int32_t>(buffer_.size());
    }

    void writeVIntRaw(std::uint32_t u, bool negative) {
        if (!negative) {
            if (u < 64) {
                push(static_cast<std::uint8_t>(u & 0x3F));
                return;
            }
            if (u < 8192) {
                push(static_cast<std::uint8_t>((u & 0x3F) | 0x80));
                push(static_cast<std::uint8_t>((u >> 6) & 0x7F));
                return;
            }
            if (u < 1048576) {
                push(static_cast<std::uint8_t>((u & 0x3F) | 0x80));
                push(static_cast<std::uint8_t>(((u >> 6) & 0xFF) | 0x80));
                push(static_cast<std::uint8_t>((u >> 13) & 0x7F));
                return;
            }
            if (u <= 134217727) {
                push(static_cast<std::uint8_t>((u & 0x3F) | 0x80));
                push(static_cast<std::uint8_t>(((u >> 6) & 0xFF) | 0x80));
                push(static_cast<std::uint8_t>(((u >> 13) & 0xFF) | 0x80));
                push(static_cast<std::uint8_t>((u >> 20) & 0x7F));
                return;
            }
            push(static_cast<std::uint8_t>((u & 0x3F) | 0x80));
            push(static_cast<std::uint8_t>(((u >> 6) & 0xFF) | 0x80));
            push(static_cast<std::uint8_t>(((u >> 13) & 0xFF) | 0x80));
            push(static_cast<std::uint8_t>(((u >> 20) & 0xFF) | 0x80));
            push(static_cast<std::uint8_t>((u >> 27) & 0x0F));
            return;
        }
        std::int32_t v = static_cast<std::int32_t>(u);
        if (v >= -63) {
            push(static_cast<std::uint8_t>((u & 0x3F) | 0x40));
            return;
        }
        if (v >= -8191) {
            push(static_cast<std::uint8_t>((u & 0x3F) | 0xC0));
            push(static_cast<std::uint8_t>((u >> 6) & 0x7F));
            return;
        }
        if (v >= -1048575) {
            push(static_cast<std::uint8_t>((u & 0x3F) | 0xC0));
            push(static_cast<std::uint8_t>(((u >> 6) & 0xFF) | 0x80));
            push(static_cast<std::uint8_t>((u >> 13) & 0x7F));
            return;
        }
        if (v >= -134217727) {
            push(static_cast<std::uint8_t>((u & 0x3F) | 0xC0));
            push(static_cast<std::uint8_t>(((u >> 6) & 0xFF) | 0x80));
            push(static_cast<std::uint8_t>(((u >> 13) & 0xFF) | 0x80));
            push(static_cast<std::uint8_t>((u >> 20) & 0x7F));
            return;
        }
        push(static_cast<std::uint8_t>((u & 0x3F) | 0xC0));
        push(static_cast<std::uint8_t>(((u >> 6) & 0xFF) | 0x80));
        push(static_cast<std::uint8_t>(((u >> 13) & 0xFF) | 0x80));
        push(static_cast<std::uint8_t>(((u >> 20) & 0xFF) | 0x80));
        push(static_cast<std::uint8_t>((u >> 27) & 0x0F));
    }

    void ensure(std::int32_t extra) {
        if (length_ + extra > static_cast<std::int32_t>(buffer_.capacity())) {
            buffer_.reserve(buffer_.size() + static_cast<std::size_t>(extra) + 100);
        }
    }

    void push(std::uint8_t b) {
        buffer_.push_back(b);
        length_ = static_cast<std::int32_t>(buffer_.size());
    }

    std::vector<std::uint8_t> buffer_;
    std::int32_t length_ = 0;
    std::int32_t offset_ = 0;
    std::int32_t bitOffset_ = 0;
};

const char *ByteStream::CHARSET = "UTF-8";
