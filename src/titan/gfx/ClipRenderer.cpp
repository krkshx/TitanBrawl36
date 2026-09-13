#pragma once
#ifdef TITAN_HAS_FREETYPE
#include "FontEngine.cpp"
#else
#include "BitmapFont.cpp"
#endif
#include "SupercellSWF.cpp"
#include <cstdint>
#include <string>
#include <vector>

class ClipRenderer {
public:
    void bind(SupercellSWF *swf) {
        swf_ = swf;
    }
    void setBarFrame(int frame) {
        barFrame_ = frame;
    }
    void setStatusText(const std::string &text) {
        statusText_ = text;
    }
    void setFonts(const std::string &primary, const std::string &fallback) {
#ifdef TITAN_HAS_FREETYPE
        fonts_.load(primary, fallback);
#else
        (void)primary;
        (void)fallback;
#endif
    }
    void render(std::vector<std::uint32_t> &frame, int w, int h, int rootId) {
        if (!swf_ || frame.empty()) {
            return;
        }
        for (auto &p : frame) {
            p = 0xFF000000u;
        }
        float s = w / 1288.0f;
        float s2 = h / 768.0f;
        if (s2 > s) {
            s = s2;
        }
        scale_ = s;
        ox_ = w * 0.5f;
        oy_ = h * 0.5f;
        const MovieClipOriginal *root = findClip(rootId);
        if (!root) {
            return;
        }
        Matrix2x3 base;
        base.setIdentity();
        drawClip(*root, 0, base, frame, w, h);
    }
private:
    static bool skipClip(const std::string &name) {
        return name == "supercell_id" || name == "buttons_tencent" || name == "buttons_yoozoo" || name == "icon_prc_age" || name == "logo_KR" || name == "logo_JP" || name == "logo_CNT" || name == "logo_CNS";
    }
    const MovieClipOriginal *findClip(int id) const {
        for (auto &c : swf_->clips) {
            if (c.id == id) {
                return &c;
            }
        }
        return nullptr;
    }
    const ShapeOriginal *findShape(int id) const {
        for (auto &s : swf_->shapes) {
            if (s.id == id) {
                return &s;
            }
        }
        return nullptr;
    }
    const TextFieldOriginal *findField(int id) const {
        for (auto &f : swf_->fields) {
            if (f.id == id) {
                return &f;
            }
        }
        return nullptr;
    }
    void drawClip(const MovieClipOriginal &clip, int frameIndex, const Matrix2x3 &parent, std::vector<std::uint32_t> &frame, int w, int h) {
        if (clip.frames.empty()) {
            return;
        }
        int fi = frameIndex;
        if (fi < 0) {
            fi = 0;
        }
        if (fi >= static_cast<int>(clip.frames.size())) {
            fi = static_cast<int>(clip.frames.size()) - 1;
        }
        const MovieClipOriginal::Frame &fr = clip.frames[static_cast<std::size_t>(fi)];
        for (auto &el : fr.elements) {
            if (el.child < 0 || el.child >= static_cast<int>(clip.children.size())) {
                continue;
            }
            const MovieClipOriginal::Child &ch = clip.children[static_cast<std::size_t>(el.child)];
            if (!ch.name.empty() && skipClip(ch.name)) {
                continue;
            }
            Matrix2x3 local;
            local.setIdentity();
            if (el.matrix != 65535 && el.matrix >= 0 && el.matrix < static_cast<int>(swf_->matrices.size())) {
                local = swf_->matrices[static_cast<std::size_t>(el.matrix)];
            }
            Matrix2x3 world = local;
            world.multiply(parent);
            const ColorTransform *ct = nullptr;
            if (el.color != 65535 && el.color >= 0 && el.color < static_cast<int>(swf_->colors.size())) {
                ct = &swf_->colors[static_cast<std::size_t>(el.color)];
            }
            if (findClip(ch.id)) {
                int sub = 0;
                if (ch.name == "loading_bar" || ch.name == "progress_bar") {
                    sub = barFrame_;
                }
                drawClip(*findClip(ch.id), sub, world, frame, w, h);
            } else if (findShape(ch.id)) {
                drawShape(*findShape(ch.id), world, ct, frame, w, h);
            } else if (findField(ch.id)) {
                if (ch.name == statusName_) {
                    drawField(*findField(ch.id), world, frame, w, h);
                }
            }
        }
    }
    void drawShape(const ShapeOriginal &shape, const Matrix2x3 &m, const ColorTransform *ct, std::vector<std::uint32_t> &frame, int w, int h) {
        for (auto &c : shape.commands) {
            if (c.texture < 0 || c.texture >= static_cast<int>(swf_->textures.size())) {
                continue;
            }
            const SWFTexture &tex = swf_->textures[static_cast<std::size_t>(c.texture)];
            if (tex.pixels.empty()) {
                continue;
            }
            std::size_t n = c.x.size();
            if (n < 3) {
                continue;
            }
            for (std::size_t i = 1; i + 1 < n; i++) {
                drawTri(tex, c, 0, i, i + 1, m, ct, frame, w, h);
            }
        }
    }
    void drawTri(const SWFTexture &tex, const ShapeOriginal::Command &c, std::size_t i0, std::size_t i1, std::size_t i2, const Matrix2x3 &m, const ColorTransform *ct, std::vector<std::uint32_t> &frame, int w, int h) {
        float ax = toX(m.applyX(c.x[i0], c.y[i0]));
        float ay = toY(m.applyY(c.x[i0], c.y[i0]));
        float bx = toX(m.applyX(c.x[i1], c.y[i1]));
        float by = toY(m.applyY(c.x[i1], c.y[i1]));
        float cx = toX(m.applyX(c.x[i2], c.y[i2]));
        float cy = toY(m.applyY(c.x[i2], c.y[i2]));
        float au = c.u[i0] / 65535.0f;
        float av = c.v[i0] / 65535.0f;
        float bu = c.u[i1] / 65535.0f;
        float bv = c.v[i1] / 65535.0f;
        float cu = c.u[i2] / 65535.0f;
        float cv = c.v[i2] / 65535.0f;
        float minX = ax;
        if (bx < minX) minX = bx;
        if (cx < minX) minX = cx;
        float maxX = ax;
        if (bx > maxX) maxX = bx;
        if (cx > maxX) maxX = cx;
        float minY = ay;
        if (by < minY) minY = by;
        if (cy < minY) minY = cy;
        float maxY = ay;
        if (by > maxY) maxY = by;
        if (cy > maxY) maxY = cy;
        int x0 = static_cast<int>(minX);
        int x1 = static_cast<int>(maxX) + 1;
        int y0 = static_cast<int>(minY);
        int y1 = static_cast<int>(maxY) + 1;
        if (x0 < 0) x0 = 0;
        if (y0 < 0) y0 = 0;
        if (x1 > w) x1 = w;
        if (y1 > h) y1 = h;
        float den = (by - cy) * (ax - cx) + (cx - bx) * (ay - cy);
        if (den > -0.0001f && den < 0.0001f) {
            return;
        }
        for (int y = y0; y < y1; y++) {
            for (int x = x0; x < x1; x++) {
                float px = x + 0.5f;
                float py = y + 0.5f;
                float l0 = ((by - cy) * (px - cx) + (cx - bx) * (py - cy)) / den;
                float l1 = ((cy - ay) * (px - cx) + (ax - cx) * (py - cy)) / den;
                float l2 = 1.0f - l0 - l1;
                if (l0 < 0 || l1 < 0 || l2 < 0) {
                    continue;
                }
                float u = l0 * au + l1 * bu + l2 * cu;
                float v = l0 * av + l1 * bv + l2 * cv;
                std::uint32_t src = tex.sample(u, v);
                if (ct) {
                    src = ct->apply(src);
                }
                unsigned sa = (src >> 24) & 0xFF;
                if (sa == 0) {
                    continue;
                }
                std::size_t k = static_cast<std::size_t>(y) * static_cast<std::size_t>(w) + static_cast<std::size_t>(x);
                if (sa == 255) {
                    frame[k] = 0xFF000000u | (src & 0x00FFFFFFu);
                } else {
                    std::uint32_t dst = frame[k];
                    unsigned sr = (src >> 16) & 0xFF;
                    unsigned sg = (src >> 8) & 0xFF;
                    unsigned sb = src & 0xFF;
                    unsigned dr = (dst >> 16) & 0xFF;
                    unsigned dg = (dst >> 8) & 0xFF;
                    unsigned db = dst & 0xFF;
                    unsigned r = (sr * sa + dr * (255 - sa)) / 255;
                    unsigned g = (sg * sa + dg * (255 - sa)) / 255;
                    unsigned b = (sb * sa + db * (255 - sa)) / 255;
                    frame[k] = 0xFF000000u | (r << 16) | (g << 8) | b;
                }
            }
        }
    }
    void drawField(const TextFieldOriginal &f, const Matrix2x3 &m, std::vector<std::uint32_t> &frame, int w, int h) {
        if (statusText_.empty()) {
            return;
        }
        float x0 = toX(m.applyX(static_cast<float>(f.left), static_cast<float>(f.top)));
        float x1 = toX(m.applyX(static_cast<float>(f.right), static_cast<float>(f.bottom)));
        float y0 = toY(m.applyX(static_cast<float>(f.left), static_cast<float>(f.top)));
        float y1 = toY(m.applyX(static_cast<float>(f.right), static_cast<float>(f.bottom)));
        if (x1 < x0) {
            float t = x0;
            x0 = x1;
            x1 = t;
        }
        if (y1 < y0) {
            float t = y0;
            y0 = y1;
            y1 = t;
        }
        float boxW = x1 - x0;
        float boxH = y1 - y0;
        if (boxW < 4 || boxH < 4) {
            return;
        }
#ifdef TITAN_HAS_FREETYPE
        int px = static_cast<int>(f.size * scale_);
        if (px < 1) {
            px = 1;
        }
        fonts_.drawCentered(frame, w, h, statusText_, x0, y0, boxW, boxH, px, static_cast<std::uint32_t>(f.color));
#else
        int tw = BitmapFont::measure(statusText_);
        if (tw <= 0) {
            return;
        }
        int scale = static_cast<int>(boxW / tw);
        int scaleH = static_cast<int>(boxH / 7);
        if (scaleH < scale) {
            scale = scaleH;
        }
        if (scale < 1) {
            scale = 1;
        }
        int sw = tw * scale;
        int sh = 7 * scale;
        int dx = static_cast<int>(x0 + (boxW - sw) * 0.5f);
        int dy = static_cast<int>(y0 + (boxH - sh) * 0.5f);
        BitmapFont::drawText(frame, w, h, statusText_, dx, dy, scale, static_cast<std::uint32_t>(f.color));
#endif
    }
    float toX(float dx) const {
        return ox_ + dx * scale_;
    }
    float toY(float dy) const {
        return oy_ + dy * scale_;
    }
    SupercellSWF *swf_ = nullptr;
    int barFrame_ = 0;
#ifdef TITAN_HAS_FREETYPE
    FontEngine fonts_;
#endif
    std::string statusName_ = "text";
    std::string statusText_;
    float scale_ = 1;
    float ox_ = 0;
    float oy_ = 0;
};
