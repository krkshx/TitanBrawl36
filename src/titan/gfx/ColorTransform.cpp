#pragma once
#include "ScReader.cpp"
#include <cstdint>

class ColorTransform {
public:
    std::uint8_t redAddition = 0;
    std::uint8_t greenAddition = 0;
    std::uint8_t blueAddition = 0;
    std::uint8_t alpha = 255;
    std::uint8_t redMultiplier = 255;
    std::uint8_t greenMultiplier = 255;
    std::uint8_t blueMultiplier = 255;
    void read(ScReader &s) {
        redAddition = static_cast<std::uint8_t>(s.readU8());
        greenAddition = static_cast<std::uint8_t>(s.readU8());
        blueAddition = static_cast<std::uint8_t>(s.readU8());
        alpha = static_cast<std::uint8_t>(s.readU8());
        redMultiplier = static_cast<std::uint8_t>(s.readU8());
        greenMultiplier = static_cast<std::uint8_t>(s.readU8());
        blueMultiplier = static_cast<std::uint8_t>(s.readU8());
    }
    std::uint32_t apply(std::uint32_t c) const {
        unsigned r = (c >> 16) & 0xFF;
        unsigned g = (c >> 8) & 0xFF;
        unsigned b = c & 0xFF;
        unsigned a = (c >> 24) & 0xFF;
        r = (r * redMultiplier) / 255 + redAddition;
        g = (g * greenMultiplier) / 255 + greenAddition;
        b = (b * blueMultiplier) / 255 + blueAddition;
        a = (a * alpha) / 255;
        if (r > 255) {
            r = 255;
        }
        if (g > 255) {
            g = 255;
        }
        if (b > 255) {
            b = 255;
        }
        if (a > 255) {
            a = 255;
        }
        return (a << 24) | (r << 16) | (g << 8) | b;
    }
};
