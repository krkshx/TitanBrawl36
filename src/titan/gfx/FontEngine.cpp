#pragma once
#include "NativeFont.cpp"
#include <cstdint>
#include <string>
#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H

class FontEngine {
public:
    bool load(const std::vector<std::string> &paths) {
        if (!lib_) {
            if (FT_Init_FreeType(&lib_)) {
                return false;
            }
        }
        for (std::size_t i = 0; i < paths.size(); i++) {
            FT_Face face = nullptr;
            if (!FT_New_Face(lib_, paths[i].c_str(), 0, &face)) {
                faces_.push_back(face);
            }
        }
        return !faces_.empty();
    }
    bool load(const std::string &primary, const std::string &fallback) {
        std::vector<std::string> paths;
        paths.push_back(primary);
        paths.push_back(fallback);
        return load(paths);
    }
    bool loadBundled(const std::string &assetsDir) {
        std::vector<std::string> paths;
        paths.push_back(assetsDir + "/font/LilitaOne-Regular.ttf");
        paths.push_back(assetsDir + "/font/Pusia-Bold.otf");
        paths.push_back(assetsDir + "/titan/fonts/droid_sans_fallback.ttf");
        std::vector<std::string> system = NativeFont::systemFallbacks();
        for (std::size_t i = 0; i < system.size(); i++) {
            paths.push_back(system[i]);
        }
        return load(paths);
    }
    bool loadSystem(const std::string &assetsDir, const std::string &family, bool bold) {
        std::vector<std::string> paths = NativeFont::candidatesFor(family, bold);
        paths.push_back(assetsDir + "/font/Pusia-Bold.otf");
        paths.push_back(assetsDir + "/font/LilitaOne-Regular.ttf");
        paths.push_back(assetsDir + "/titan/fonts/droid_sans_fallback.ttf");
        std::vector<std::string> system = NativeFont::systemFallbacks();
        for (std::size_t i = 0; i < system.size(); i++) {
            paths.push_back(system[i]);
        }
        return load(paths);
    }
    int measure(const std::string &text, int px) const {
        if (faces_.empty() || px < 1) {
            return 0;
        }
        FT_Face face = faces_[0];
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
        drawCentered(frame, w, h, text, bx, by, boxW, boxH, px, color, false, 0xFF000000u);
    }
    void drawCentered(std::vector<std::uint32_t> &frame, int w, int h, const std::string &text, float bx, float by, float boxW, float boxH, int px, std::uint32_t color, bool outline, std::uint32_t outlineColor) {
        if (faces_.empty() || text.empty() || boxW < 4 || boxH < 4) {
            return;
        }
        if (px < 1) {
            px = 1;
        }
        while (px > 1 && measure(text, px) > static_cast<int>(boxW)) {
            px--;
        }
        FT_Face face = faces_[0];
        FT_Set_Pixel_Sizes(face, 0, static_cast<FT_UInt>(px));
        int asc = static_cast<int>(face->size->metrics.ascender >> 6);
        int desc = static_cast<int>(face->size->metrics.descender >> 6);
        int lineH = asc - desc;
        int baseline = static_cast<int>(by + (boxH - lineH) * 0.5f) + asc;
        int pen = static_cast<int>(bx + (boxW - measure(text, px)) * 0.5f);
        if (outline) {
            int o = px / 12;
            if (o < 1) {
                o = 1;
            }
            if (o > 4) {
                o = 4;
            }
            drawPass(frame, w, h, text, pen, baseline, px, outlineColor, -o, 0);
            drawPass(frame, w, h, text, pen, baseline, px, outlineColor, o, 0);
            drawPass(frame, w, h, text, pen, baseline, px, outlineColor, 0, -o);
            drawPass(frame, w, h, text, pen, baseline, px, outlineColor, 0, o);
            drawPass(frame, w, h, text, pen, baseline, px, outlineColor, -o, -o);
            drawPass(frame, w, h, text, pen, baseline, px, outlineColor, o, -o);
            drawPass(frame, w, h, text, pen, baseline, px, outlineColor, -o, o);
            drawPass(frame, w, h, text, pen, baseline, px, outlineColor, o, o);
        }
        drawPass(frame, w, h, text, pen, baseline, px, color, 0, 0);
    }
private:
    void drawPass(std::vector<std::uint32_t> &frame, int w, int h, const std::string &text, int pen, int baseline, int px, std::uint32_t color, int offX, int offY) const {
        unsigned sr = (color >> 16) & 0xFF;
        unsigned sg = (color >> 8) & 0xFF;
        unsigned sb = color & 0xFF;
        unsigned sa = (color >> 24) & 0xFF;
        if (sa == 0) {
            return;
        }
        std::size_t i = 0;
        int cursor = pen;
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
            int gx = cursor + f->glyph->bitmap_left + offX;
            int gy = baseline - f->glyph->bitmap_top + offY;
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
            cursor += static_cast<int>(f->glyph->advance.x >> 6);
        }
    }
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
        for (std::size_t i = 0; i < faces_.size(); i++) {
            FT_Face f = faces_[i];
            if (FT_Get_Char_Index(f, static_cast<FT_ULong>(cp))) {
                FT_Set_Pixel_Sizes(f, 0, static_cast<FT_UInt>(px));
                return f;
            }
        }
        return faces_[0];
    }
    FT_Library lib_ = nullptr;
    std::vector<FT_Face> faces_;
};
