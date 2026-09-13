#pragma once
#include "ScReader.cpp"
#include <cstdint>
#include <vector>

class SWFTexture {
public:
    int tag = 0;
    int type = 0;
    int width = 0;
    int height = 0;
    std::vector<std::uint32_t> pixels;
    bool hasTexture = false;
    void load(ScReader &s, int t, bool withPixels) {
        tag = t;
        type = s.readU8();
        width = s.readU16();
        height = s.readU16();
        hasTexture = withPixels;
        if (!withPixels) {
            return;
        }
        int n = width * height * pixelBytes(type);
        std::vector<std::uint8_t> raw = s.readBytes(n);
        decode(raw);
        if (t == 27 || t == 28 || t == 29) {
            detile();
        }
    }
    static int pixelBytes(int t) {
        if (t == 10) {
            return 1;
        }
        if (t == 2 || t == 3 || t == 4 || t == 8) {
            return 2;
        }
        if (t == 6) {
            return 2;
        }
        return 4;
    }
    void decode(const std::vector<std::uint8_t> &raw) {
        pixels.resize(static_cast<std::size_t>(width) * static_cast<std::size_t>(height));
        if (type == 10) {
            for (std::size_t i = 0; i < pixels.size() && i < raw.size(); i++) {
                std::uint32_t l = raw[i];
                pixels[i] = 0xFF000000u | (l << 16) | (l << 8) | l;
            }
            return;
        }
        if (type == 6) {
            for (std::size_t i = 0; i < pixels.size() && 2 * i + 1 < raw.size(); i++) {
                std::uint32_t l = raw[2 * i];
                std::uint32_t a = raw[2 * i + 1];
                pixels[i] = (a << 24) | (l << 16) | (l << 8) | l;
            }
            return;
        }
        if (type == 2 || type == 8) {
            for (std::size_t i = 0; i < pixels.size() && 2 * i + 1 < raw.size(); i++) {
                std::uint32_t v = raw[2 * i] | (static_cast<std::uint32_t>(raw[2 * i + 1]) << 8);
                std::uint32_t r = ((v >> 12) & 0xF) * 17;
                std::uint32_t g = ((v >> 8) & 0xF) * 17;
                std::uint32_t b = ((v >> 4) & 0xF) * 17;
                std::uint32_t a = (v & 0xF) * 17;
                pixels[i] = (a << 24) | (r << 16) | (g << 8) | b;
            }
            return;
        }
        if (type == 3) {
            for (std::size_t i = 0; i < pixels.size() && 2 * i + 1 < raw.size(); i++) {
                std::uint32_t v = raw[2 * i] | (static_cast<std::uint32_t>(raw[2 * i + 1]) << 8);
                std::uint32_t r = ((v >> 11) & 0x1F) * 8;
                std::uint32_t g = ((v >> 6) & 0x1F) * 8;
                std::uint32_t b = ((v >> 1) & 0x1F) * 8;
                std::uint32_t a = (v & 1) ? 255 : 0;
                if (r > 255) {
                    r = 255;
                }
                if (g > 255) {
                    g = 255;
                }
                if (b > 255) {
                    b = 255;
                }
                pixels[i] = (a << 24) | (r << 16) | (g << 8) | b;
            }
            return;
        }
        if (type == 4) {
            for (std::size_t i = 0; i < pixels.size() && 2 * i + 1 < raw.size(); i++) {
                std::uint32_t v = raw[2 * i] | (static_cast<std::uint32_t>(raw[2 * i + 1]) << 8);
                std::uint32_t r = ((v >> 11) & 0x1F) * 8;
                std::uint32_t g = ((v >> 5) & 0x3F) * 4;
                std::uint32_t b = (v & 0x1F) * 8;
                if (r > 255) {
                    r = 255;
                }
                if (g > 255) {
                    g = 255;
                }
                if (b > 255) {
                    b = 255;
                }
                pixels[i] = 0xFF000000u | (r << 16) | (g << 8) | b;
            }
            return;
        }
        for (std::size_t i = 0; i < pixels.size() && 4 * i + 3 < raw.size(); i++) {
            std::uint32_t r = raw[4 * i];
            std::uint32_t g = raw[4 * i + 1];
            std::uint32_t b = raw[4 * i + 2];
            std::uint32_t a = raw[4 * i + 3];
            pixels[i] = (a << 24) | (r << 16) | (g << 8) | b;
        }
    }
    void detile() {
        int tw = width < 32 ? width : 32;
        (void)tw;
        std::vector<std::uint32_t> src = pixels;
        std::size_t offset = 0;
        int tilesX = (width + 31) / 32;
        int tilesY = (height + 31) / 32;
        for (int ty = 0; ty < tilesY; ty++) {
            for (int tx = 0; tx < tilesX; tx++) {
                int w = width - tx * 32;
                if (w > 32) {
                    w = 32;
                }
                int h = height - ty * 32;
                if (h > 32) {
                    h = 32;
                }
                for (int y = 0; y < h; y++) {
                    for (int x = 0; x < w; x++) {
                        std::size_t di = static_cast<std::size_t>(ty * 32 + y) * static_cast<std::size_t>(width) + static_cast<std::size_t>(tx * 32 + x);
                        if (offset < src.size() && di < pixels.size()) {
                            pixels[di] = src[offset];
                        }
                        offset++;
                    }
                }
            }
        }
    }
    std::uint32_t sampleBilinear(float u, float v) const {
        if (pixels.empty() || width <= 0 || height <= 0) {
            return 0;
        }
        float fx = u * width - 0.5f;
        float fy = v * height - 0.5f;
        int x0 = static_cast<int>(fx);
        int y0 = static_cast<int>(fy);
        float tx = fx - x0;
        float ty = fy - y0;
        if (tx < 0) {
            tx = 0;
        }
        if (ty < 0) {
            ty = 0;
        }
        std::uint32_t c00 = at(x0, y0);
        std::uint32_t c10 = at(x0 + 1, y0);
        std::uint32_t c01 = at(x0, y0 + 1);
        std::uint32_t c11 = at(x0 + 1, y0 + 1);
        return mix(mix(c00, c10, tx), mix(c01, c11, tx), ty);
    }
    static std::uint32_t mix(std::uint32_t a, std::uint32_t b, float t) {
        float ar = static_cast<float>((a >> 16) & 0xFF);
        float ag = static_cast<float>((a >> 8) & 0xFF);
        float ab = static_cast<float>(a & 0xFF);
        float aa = static_cast<float>((a >> 24) & 0xFF);
        float br = static_cast<float>((b >> 16) & 0xFF);
        float bg = static_cast<float>((b >> 8) & 0xFF);
        float bb = static_cast<float>(b & 0xFF);
        float ba = static_cast<float>((b >> 24) & 0xFF);
        unsigned r = static_cast<unsigned>(ar + (br - ar) * t);
        unsigned g = static_cast<unsigned>(ag + (bg - ag) * t);
        unsigned bl = static_cast<unsigned>(ab + (bb - ab) * t);
        unsigned al = static_cast<unsigned>(aa + (ba - aa) * t);
        return (al << 24) | (r << 16) | (g << 8) | bl;
    }
    std::uint32_t sample(float u, float v) const {
        if (pixels.empty() || width <= 0 || height <= 0) {
            return 0;
        }
        int x = static_cast<int>(u * width);
        int y = static_cast<int>(v * height);
        if (x < 0) {
            x = 0;
        }
        if (y < 0) {
            y = 0;
        }
        if (x >= width) {
            x = width - 1;
        }
        if (y >= height) {
            y = height - 1;
        }
        return pixels[static_cast<std::size_t>(y) * static_cast<std::size_t>(width) + static_cast<std::size_t>(x)];
    }
private:
    std::uint32_t at(int x, int y) const {
        if (x < 0) {
            x = 0;
        }
        if (y < 0) {
            y = 0;
        }
        if (x >= width) {
            x = width - 1;
        }
        if (y >= height) {
            y = height - 1;
        }
        return pixels[static_cast<std::size_t>(y) * static_cast<std::size_t>(width) + static_cast<std::size_t>(x)];
    }
};
