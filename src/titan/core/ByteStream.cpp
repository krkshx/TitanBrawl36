#pragma once
#include "ChecksumEncoder.cpp"
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

class ByteStream : public ChecksumEncoder {
public:
    ByteStream() { buffer_.reserve(128); }
    explicit ByteStream(std::size_t capacity) { buffer_.reserve(capacity); }

    void writeByte(std::int8_t v) {
        ensure(1);
        bitOffset_ = 0;
        buffer_.push_back(static_cast<std::uint8_t>(v));
        length_ = static_cast<std::int32_t>(buffer_.size());
    }

    bool writeBoolean(bool v) override {
        ChecksumEncoder::writeBoolean(v);
        ensure(1);
        if (bitOffset_ == 0) {
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
        writeIntRaw(v);
        bitOffset_ = 0;
    }

    void writeVInt(std::int32_t v) override {
        ChecksumEncoder::writeVInt(v);
        ensure(5);
        bitOffset_ = 0;
        std::uint32_t u = static_cast<std::uint32_t>(v);
        if (v < 0) {
            if (v >= -63) {
                push(static_cast<std::uint8_t>((u & 0x3F) | 0x40));
                return;
            }
            if (v >= -8191) {
                push(static_cast<std::uint8_t>(u | 0xC0));
                push(static_cast<std::uint8_t>(((u >> 6) & 0x7F)));
                return;
            }
            if (v >= -1048575) {
                push(static_cast<std::uint8_t>(u | 0xC0));
                push(static_cast<std::uint8_t>(((u >> 6) & 0xFF) | 0x80));
                push(static_cast<std::uint8_t>(((u >> 13) & 0x7F)));
                return;
            }
            if (v >= -134217727) {
                push(static_cast<std::uint8_t>(u | 0xC0));
                push(static_cast<std::uint8_t>(((u >> 6) & 0xFF) | 0x80));
                push(static_cast<std::uint8_t>(((u >> 13) & 0xFF) | 0x80));
                push(static_cast<std::uint8_t>(((u >> 20) & 0x7F)));
                return;
            }
            push(static_cast<std::uint8_t>(u | 0xC0));
            push(static_cast<std::uint8_t>(((u >> 6) & 0xFF) | 0x80));
            push(static_cast<std::uint8_t>(((u >> 13) & 0xFF) | 0x80));
            push(static_cast<std::uint8_t>(((u >> 20) & 0xFF) | 0x80));
            push(static_cast<std::uint8_t>(((u >> 27) & 0x0F)));
            return;
        }
        if (v > 63) {
            if (v < 8192) {
                push(static_cast<std::uint8_t>(((u & 0x3F) | 0x80)));
                push(static_cast<std::uint8_t>(((u >> 6) & 0x7F)));
                return;
            }
            if (v < 1048576) {
                push(static_cast<std::uint8_t>(((u & 0x3F) | 0x80)));
                push(static_cast<std::uint8_t>((((u >> 6) & 0xFF)) | 0x80));
                push(static_cast<std::uint8_t>(((u >> 13) & 0x7F)));
                return;
            }
            if (v <= 134217727) {
                push(static_cast<std::uint8_t>(((u & 0x3F) | 0x80)));
                push(static_cast<std::uint8_t>((((u >> 6) & 0xFF)) | 0x80));
                push(static_cast<std::uint8_t>((((u >> 13) & 0xFF)) | 0x80));
                push(static_cast<std::uint8_t>(((u >> 20) & 0x7F)));
                return;
            }
            push(static_cast<std::uint8_t>(((u & 0x3F) | 0x80)));
            push(static_cast<std::uint8_t>((((u >> 6) & 0xFF)) | 0x80));
            push(static_cast<std::uint8_t>((((u >> 13) & 0xFF)) | 0x80));
            push(static_cast<std::uint8_t>((((u >> 20) & 0xFF)) | 0x80));
            push(static_cast<std::uint8_t>(((u >> 27) & 0x0F)));
            return;
        }
        push(static_cast<std::uint8_t>(u & 0x3F));
    }

    void writeString(const std::string *v) {
        bitOffset_ = 0;
        if (v == nullptr) {
            writeIntRaw(-1);
            return;
        }
        if (v->size() >= 900000) {
            writeIntRaw(-1);
            return;
        }
        writeIntRaw(static_cast<std::int32_t>(v->size()));
        ensure(static_cast<std::int32_t>(v->size()));
        for (char c : *v) {
            buffer_.push_back(static_cast<std::uint8_t>(c));
        }
        length_ = static_cast<std::int32_t>(buffer_.size());
    }

    void writeBytes(const std::uint8_t *d, std::int32_t n) {
        bitOffset_ = 0;
        if (d == nullptr) {
            writeIntRaw(-1);
            return;
        }
        writeIntRaw(n);
        ensure(n);
        for (std::int32_t i = 0; i < n; i++) {
            buffer_.push_back(d[i]);
        }
        length_ = static_cast<std::int32_t>(buffer_.size());
    }

    bool readBoolean() {
        std::int32_t v1 = bitOffset_;
        std::int32_t base = readCursor_ + ((8 - v1) >> 3);
        readCursor_ = base;
        std::uint8_t b = buffer_[static_cast<std::size_t>(base - 1)];
        bitOffset_ = (v1 + 1) & 7;
        return (((1 << v1) & b) != 0);
    }

    std::int32_t readInt() {
        bitOffset_ = 0;
        std::uint32_t b0 = buffer_[static_cast<std::size_t>(readCursor_++)];
        std::uint32_t b1 = buffer_[static_cast<std::size_t>(readCursor_++)];
        std::uint32_t b2 = buffer_[static_cast<std::size_t>(readCursor_++)];
        std::uint32_t b3 = buffer_[static_cast<std::size_t>(readCursor_++)];
        std::uint32_t u = (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
        return static_cast<std::int32_t>(u);
    }

    std::int32_t readVInt() {
        bitOffset_ = 0;
        std::uint8_t b0 = buffer_[static_cast<std::size_t>(readCursor_++)];
        std::int32_t r = b0 & 0x3F;
        if ((b0 & 0x40) != 0) {
            if ((b0 & 0x80) != 0) {
                std::uint8_t b1 = buffer_[static_cast<std::size_t>(readCursor_++)];
                r = (r & 0xFFFFE03F) | ((b1 & 0x7F) << 6);
                if ((b1 & 0x80) != 0) {
                    std::uint8_t b2 = buffer_[static_cast<std::size_t>(readCursor_++)];
                    std::uint32_t t = static_cast<std::uint32_t>(r) & 0xFFF01FFF | ((b2 & 0x7F) << 13);
                    if ((b2 & 0x80) != 0) {
                        std::uint8_t b3 = buffer_[static_cast<std::size_t>(readCursor_++)];
                        std::uint32_t t2 = t & 0xF80FFFFF | ((b3 & 0x7F) << 20);
                        if ((b3 & 0x80) != 0) {
                            std::uint8_t b4 = buffer_[static_cast<std::size_t>(readCursor_++)];
                            std::uint32_t t3 = (t2 & 0x7FFFFFF) | (static_cast<std::uint32_t>(b4) << 27) | 0x80000000u;
                            return static_cast<std::int32_t>(t3);
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
            std::uint8_t b1 = buffer_[static_cast<std::size_t>(readCursor_++)];
            r = (r & 0xFFFFE03F) | ((b1 & 0x7F) << 6);
            if ((b1 & 0x80) != 0) {
                std::uint8_t b2 = buffer_[static_cast<std::size_t>(readCursor_++)];
                r = (r & 0xFFF01FFF) | ((b2 & 0x7F) << 13);
                if ((b2 & 0x80) != 0) {
                    std::uint8_t b3 = buffer_[static_cast<std::size_t>(readCursor_++)];
                    r = (r & 0xF80FFFFF) | ((b3 & 0x7F) << 20);
                    if ((b3 & 0x80) != 0) {
                        std::uint8_t b4 = buffer_[static_cast<std::size_t>(readCursor_++)];
                        r = (r & 0x7FFFFFF) | (b4 << 27);
                    }
                }
            }
        }
        return r;
    }

    std::string readString() {
        bitOffset_ = 0;
        std::int32_t n = readInt();
        if (n < 0) {
            return std::string();
        }
        std::string s;
        s.resize(static_cast<std::size_t>(n));
        for (std::int32_t i = 0; i < n; i++) {
            s[static_cast<std::size_t>(i)] = static_cast<char>(buffer_[static_cast<std::size_t>(readCursor_++)]);
        }
        return s;
    }

    const std::uint8_t *data() const { return buffer_.data(); }
    std::int32_t size() const { return length_; }
    std::int32_t remaining() const { return length_ - readCursor_; }
    bool atEnd() const { return readCursor_ >= length_; }
    void reset() { readCursor_ = 0; bitOffset_ = 0; }
    void clear() { buffer_.clear(); length_ = 0; readCursor_ = 0; bitOffset_ = 0; }
    void setBuffer(const std::uint8_t *d, std::int32_t n) {
        buffer_.assign(d, d + n);
        length_ = n;
        readCursor_ = 0;
        bitOffset_ = 0;
    }

private:
    void ensure(std::int32_t extra) {
        if (length_ + extra > static_cast<std::int32_t>(buffer_.capacity())) {
            buffer_.reserve(buffer_.size() + static_cast<std::size_t>(extra) + 105);
        }
    }
    void push(std::uint8_t b) {
        buffer_.push_back(b);
        length_ = static_cast<std::int32_t>(buffer_.size());
    }
    void writeIntRaw(std::int32_t v) {
        ensure(4);
        std::uint32_t u = static_cast<std::uint32_t>(v);
        buffer_.push_back(static_cast<std::uint8_t>((u >> 24) & 0xFF));
        buffer_.push_back(static_cast<std::uint8_t>((u >> 16) & 0xFF));
        buffer_.push_back(static_cast<std::uint8_t>((u >> 8) & 0xFF));
        buffer_.push_back(static_cast<std::uint8_t>(u & 0xFF));
        length_ = static_cast<std::int32_t>(buffer_.size());
    }
    std::vector<std::uint8_t> buffer_;
    std::int32_t length_ = 0;
    std::int32_t bitOffset_ = 0;
    std::int32_t readCursor_ = 0;
};
