#pragma once
#include "NativeFont.cpp"
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
        (void)frame;
    }
    void setProgress(float p) {
        if (p < 0) {
            p = 0;
        }
        if (p > 1) {
            p = 1;
        }
        progress_ = p;
    }
    void setStatusText(const std::string &text) {
        statusText_ = text;
    }
    void setFonts(const std::vector<std::string> &paths) {
#ifdef TITAN_HAS_FREETYPE
        fonts_.load(paths);
#else
        (void)paths;
#endif
    }
    void setSystemFonts(const std::vector<std::string> &paths) {
#ifdef TITAN_HAS_FREETYPE
        nativeFonts_.load(paths);
#else
        (void)paths;
#endif
    }
    void setAssetDir(const std::string &assetsDir) {
        assetsDir_ = assetsDir;
#ifdef TITAN_HAS_FREETYPE
        fonts_.loadBundled(assetsDir);
        nativeFonts_.loadSystem(assetsDir, "Times New Roman", false);
#else
        (void)assetsDir;
#endif
    }
    void renderLogo(std::vector<std::uint32_t> &frame, int w, int h, int rootId) {
        if (!setupStage(frame, w, h)) {
            return;
        }
        const MovieClipOriginal *mc = findClip(rootId);
        if (!mc || mc->frames.empty()) {
            return;
        }
        int fi = static_cast<int>(mc->frames.size()) / 2;
        Matrix2x3 base;
        base.setIdentity();
        drawLogoFrame(*mc, fi, base, frame, w, h);
    }
    void render(std::vector<std::uint32_t> &frame, int w, int h, int rootId) {
        if (!setupStage(frame, w, h)) {
            return;
        }
        const MovieClipOriginal *root = findClip(rootId);
        if (!root) {
            return;
        }
        Matrix2x3 base;
        base.setIdentity();
        drawClip(*root, 0, base, frame, w, h);
    }
