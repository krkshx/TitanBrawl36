#pragma once
#include <cstdint>
#include <string>
#include <vector>

class ScReader {
public:
    ScReader() = default;
    explicit ScReader(const std::vector<std::uint8_t> &d) : data(d) {
    }
    void set(const std::vector<std::uint8_t> &d) {
        data = d;
        pos = 0;
    }
    int readU8() {
        if (pos >= data.size()) {
            return 0;
        }
        return data[pos++];
    }
    int readU16() {
        int a = readU8();
        int b = readU8();
        return a | (b << 8);
    }
    int readI16() {
        int v = readU16();
        if (v >= 32768) {
            v -= 65536;
        }
        return v;
    }
    std::int32_t readI32() {
        std::uint32_t a = static_cast<std::uint32_t>(readU8());
        std::uint32_t b = static_cast<std::uint32_t>(readU8());
        std::uint32_t c = static_cast<std::uint32_t>(readU8());
        std::uint32_t d = static_cast<std::uint32_t>(readU8());
        return static_cast<std::int32_t>(a | (b << 8) | (c << 16) | (d << 24));
    }
    bool readBool() {
        return readU8() == 1;
    }
    float readTwip() {
        return static_cast<float>(readI32()) / 20.0f;
    }
    std::string readAscii() {
        int n = readU8();
        if (n == 255) {
            return std::string();
        }
        std::string s;
        s.resize(static_cast<std::size_t>(n));
        for (int i = 0; i < n && pos < data.size(); i++) {
            s[static_cast<std::size_t>(i)] = static_cast<char>(data[pos++]);
        }
        return s;
    }
    bool readAsciiNull(std::string &out) {
        int n = readU8();
        if (n == 255) {
            return false;
        }
        out.resize(static_cast<std::size_t>(n));
        for (int i = 0; i < n && pos < data.size(); i++) {
            out[static_cast<std::size_t>(i)] = static_cast<char>(data[pos++]);
        }
        return true;
    }
    std::vector<std::uint8_t> readBytes(int n) {
        std::vector<std::uint8_t> r;
        r.reserve(static_cast<std::size_t>(n));
        for (int i = 0; i < n && pos < data.size(); i++) {
            r.push_back(data[pos++]);
        }
        return r;
    }
    void skip(int n) {
        pos += static_cast<std::size_t>(n);
        if (pos > data.size()) {
            pos = data.size();
        }
    }
    std::size_t position() const {
        return pos;
    }
    std::size_t size() const {
        return data.size();
    }
private:
    std::vector<std::uint8_t> data;
    std::size_t pos = 0;
};
