#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H

class FontEngine {
public:
    bool load(const std::string &primary, const std::string &fallback) {
        if (FT_Init_FreeType(&lib_)) {
            return false;
        }
        hasPrimary_ = !FT_New_Face(lib_, primary.c_str(), 0, &primary_);
        hasFallback_ = !FT_New_Face(lib_, fallback.c_str(), 0, &fallback_);
        return hasPrimary_ || hasFallback_;
    }
    int measure(const std::string &text, int px) const {
        if ((!hasPrimary_ && !hasFallback_) || px < 1) {
            return 0;
        }
        FT_Face face = hasPrimary_ ? primary_ : fallback_;
        FT_Set_Pixel_Sizes(face, 0, static_cast<FT_UInt>(px));
        int w = 0;
        std::size_t i = 0;
        while (i < text.size()) {
            unsigned cp = 0;
            i = decode(text, i, cp);
            FT_Face f = faceFor(cp, px);
            FT_UInt idx = FT_Get_Char_Index(f, static_cast<FT_ULong>(cp));
            if (idx == 0) {
                idx = FT_Get_Char_Index(f, 63);
            }
            if (FT_Load_Glyph(f, idx, FT_LOAD_DEFAULT)) {
                continue;
            }
            w += static_cast<int>(f->glyph->advance.x >> 6);
        }
        return w;
    }
    void drawCentered(std::vector<std::uint32_t> &frame, int w, int h, const std::string &text, float bx, float by, float boxW, float boxH, int px, std::uint32_t color) {
        if ((!hasPrimary_ && !hasFallback_) || text.empty() || boxW < 4 || boxH < 4) {
            return;
        }
        if (px < 1) {
            px = 1;
        }
        while (px > 1 && measure(text, px) > static_cast<int>(boxW)) {
            px--;
        }
        FT_Face face = hasPrimary_ ? primary_ : fallback_;
        FT_Set_Pixel_Sizes(face, 0, static_cast<FT_UInt>(px));
        int asc = static_cast<int>(face->size->metrics.ascender >> 6);
        int desc = static_cast<int>(face->size->metrics.descender >> 6);
        int lineH = asc - desc;
        int baseline = static_cast<int>(by + (boxH - lineH) * 0.5f) + asc;
        int pen = static_cast<int>(bx + (boxW - measure(text, px)) * 0.5f);
        unsigned sr = (color >> 16) & 0xFF;
        unsigned sg = (color >> 8) & 0xFF;
        unsigned sb = color & 0xFF;
        unsigned sa = (color >> 24) & 0xFF;
        std::size_t i = 0;
        while (i < text.size()) {
            unsigned cp = 0;
            i = decode(text, i, cp);
            FT_Face f = faceFor(cp, px);
            FT_UInt idx = FT_Get_Char_Index(f, static_cast<FT_ULong>(cp));
            if (idx == 0) {
                idx = FT_Get_Char_Index(f, 63);
            }
            if (FT_Load_Glyph(f, idx, FT_LOAD_RENDER)) {
                continue;
            }
            FT_Bitmap *bm = &f->glyph->bitmap;
            int gx = pen + f->glyph->bitmap_left;
            int gy = baseline - f->glyph->bitmap_top;
            for (unsigned r = 0; r < bm->rows; r++) {
                for (unsigned c = 0; c < bm->width; c++) {
                    int dx = gx + static_cast<int>(c);
                    int dy = gy + static_cast<int>(r);
                    if (dx < 0 || dx >= w || dy < 0 || dy >= h) {
                        continue;
                    }
                    unsigned cov = bm->buffer[r * bm->pitch + c];
                    unsigned a = (cov * sa) / 255;
                    if (a == 0) {
                        continue;
                    }
                    std::size_t k = static_cast<std::size_t>(dy) * static_cast<std::size_t>(w) + static_cast<std::size_t>(dx);
                    std::uint32_t dst = frame[k];
                    unsigned dr = (dst >> 16) & 0xFF;
                    unsigned dg = (dst >> 8) & 0xFF;
                    unsigned db = dst & 0xFF;
                    unsigned rr = (sr * a + dr * (255 - a)) / 255;
                    unsigned gg = (sg * a + dg * (255 - a)) / 255;
                    unsigned bb = (sb * a + db * (255 - a)) / 255;
                    frame[k] = 0xFF000000u | (rr << 16) | (gg << 8) | bb;
                }
            }
            pen += static_cast<int>(f->glyph->advance.x >> 6);
        }
    }
private:
    static std::size_t decode(const std::string &s, std::size_t i, unsigned &cp) {
        unsigned char c0 = static_cast<unsigned char>(s[i]);
        if (c0 < 0x80) {
            cp = c0;
            return i + 1;
        }
        if ((c0 & 0xE0) == 0xC0 && i + 1 < s.size()) {
            unsigned char c1 = static_cast<unsigned char>(s[i + 1]);
            cp = (static_cast<unsigned>(c0 & 0x1F) << 6) | (c1 & 0x3F);
            return i + 2;
        }
        if ((c0 & 0xF0) == 0xE0 && i + 2 < s.size()) {
            unsigned char c1 = static_cast<unsigned char>(s[i + 1]);
            unsigned char c2 = static_cast<unsigned char>(s[i + 2]);
            cp = (static_cast<unsigned>(c0 & 0x0F) << 12) | (static_cast<unsigned>(c1 & 0x3F) << 6) | (c2 & 0x3F);
            return i + 3;
        }
        cp = 63;
        return i + 1;
    }
    FT_Face faceFor(unsigned cp, int px) const {
        if (hasPrimary_ && FT_Get_Char_Index(primary_, static_cast<FT_ULong>(cp))) {
            FT_Set_Pixel_Sizes(primary_, 0, static_cast<FT_UInt>(px));
            return primary_;
        }
        if (hasFallback_) {
            FT_Set_Pixel_Sizes(fallback_, 0, static_cast<FT_UInt>(px));
            return fallback_;
        }
        return primary_;
    }
    FT_Library lib_ = nullptr;
    FT_Face primary_ = nullptr;
    FT_Face fallback_ = nullptr;
    bool hasPrimary_ = false;
    bool hasFallback_ = false;
};
