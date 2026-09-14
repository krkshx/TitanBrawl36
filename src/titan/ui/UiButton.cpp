#pragma once
#include "../gfx/SupercellSWF.cpp"
#include <cstdint>
#include <string>
#include <vector>

// Кликабельная кнопка из ui.sc: берёт текстуру первого шейпа клипа-экспорта,
// рисует её прямоугольником слева снизу, отдаёт хит-тест в пикселях фреймбуфера.
class UiButton {
public:
    struct Rect {
        int x = 0;
        int y = 0;
        int w = 0;
        int h = 0;
    };
    // needle — подстрока имени экспорта (регистр не важен), напр. "button".
    bool bind(SupercellSWF *swf, const std::string &needle) {
        unbind();
        if (!swf || needle.empty()) {
            return false;
        }
        swf_ = swf;
        for (std::size_t i = 0; i < swf_->exports.size(); i++) {
            if (!contains(swf_->exports[i].name, needle)) {
                continue;
            }
            const SWFTexture *tex = textureOf(swf_->exports[i].id);
            if (!tex) {
                continue;
            }
            tex_ = tex;
            asset_ = swf_->exports[i].name;
            return true;
        }
        unbind();
        return false;
    }
    void unbind() {
        swf_ = nullptr;
        tex_ = nullptr;
        asset_.clear();
        label_.clear();
    }
    bool bound() const {
        return tex_ != nullptr;
    }
    const std::string &assetName() const {
        return asset_;
    }
    // Подпись кнопки (аналог GameButton::setText): пустая — не рисуется.
    void setLabel(const std::string &text) {
        label_ = text;
    }
    const std::string &label() const {
        return label_;
    }
    Rect slotRect(int w, int h) const {
        return rect(w, h);
    }
    // Слот меню: переопределить геометрию (пиксели фреймбуфера).
    // По умолчанию — кнопка входа слева снизу 200x64.
    void setSlot(int x, int y, int w, int h) {
        slot_.x = x;
        slot_.y = y;
        slot_.w = w;
        slot_.h = h;
        hasSlot_ = true;
    }
    void draw(std::vector<std::uint32_t> &frame, int w, int h) const {
        if (!tex_ || tex_->pixels.empty() || frame.empty()) {
            return;
        }
        Rect rc = rect(w, h);
        for (int y = 0; y < rc.h; y++) {
            int dy = rc.y + y;
            if (dy < 0 || dy >= h) {
                continue;
            }
            float v = static_cast<float>(y) / static_cast<float>(rc.h);
            for (int x = 0; x < rc.w; x++) {
                int dx = rc.x + x;
                if (dx < 0 || dx >= w) {
                    continue;
                }
                float u = static_cast<float>(x) / static_cast<float>(rc.w);
                std::uint32_t src = tex_->sampleBilinear(u, v);
                unsigned sa = (src >> 24) & 0xFF;
                if (sa == 0) {
                    continue;
                }
                std::size_t k = static_cast<std::size_t>(dy) * static_cast<std::size_t>(w) + static_cast<std::size_t>(dx);
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
    // fx/fy — пиксели фреймбуфера (такие отдаёт Window::takeClick).
    bool hit(int fx, int fy, int w, int h) const {
        if (!tex_) {
            return false;
        }
        Rect rc = rect(w, h);
        return fx >= rc.x && fx < rc.x + rc.w && fy >= rc.y && fy < rc.y + rc.h;
    }
private:
    // Фикс-геометрия кнопки входа: 200x64, слева снизу с отступом 24.
    Rect rect(int w, int h) const {
        if (hasSlot_) {
            return slot_;
        }
        Rect rc;
        rc.w = 200;
        rc.h = 64;
        rc.x = 24;
        rc.y = h - 24 - rc.h;
        if (rc.y < 0) {
            rc.y = 0;
        }
        (void)w;
        return rc;
    }
    static bool contains(const std::string &name, const std::string &needle) {
        if (needle.size() > name.size()) {
            return false;
        }
        for (std::size_t i = 0; i + needle.size() <= name.size(); i++) {
            bool ok = true;
            for (std::size_t k = 0; k < needle.size(); k++) {
                if (lower(name[i + k]) != lower(needle[k])) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                return true;
            }
        }
        return false;
    }
    static char lower(char c) {
        return (c >= 'A' && c <= 'Z') ? static_cast<char>(c + 32) : c;
    }
    // Текстура первого шейпа среди прямых детей клипа-экспорта.
    const SWFTexture *textureOf(int exportId) const {
        for (std::size_t i = 0; i < swf_->clips.size(); i++) {
            if (swf_->clips[i].id != exportId) {
                continue;
            }
            const MovieClipOriginal &clip = swf_->clips[i];
            for (std::size_t c = 0; c < clip.children.size(); c++) {
                const ShapeOriginal *shape = findShape(clip.children[c].id);
                if (!shape || shape->commands.empty()) {
                    continue;
                }
                int ti = shape->commands[0].texture;
                if (ti < 0 || ti >= static_cast<int>(swf_->textures.size())) {
                    continue;
                }
                const SWFTexture &tex = swf_->textures[static_cast<std::size_t>(ti)];
                if (tex.pixels.empty()) {
                    continue;
                }
                return &tex;
            }
            return nullptr;
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
    SupercellSWF *swf_ = nullptr;
    const SWFTexture *tex_ = nullptr;
    std::string asset_;
    std::string label_;
    Rect slot_;
    bool hasSlot_ = false;
};
