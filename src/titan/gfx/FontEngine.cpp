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
        // Первым — Pusia-Bold: именно им рисуется игровой текст (статус загрузки и т.д.).
        std::vector<std::string> paths;
        paths.push_back(assetsDir + "/font/Pusia-Bold.otf");
        paths.push_back(assetsDir + "/font/LilitaOne-Regular.ttf");
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
        if (!outline) {
            drawPass(frame, w, h, text, pen, baseline, px, color, 0, 0);
            return;
        }
        int o = px / 12;
        if (o < 1) {
            o = 1;
        }
        if (o > 4) {
            o = 4;
        }
        drawStroked(frame, w, h, text, pen, baseline, px, color, outlineColor, o);
    }
    void drawStroked(std::vector<std::uint32_t> &frame, int w, int h, const std::string &text, int pen, int baseline, int px, std::uint32_t color, std::uint32_t outlineColor, int r) const {
        struct Glyph {
            std::vector<unsigned char> cov;
            int gw = 0;
            int gh = 0;
            int gx = 0;
            int gy = 0;
        };
        std::vector<Glyph> glyphs;
        std::size_t i = 0;
        int cursor = pen;
        int x0 = pen;
        int y0 = baseline;
        int x1 = pen;
        int y1 = baseline;
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
            Glyph g;
            g.gw = static_cast<int>(bm->width);
            g.gh = static_cast<int>(bm->rows);
            g.gx = cursor + f->glyph->bitmap_left;
            g.gy = baseline - f->glyph->bitmap_top;
            g.cov.assign(static_cast<std::size_t>(g.gw) * static_cast<std::size_t>(g.gh), 0);
            for (int rr2 = 0; rr2 < g.gh; rr2++) {
                for (int cc2 = 0; cc2 < g.gw; cc2++) {
                    g.cov[static_cast<std::size_t>(rr2) * static_cast<std::size_t>(g.gw) + static_cast<std::size_t>(cc2)] = bm->buffer[rr2 * bm->pitch + cc2];
                }
            }
            if (g.gw > 0 && g.gh > 0) {
                if (g.gx < x0) x0 = g.gx;
                if (g.gy < y0) y0 = g.gy;
                if (g.gx + g.gw > x1) x1 = g.gx + g.gw;
                if (g.gy + g.gh > y1) y1 = g.gy + g.gh;
                glyphs.push_back(g);
            }
            cursor += static_cast<int>(f->glyph->advance.x >> 6);
        }
        if (glyphs.empty()) {
            return;
        }
        int pad = r + 3;
        x0 -= pad;
        y0 -= pad;
        x1 += pad;
        y1 += pad;
        int mw = x1 - x0;
        int mh = y1 - y0;
        if (mw < 1 || mh < 1) {
            return;
        }
        std::vector<unsigned char> mask(static_cast<std::size_t>(mw) * static_cast<std::size_t>(mh), 0);
        for (std::size_t gi = 0; gi < glyphs.size(); gi++) {
            const Glyph &g = glyphs[gi];
            for (int yy = 0; yy < g.gh; yy++) {
                for (int xx = 0; xx < g.gw; xx++) {
                    unsigned char v = g.cov[static_cast<std::size_t>(yy) * static_cast<std::size_t>(g.gw) + static_cast<std::size_t>(xx)];
                    if (v == 0) {
                        continue;
                    }
                    std::size_t k = static_cast<std::size_t>(g.gy + yy - y0) * static_cast<std::size_t>(mw) + static_cast<std::size_t>(g.gx + xx - x0);
                    if (v > mask[k]) {
                        mask[k] = v;
                    }
                }
            }
        }
        int span = 2 * r + 1;
        std::vector<int> disk;
        for (int dy = -r; dy <= r; dy++) {
            for (int dx = -r; dx <= r; dx++) {
                if (dx * dx + dy * dy <= r * r) {
                    disk.push_back((dy + r) * span + (dx + r));
                }
            }
        }
        std::vector<unsigned char> wide(static_cast<std::size_t>(mw) * static_cast<std::size_t>(mh), 0);
        for (int yy = 0; yy < mh; yy++) {
            for (int xx = 0; xx < mw; xx++) {
                std::size_t k = static_cast<std::size_t>(yy) * static_cast<std::size_t>(mw) + static_cast<std::size_t>(xx);
                unsigned char best = 0;
                for (std::size_t di = 0; di < disk.size(); di++) {
                    int nx = xx + (disk[di] % span) - r;
                    int ny = yy + (disk[di] / span) - r;
                    if (nx < 0 || nx >= mw || ny < 0 || ny >= mh) {
                        continue;
                    }
                    unsigned char v = mask[static_cast<std::size_t>(ny) * static_cast<std::size_t>(mw) + static_cast<std::size_t>(nx)];
                    if (v > best) {
                        best = v;
                    }
                }
                wide[k] = best;
            }
        }
        // Мягкая тень — от заливки глифов (mask), со сдвигом вправо-вниз:
        // от расширенного контура (wide) она расползалась серым ореолом.
        std::vector<unsigned char> soft(static_cast<std::size_t>(mw) * static_cast<std::size_t>(mh), 0);
        for (int yy = 0; yy < mh; yy++) {
            for (int xx = 0; xx < mw; xx++) {
                int sx = xx - 1;
                int sy = yy - (r + 1);
                if (sx < 0 || sx >= mw || sy < 0 || sy >= mh) {
                    continue;
                }
                unsigned acc = 0;
                for (int ky = -1; ky <= 1; ky++) {
                    for (int kx = -1; kx <= 1; kx++) {
                        int nx = sx + kx;
                        int ny = sy + ky;
                        if (nx < 0 || nx >= mw || ny < 0 || ny >= mh) {
                            continue;
                        }
                        acc += mask[static_cast<std::size_t>(ny) * static_cast<std::size_t>(mw) + static_cast<std::size_t>(nx)];
                    }
                }
                soft[static_cast<std::size_t>(yy) * static_cast<std::size_t>(mw) + static_cast<std::size_t>(xx)] = static_cast<unsigned char>(acc / 9);
            }
        }
        unsigned orr = (outlineColor >> 16) & 0xFF;
        unsigned ogg = (outlineColor >> 8) & 0xFF;
        unsigned obb = outlineColor & 0xFF;
        unsigned oa = (outlineColor >> 24) & 0xFF;
        unsigned crr = (color >> 16) & 0xFF;
        unsigned cgg = (color >> 8) & 0xFF;
        unsigned cbb = color & 0xFF;
        unsigned caa = (color >> 24) & 0xFF;
        for (int yy = 0; yy < mh; yy++) {
            for (int xx = 0; xx < mw; xx++) {
                int dx = x0 + xx;
                int dy = y0 + yy;
                if (dx < 0 || dx >= w || dy < 0 || dy >= h) {
                    continue;
                }
                std::size_t k = static_cast<std::size_t>(yy) * static_cast<std::size_t>(mw) + static_cast<std::size_t>(xx);
                std::size_t kk = static_cast<std::size_t>(dy) * static_cast<std::size_t>(w) + static_cast<std::size_t>(dx);
                unsigned sh = soft[k];
                if (sh > 0 && oa > 0) {
                    unsigned a = (sh * oa * 140) / 65025;
                    if (a > 0) {
                        frame[kk] = blendOver(frame[kk], orr, ogg, obb, a);
                    }
                }
                unsigned char ov = wide[k];
                if (ov > 0 && oa > 0) {
                    unsigned a = (static_cast<unsigned>(ov) * oa) / 255;
                    if (a > 0) {
                        frame[kk] = blendOver(frame[kk], orr, ogg, obb, a);
                    }
                }
                unsigned char mv = mask[k];
                if (mv > 0 && caa > 0) {
                    unsigned a = (static_cast<unsigned>(mv) * caa) / 255;
                    if (a > 0) {
                        frame[kk] = blendOver(frame[kk], crr, cgg, cbb, a);
                    }
                }
            }
        }
    }
    static std::uint32_t blendOver(std::uint32_t dst, unsigned sr, unsigned sg, unsigned sb, unsigned a) {
        if (a >= 255) {
            return 0xFF000000u | (sr << 16) | (sg << 8) | sb;
        }
        unsigned dr = (dst >> 16) & 0xFF;
        unsigned dg = (dst >> 8) & 0xFF;
        unsigned db = dst & 0xFF;
        unsigned rr = (sr * a + dr * (255 - a)) / 255;
        unsigned gg = (sg * a + dg * (255 - a)) / 255;
        unsigned bb = (sb * a + db * (255 - a)) / 255;
        return 0xFF000000u | (rr << 16) | (gg << 8) | bb;
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