private:
    bool setupStage(std::vector<std::uint32_t> &frame, int w, int h) {
        if (!swf_ || frame.empty()) {
            return false;
        }
        for (std::size_t i = 0; i < frame.size(); i++) {
            frame[i] = 0xFF000000u;
        }
        scale_ = w / 1288.0f;
        float s2 = h / 768.0f;
        if (s2 > scale_) {
            scale_ = s2;
        }
        ox_ = w * 0.5f;
        oy_ = h * 0.5f;
        return true;
    }
    static bool skipClip(const std::string &name) {
        return name == "supercell_id" || name == "buttons_tencent" || name == "buttons_yoozoo" || name == "icon_prc_age" || name == "logo_KR" || name == "logo_JP" || name == "logo_CNT" || name == "logo_CNS";
    }
    const MovieClipOriginal *findClip(int id) const {
        for (std::size_t i = 0; i < swf_->clips.size(); i++) {
            if (swf_->clips[i].id == id) {
                return &swf_->clips[i];
            }
        }
        return nullptr;
    }
    const ShapeOriginal *findShape(int id) const {
        for (std::size_t i = 0; i < swf_->shapes.size(); i++) {
            if (swf_->shapes[i].id == id) {
                return &swf_->shapes[i];
            }
        }
        return nullptr;
    }
    const TextFieldOriginal *findField(int id) const {
        for (std::size_t i = 0; i < swf_->fields.size(); i++) {
            if (swf_->fields[i].id == id) {
                return &swf_->fields[i];
            }
        }
        return nullptr;
    }
    int progressFrame(const MovieClipOriginal &mc) const {
        if (mc.frames.empty()) {
            return 0;
        }
        int n = static_cast<int>(mc.frames.size());
        int fi = static_cast<int>(progress_ * static_cast<float>(n - 1) + 0.5f);
        if (fi < 0) {
            fi = 0;
        }
        if (fi >= n) {
            fi = n - 1;
        }
        return fi;
    }
    void drawLogoFrame(const MovieClipOriginal &clip, int frameIndex, const Matrix2x3 &parent, std::vector<std::uint32_t> &frame, int w, int h) {
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
        for (std::size_t k = 0; k < fr.elements.size(); k++) {
            const MovieClipOriginal::Element &el = fr.elements[k];
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
                drawLogoFrame(*findClip(ch.id), 0, world, frame, w, h);
            } else if (findShape(ch.id)) {
                drawShape(*findShape(ch.id), local, parent, ct, frame, w, h);
            }
        }
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
        for (std::size_t k = 0; k < fr.elements.size(); k++) {
            const MovieClipOriginal::Element &el = fr.elements[k];
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
                if (ch.name == "loading_bar") {
                    drawLoadingBar(*findClip(ch.id), world, frame, w, h);
                } else if (ch.name == "progress_bar") {
                    drawClip(*findClip(ch.id), progressFrame(*findClip(ch.id)), world, frame, w, h);
                } else {
                    drawClip(*findClip(ch.id), 0, world, frame, w, h);
                }
            } else if (findShape(ch.id)) {
                drawShape(*findShape(ch.id), local, parent, ct, frame, w, h);
            } else if (findField(ch.id)) {
                if (ch.name == statusName_) {
                    drawField(*findField(ch.id), world, frame, w, h);
                }
            }
        }
    }
    void drawLoadingBar(const MovieClipOriginal &bar, const Matrix2x3 &parent, std::vector<std::uint32_t> &frame, int w, int h) {
        if (bar.frames.empty()) {
            return;
        }
        int fi = progressFrame(bar);
        const MovieClipOriginal::Frame &fr = bar.frames[static_cast<std::size_t>(fi)];
        for (std::size_t k = 0; k < fr.elements.size(); k++) {
            const MovieClipOriginal::Element &el = fr.elements[k];
            if (el.child < 0 || el.child >= static_cast<int>(bar.children.size())) {
                continue;
            }
            const MovieClipOriginal::Child &ch = bar.children[static_cast<std::size_t>(el.child)];
            Matrix2x3 world = elementMatrix(el.matrix);
            world.multiply(parent);
            if (findField(ch.id)) {
                if (ch.name == statusName_) {
                    drawField(*findField(ch.id), world, frame, w, h);
                }
            } else if (findClip(ch.id) && ch.name == "progress_bar") {
                drawClip(*findClip(ch.id), progressFrame(*findClip(ch.id)), world, frame, w, h);
            } else if (findClip(ch.id)) {
                drawClip(*findClip(ch.id), 0, world, frame, w, h);
            }
        }
    }
    Matrix2x3 elementMatrix(int el) const {
        Matrix2x3 local;
        local.setIdentity();
        if (el != 65535 && el >= 0 && el < static_cast<int>(swf_->matrices.size())) {
            local = swf_->matrices[static_cast<std::size_t>(el)];
        }
        return local;
    }
    static bool useNativeFont(const TextFieldOriginal &f) {
        if (f.deviceFont) {
            return true;
        }
        return NativeFont::isSystemFamily(f.font);
    }
    static std::uint32_t outlineColorFor(const TextFieldOriginal &f) {
        if (!f.outline) {
            return 0xFF000000u;
        }
        std::uint32_t c = static_cast<std::uint32_t>(f.outlineColor);
        if (((c >> 24) & 0xFF) == 0 && (c & 0x00FFFFFFu) == 0) {
            return 0xFF000000u;
        }
        return c;
    }
    static bool mapSlice(const std::vector<float> &grid, float scale, float trans, std::vector<float> &mapped) {
        std::size_t n = grid.size();
        if (n < 2) {
            return false;
        }
        mapped.clear();
        mapped.resize(n);
        mapped[0] = grid[0] * scale + trans;
        mapped[n - 1] = grid[n - 1] * scale + trans;
        for (std::size_t i = 1; i + 1 < n; i++) {
            if (i * 2 < n) {
                mapped[i] = mapped[0] + (grid[i] - grid[0]);
            } else {
                mapped[i] = mapped[n - 1] - (grid[n - 1] - grid[i]);
            }
        }
        for (std::size_t i = 0; i + 1 < n; i++) {
            if (mapped[i + 1] < mapped[i]) {
                return false;
            }
        }
        return true;
    }
    void drawShape(const ShapeOriginal &shape, const Matrix2x3 &local, const Matrix2x3 &parent, const ColorTransform *ct, std::vector<std::uint32_t> &frame, int w, int h) {
        Matrix2x3 world = local;
        world.multiply(parent);
        if (shape.nineSlice && world.b == 0.0f && world.c == 0.0f && world.a > 0.0f && world.d > 0.0f) {
            std::vector<float> mapX;
            std::vector<float> mapY;
            if (mapSlice(shape.gridX, world.a, world.x, mapX) && mapSlice(shape.gridY, world.d, world.y, mapY)) {
                for (std::size_t ci = 0; ci < shape.commands.size(); ci++) {
                    const ShapeOriginal::Command &c = shape.commands[ci];
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
                    ShapeOriginal::Command mc = c;
                    bool ok = true;
                    for (std::size_t vi = 0; vi < n; vi++) {
                        int ix = shape.gridIndexX(c.x[vi]);
                        int iy = shape.gridIndexY(c.y[vi]);
                        if (ix < 0 || iy < 0) {
                            ok = false;
                            break;
                        }
                        mc.x[vi] = mapX[static_cast<std::size_t>(ix)];
                        mc.y[vi] = mapY[static_cast<std::size_t>(iy)];
                    }
                    if (!ok) {
                        continue;
                    }
                    Matrix2x3 ident;
                    ident.setIdentity();
                    for (std::size_t i = 1; i + 1 < n; i++) {
                        drawTri(tex, mc, 0, i, i + 1, ident, ct, frame, w, h);
                    }
                }
                return;
            }
        }
        for (std::size_t ci = 0; ci < shape.commands.size(); ci++) {
            const ShapeOriginal::Command &c = shape.commands[ci];
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
                drawTri(tex, c, 0, i, i + 1, world, ct, frame, w, h);
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
                std::uint32_t src = tex.sampleBilinear(u, v);
                if (ct) {
                    src = ct->apply(src);
                }
                unsigned sa = (src >> 24) & 0xFF;
                if (sa == 0) {
                    continue;
                }
                std::size_t kk = static_cast<std::size_t>(y) * static_cast<std::size_t>(w) + static_cast<std::size_t>(x);
                if (sa == 255) {
                    frame[kk] = 0xFF000000u | (src & 0x00FFFFFFu);
                } else {
                    std::uint32_t dst = frame[kk];
                    unsigned sr = (src >> 16) & 0xFF;
                    unsigned sg = (src >> 8) & 0xFF;
                    unsigned sb = src & 0xFF;
                    unsigned dr = (dst >> 16) & 0xFF;
                    unsigned dg = (dst >> 8) & 0xFF;
                    unsigned db = dst & 0xFF;
                    unsigned r = (sr * sa + dr * (255 - sa)) / 255;
                    unsigned g = (sg * sa + dg * (255 - sa)) / 255;
                    unsigned b = (sb * sa + db * (255 - sa)) / 255;
                    frame[kk] = 0xFF000000u | (r << 16) | (g << 8) | b;
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
        float y0 = toY(m.applyY(static_cast<float>(f.left), static_cast<float>(f.top)));
        float y1 = toY(m.applyY(static_cast<float>(f.right), static_cast<float>(f.bottom)));
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
        std::uint32_t col = static_cast<std::uint32_t>(f.color);
        std::uint32_t ocol = outlineColorFor(f);
        if (useNativeFont(f)) {
            nativeFonts_.drawCentered(frame, w, h, statusText_, x0, y0, boxW, boxH, px, col, f.outline, ocol);
        } else {
            fonts_.drawCentered(frame, w, h, statusText_, x0, y0, boxW, boxH, px, col, f.outline, ocol);
        }
#else
        int tw = BitmapFont::measure(statusText_);
        if (tw <= 0) {
            return;
        }
        int sc = static_cast<int>(boxW / tw);
        int scH = static_cast<int>(boxH / 7);
        if (scH < sc) {
            sc = scH;
        }
        if (sc < 1) {
            sc = 1;
        }
        int sw = tw * sc;
        int sh = 7 * sc;
        int dx = static_cast<int>(x0 + (boxW - sw) * 0.5f);
        int dy = static_cast<int>(y0 + (boxH - sh) * 0.5f);
        std::uint32_t col = static_cast<std::uint32_t>(f.color);
        if (f.outline) {
            std::uint32_t ocol = outlineColorFor(f);
            BitmapFont::drawText(frame, w, h, statusText_, dx - sc, dy, sc, ocol);
            BitmapFont::drawText(frame, w, h, statusText_, dx + sc, dy, sc, ocol);
            BitmapFont::drawText(frame, w, h, statusText_, dx, dy - sc, sc, ocol);
            BitmapFont::drawText(frame, w, h, statusText_, dx, dy + sc, sc, ocol);
        }
        BitmapFont::drawText(frame, w, h, statusText_, dx, dy, sc, col);
#endif
    }
    float toX(float dx) const {
        return ox_ + dx * scale_;
    }
    float toY(float dy) const {
        return oy_ + dy * scale_;
    }
    SupercellSWF *swf_ = nullptr;
    float progress_ = 0;
#ifdef TITAN_HAS_FREETYPE
    FontEngine fonts_;
    FontEngine nativeFonts_;
#endif
    std::string assetsDir_;
    std::string statusName_ = "text";
    std::string statusText_;
    float scale_ = 1;
    float ox_ = 0;
    float oy_ = 0;
};
