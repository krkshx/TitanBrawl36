#pragma once
#include "../gfx/SupercellSWF.cpp"
#include <cmath>
#include <cstdint>
#include <string>
#include <vector>

// Кнопка меню как в ориге (GameButton): скин — 9-slice шейп из ui.sc
// (напр. popover_button_green -> shape с grid), текст — игровым шрифтом
// поверх (drawLabel). Старый код брал commands[0].texture и растягивал ВЕСЬ
// атлас 4096x4096 на слот 220x64 — отсюда цветной мусор на скрине.
// Здесь рендерим шейп с его родными UV (как ClipRenderer::drawTri) и
// 9-slice растяжкой сетки в слот.
class UiButton {
public:
    struct Rect {
        int x = 0;
        int y = 0;
        int w = 0;
        int h = 0;
    };
    // Точное имя экспорта ("popover_button_green"), без подстрок:
    // поиск по подстроке ("play"/"shop"/"brawler") цеплял первые попавшиеся
    // клипы (chat_letsplay_gemgrab, shop_item_resourses_gems, ...) — тоже мусор.
    bool bind(SupercellSWF *swf, const std::string &exactName) {
        unbind();
        if (!swf || exactName.empty()) {
            return false;
        }
        int exportId = -1;
        for (std::size_t i = 0; i < swf->exports.size(); i++) {
            if (swf->exports[i].name == exactName) {
                exportId = swf->exports[i].id;
                break;
            }
        }
        if (exportId < 0) {
            unbind();
            return false;
        }
        return bindClipId(swf, exportId, exactName);
    }
    // Бинд напрямую по id клипа (кнопки меню лежат внутри HUD-клипов
    // без собственных экспортов: brawl_button=10274, navi=10183/...).
    bool bindClip(SupercellSWF *swf, int clipId) {
        unbind();
        if (!swf) {
            return false;
        }
        return bindClipId(swf, clipId, "");
    }
    // Бинд сырого шейпа (фон поля ввода: 9678): только скин, без иконки.
    bool bindShape(SupercellSWF *swf, int shapeId) {
        unbind();
        if (!swf) {
            return false;
        }
        swf_ = swf;
        for (std::size_t i = 0; i < swf_->shapes.size(); i++) {
            if (swf_->shapes[i].id == shapeId && !swf_->shapes[i].commands.empty() &&
                hasLiveTexture(swf_->shapes[i])) {
                shape_ = &swf_->shapes[i];
                asset_.clear();
                measure();
                return true;
            }
        }
        unbind();
        return false;
    }
    void unbind() {
        swf_ = nullptr;
        shape_ = nullptr;
        icon_ = nullptr;
        badge_ = nullptr;
        asset_.clear();
        label_.clear();
        minX_ = minY_ = maxX_ = maxY_ = 0;
        iminX_ = iminY_ = imaxX_ = imaxY_ = 0;
    }
    bool bound() const {
        return shape_ != nullptr;
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
    // Где рисовать подпись: 0 — по центру слота (PLAY, OK),
    // 1 — нижняя полоса слота (нави-кнопки: текст под иконкой как в ориге).
    void setLabelAlign(int align) {
        labelAlign_ = align;
    }
    Rect slotRect(int w, int h) const {
        return rect(w, h);
    }
    // Прямоугольник подписи: центр слота (0) или нижняя полоса (1, нави).
    Rect labelRect(int w, int h) const {
        Rect rc = rect(w, h);
        if (labelAlign_ == 1) {
            int top = rc.y + (rc.h * 58) / 100;
            rc.h = rc.y + rc.h - top;
            rc.y = top;
        }
        return rc;
    }
    // Бейдж уведомлений — арт из самой кнопки (notification-шейп либы).
    bool hasBadge() const {
        return badge_ != nullptr;
    }
    Rect badgeRect(int w, int h) const {
        Rect rc = rect(w, h);
        const int s = 26;
        Rect b;
        b.w = s;
        b.h = s;
        b.x = rc.x + rc.w - s + 4;
        b.y = rc.y - 4;
        return b;
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
        if (!shape_ || !swf_ || frame.empty()) {
            return;
        }
        Rect rc = rect(w, h);
        if (rc.w < 4 || rc.h < 4) {
            return;
        }
        float shapeW = maxX_ - minX_;
        float shapeH = maxY_ - minY_;
        if (shapeW <= 0 || shapeH <= 0) {
            return;
        }
        // 9-slice маппинг сетки шейпа в слот (как ClipRenderer::mapSlice:
        // края сохраняют размер, тянется только середина).
        std::vector<float> mapX;
        std::vector<float> mapY;
        bool sliced = false;
        if (shape_->nineSlice && shape_->gridX.size() >= 2 && shape_->gridY.size() >= 2) {
            if (mapSlice(shape_->gridX, minX_, maxX_, static_cast<float>(rc.x),
                         static_cast<float>(rc.x + rc.w), mapX) &&
                mapSlice(shape_->gridY, minY_, maxY_, static_cast<float>(rc.y),
                         static_cast<float>(rc.y + rc.h), mapY)) {
                sliced = true;
            }
        }
        for (std::size_t ci = 0; ci < shape_->commands.size(); ci++) {
            const ShapeOriginal::Command &c = shape_->commands[ci];
            if (c.texture < 0 || c.texture >= static_cast<int>(swf_->textures.size())) {
                continue;
            }
            const SWFTexture &tex = swf_->textures[static_cast<std::size_t>(c.texture)];
            if (tex.pixels.empty()) {
                continue;
            }
            std::size_t n = c.x.size();
            if (n < 3 || c.u.size() < n || c.v.size() < n) {
                continue;
            }
            ShapeOriginal::Command mc = c;
            if (sliced) {
                bool ok = true;
                for (std::size_t vi = 0; vi < n; vi++) {
                    int ix = shape_->gridIndexX(c.x[vi]);
                    int iy = shape_->gridIndexY(c.y[vi]);
                    if (ix < 0 || iy < 0 || ix >= static_cast<int>(mapX.size()) ||
                        iy >= static_cast<int>(mapY.size())) {
                        ok = false;
                        break;
                    }
                    mc.x[vi] = mapX[static_cast<std::size_t>(ix)];
                    mc.y[vi] = mapY[static_cast<std::size_t>(iy)];
                }
                if (!ok) {
                    continue;
                }
            } else {
                float sx = static_cast<float>(rc.w) / shapeW;
                float sy = static_cast<float>(rc.h) / shapeH;
                for (std::size_t vi = 0; vi < n; vi++) {
                    mc.x[vi] = static_cast<float>(rc.x) + (c.x[vi] - minX_) * sx;
                    mc.y[vi] = static_cast<float>(rc.y) + (c.y[vi] - minY_) * sy;
                }
            }
            drawCommand(tex, mc, frame, w, h);
        }
        drawIcon(frame, w, h, rc);
        drawBadge(frame, w, h, rc);
    }
    // fx/fy — пиксели фреймбуфера (такие отдаёт Window::takeClick).
    bool hit(int fx, int fy, int w, int h) const {
        if (!shape_) {
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
    void measure() {
        bool first = true;
        for (std::size_t i = 0; i < shape_->commands.size(); i++) {
            const ShapeOriginal::Command &c = shape_->commands[i];
            for (std::size_t k = 0; k < c.x.size(); k++) {
                if (first) {
                    minX_ = maxX_ = c.x[k];
                    minY_ = maxY_ = c.y[k];
                    first = false;
                } else {
                    if (c.x[k] < minX_) minX_ = c.x[k];
                    if (c.x[k] > maxX_) maxX_ = c.x[k];
                    if (c.y[k] < minY_) minY_ = c.y[k];
                    if (c.y[k] > maxY_) maxY_ = c.y[k];
                }
            }
        }
    }
    // Поиск скина: экспорт -> клип -> дети кадра 0 (рекурсивно): первый
    // 9-slice шейп — скин кнопки; запасной — первый шейп с командами.
    void findSkin(int id, int depth, const ShapeOriginal *&nine, const ShapeOriginal *&any) const {
        for (std::size_t i = 0; i < swf_->shapes.size(); i++) {
            if (swf_->shapes[i].id == id) {
                const ShapeOriginal &sh = swf_->shapes[i];
                if (!sh.commands.empty()) {
                    if (!any) {
                        any = &sh;
                    }
                    if (sh.nineSlice && !nine) {
                        nine = &sh;
                    }
                }
                return;
            }
        }
        if (depth >= 4) {
            return;
        }
        for (std::size_t i = 0; i < swf_->clips.size(); i++) {
            if (swf_->clips[i].id != id) {
                continue;
            }
            const MovieClipOriginal &clip = swf_->clips[i];
            if (clip.frames.empty()) {
                return;
            }
            const MovieClipOriginal::Frame &fr = clip.frames[0];
            for (std::size_t k = 0; k < fr.elements.size(); k++) {
                const MovieClipOriginal::Element &el = fr.elements[k];
                if (el.child < 0 || el.child >= static_cast<int>(clip.children.size())) {
                    continue;
                }
                const MovieClipOriginal::Child &ch = clip.children[static_cast<std::size_t>(el.child)];
                // Текстовые поля ('txt') — не скин.
                if (!ch.name.empty() && (ch.name == "txt" || ch.name == "text")) {
                    continue;
                }
                findSkin(ch.id, depth + 1, nine, any);
                if (nine && any) {
                    return;
                }
            }
            return;
        }
    }
    // Иконка кнопки (второй шейп клипа: картинка поверх скина).
    // Рисуется с сохранением пропорций в верхней зоне слота.
    void drawBadge(std::vector<std::uint32_t> &frame, int w, int h, const Rect &rc) const {
        if (!badge_ || !swf_) {
            return;
        }
        float bw = bmaxX_ - bminX_;
        float bh = bmaxY_ - bminY_;
        if (bw <= 0 || bh <= 0) {
            return;
        }
        Rect b = badgeRect(w, h);
        (void)rc;
        float k = static_cast<float>(b.w) / bw;
        float k2 = static_cast<float>(b.h) / bh;
        if (k2 < k) {
            k = k2;
        }
        if (k <= 0) {
            return;
        }
        float dw = bw * k;
        float dh = bh * k;
        float ox = static_cast<float>(b.x) + (static_cast<float>(b.w) - dw) * 0.5f;
        float oy = static_cast<float>(b.y) + (static_cast<float>(b.h) - dh) * 0.5f;
        for (std::size_t ci = 0; ci < badge_->commands.size(); ci++) {
            const ShapeOriginal::Command &c = badge_->commands[ci];
            if (c.texture < 0 || c.texture >= static_cast<int>(swf_->textures.size())) {
                continue;
            }
            const SWFTexture &tex = swf_->textures[static_cast<std::size_t>(c.texture)];
            if (tex.pixels.empty()) {
                continue;
            }
            std::size_t n = c.x.size();
            if (n < 3 || c.u.size() < n || c.v.size() < n) {
                continue;
            }
            ShapeOriginal::Command mc = c;
            for (std::size_t vi = 0; vi < n; vi++) {
                mc.x[vi] = ox + (c.x[vi] - bminX_) * k;
                mc.y[vi] = oy + (c.y[vi] - bminY_) * k;
            }
            drawCommand(tex, mc, frame, w, h);
        }
    }
    void drawIcon(std::vector<std::uint32_t> &frame, int w, int h, const Rect &rc) const {
        if (!icon_ || !swf_) {
            return;
        }
        float iw = imaxX_ - iminX_;
        float ih = imaxY_ - iminY_;
        if (iw <= 0 || ih <= 0) {
            return;
        }
        float zoneH = labelAlign_ == 1 ? static_cast<float>(rc.h) * 0.62f : static_cast<float>(rc.h) * 0.8f;
        float zoneW = static_cast<float>(rc.w) * 0.8f;
        float k = zoneW / iw;
        float k2 = zoneH / ih;
        if (k2 < k) {
            k = k2;
        }
        if (k <= 0) {
            return;
        }
        float dw = iw * k;
        float dh = ih * k;
        float ox = static_cast<float>(rc.x) + (static_cast<float>(rc.w) - dw) * 0.5f;
        float oy = labelAlign_ == 1 ? static_cast<float>(rc.y) + 4.0f
                                    : static_cast<float>(rc.y) + (static_cast<float>(rc.h) - dh) * 0.5f;
        for (std::size_t ci = 0; ci < icon_->commands.size(); ci++) {
            const ShapeOriginal::Command &c = icon_->commands[ci];
            if (c.texture < 0 || c.texture >= static_cast<int>(swf_->textures.size())) {
                continue;
            }
            const SWFTexture &tex = swf_->textures[static_cast<std::size_t>(c.texture)];
            if (tex.pixels.empty()) {
                continue;
            }
            std::size_t n = c.x.size();
            if (n < 3 || c.u.size() < n || c.v.size() < n) {
                continue;
            }
            ShapeOriginal::Command mc = c;
            for (std::size_t vi = 0; vi < n; vi++) {
                mc.x[vi] = ox + (c.x[vi] - iminX_) * k;
                mc.y[vi] = oy + (c.y[vi] - iminY_) * k;
            }
            drawCommand(tex, mc, frame, w, h);
        }
    }
    bool bindClipId(SupercellSWF *swf, int clipId, const std::string &assetName) {
        swf_ = swf;
        const ShapeOriginal *nine = nullptr;
        const ShapeOriginal *any = nullptr;
        findSkin(clipId, 0, nine, any);
        const ShapeOriginal *skin = nine ? nine : any;
        if (!skin || skin->commands.empty() || !hasLiveTexture(*skin)) {
            unbind();
            return false;
        }
        shape_ = skin;
        asset_ = assetName;
        measure();
        // Иконка: первый живой шейп, отличный от скина (обход по детям клипа).
        icon_ = nullptr;
        findIcon(clipId, 0);
        if (icon_) {
            measureIcon();
        }
        // Бейдж: шейп из notification-ветки той же кнопки.
        badge_ = nullptr;
        findBadge(clipId, 0);
        if (badge_) {
            measureBadge();
        }
        return true;
    }
    static bool hasLiveTextureStatic(const SupercellSWF *swf, const ShapeOriginal &sh) {
        for (std::size_t i = 0; i < sh.commands.size(); i++) {
            int ti = sh.commands[i].texture;
            if (ti >= 0 && ti < static_cast<int>(swf->textures.size()) &&
                !swf->textures[static_cast<std::size_t>(ti)].pixels.empty()) {
                return true;
            }
        }
        return false;
    }
    bool hasLiveTexture(const ShapeOriginal &sh) const {
        return hasLiveTextureStatic(swf_, sh);
    }
    void findIcon(int id, int depth) {
        // Сперва иконки под детьми с 'icon' в имени (profile_trophy_icon и т.п.),
        // иначе — самый крупный живой шейп (логотип кнопки, а не мелочь).
        float best = 0;
        const ShapeOriginal *anyBest = nullptr;
        float anyArea = 0;
        findIconBest(id, depth, false, best, anyBest, anyArea);
        if (!icon_ && anyBest) {
            icon_ = anyBest;
        }
    }
    // Бейдж уведомлений: первый живой шейп под ребёнком 'notification'.
    void findBadge(int id, int depth) {
        if (badge_ || depth > 4) {
            return;
        }
        for (std::size_t i = 0; i < swf_->shapes.size(); i++) {
            if (swf_->shapes[i].id == id) {
                return;
            }
        }
        for (std::size_t i = 0; i < swf_->clips.size(); i++) {
            if (swf_->clips[i].id != id) {
                continue;
            }
            const MovieClipOriginal &clip = swf_->clips[i];
            if (clip.frames.empty()) {
                return;
            }
            const MovieClipOriginal::Frame &fr = clip.frames[0];
            for (std::size_t k = 0; k < fr.elements.size(); k++) {
                const MovieClipOriginal::Element &el = fr.elements[k];
                if (el.child < 0 || el.child >= static_cast<int>(clip.children.size())) {
                    continue;
                }
                const MovieClipOriginal::Child &ch = clip.children[static_cast<std::size_t>(el.child)];
                if (ch.name == "notification") {
                    const ShapeOriginal *nine = nullptr;
                    const ShapeOriginal *any = nullptr;
                    findSkin(ch.id, depth + 1, nine, any);
                    const ShapeOriginal *sk = nine ? nine : any;
                    if (sk && sk != shape_ && sk != icon_) {
                        badge_ = sk;
                        return;
                    }
                    return;
                }
                findBadge(ch.id, depth + 1);
                if (badge_) {
                    return;
                }
            }
            return;
        }
    }
    void measureBadge() {
        bool first = true;
        for (std::size_t i = 0; i < badge_->commands.size(); i++) {
            const ShapeOriginal::Command &c = badge_->commands[i];
            for (std::size_t k = 0; k < c.x.size(); k++) {
                if (first) {
                    bminX_ = bmaxX_ = c.x[k];
                    bminY_ = bmaxY_ = c.y[k];
                    first = false;
                } else {
                    if (c.x[k] < bminX_) bminX_ = c.x[k];
                    if (c.x[k] > bmaxX_) bmaxX_ = c.x[k];
                    if (c.y[k] < bminY_) bminY_ = c.y[k];
                    if (c.y[k] > bmaxY_) bmaxY_ = c.y[k];
                }
            }
        }
    }
    // Оверлеи либы — не иконки: анимации, прогресс-бары, тиры, ранги, бейджи.
    static bool isOverlayName(const std::string &name) {        static const char *k[] = {"notification", "txt", "hit_area", "anim", "progress",
                                  "highlight", "tier", "rank", nullptr};
        for (int i = 0; k[i]; i++) {
            const char *p = k[i];
            std::size_t n = 0;
            while (p[n]) {
                n++;
            }
            if (n > name.size()) {
                continue;
            }
            for (std::size_t s = 0; s + n <= name.size(); s++) {
                bool ok = true;
                for (std::size_t j = 0; j < n; j++) {
                    char a = name[s + j];
                    if (a >= 'A' && a <= 'Z') {
                        a = static_cast<char>(a + 32);
                    }
                    if (a != p[j]) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    return true;
                }
            }
        }
        return false;
    }
    static bool hasIconName(const std::string &name) {
        if (name.size() < 4) {
            return false;
        }
        for (std::size_t i = 0; i + 4 <= name.size(); i++) {
            if ((name[i] == 'i' || name[i] == 'I') && (name[i + 1] == 'c' || name[i + 1] == 'C') &&
                (name[i + 2] == 'o' || name[i + 2] == 'O') && (name[i + 3] == 'n' || name[i + 3] == 'N')) {
                return true;
            }
        }
        return false;
    }
    // Иконка — самый крупный живой шейп, отличный от скина (логотип кнопки,
    // а не мелочь вроде прогресс-баров: у brawl_pass это icon_brawl_pass).
    void findIconBest(int id, int depth, bool iconBranch, float &best,
                      const ShapeOriginal *&anyBest, float &anyArea) {
        if (depth > 4) {
            return;
        }
        for (std::size_t i = 0; i < swf_->shapes.size(); i++) {
            if (swf_->shapes[i].id == id) {
                const ShapeOriginal &sh = swf_->shapes[i];
                if (&sh != shape_ && !sh.commands.empty() && hasLiveTexture(sh)) {
                    float a = shapeArea(sh);
                    if (iconBranch) {
                        if (a > best) {
                            best = a;
                            icon_ = &sh;
                        }
                    } else if (a > anyArea) {
                        anyArea = a;
                        anyBest = &sh;
                    }
                }
                return;
            }
        }
        for (std::size_t i = 0; i < swf_->clips.size(); i++) {
            if (swf_->clips[i].id != id) {
                continue;
            }
            const MovieClipOriginal &clip = swf_->clips[i];
            if (clip.frames.empty()) {
                return;
            }
            const MovieClipOriginal::Frame &fr = clip.frames[0];
            for (std::size_t k = 0; k < fr.elements.size(); k++) {
                const MovieClipOriginal::Element &el = fr.elements[k];
                if (el.child < 0 || el.child >= static_cast<int>(clip.children.size())) {
                    continue;
                }
                const MovieClipOriginal::Child &ch = clip.children[static_cast<std::size_t>(el.child)];
                if (!ch.name.empty() && isOverlayName(ch.name)) {
                    continue;
                }
                findIconBest(ch.id, depth + 1, iconBranch || hasIconName(ch.name), best, anyBest, anyArea);
            }
            return;
        }
    }
    static float shapeArea(const ShapeOriginal &sh) {
        bool first = true;
        float minX = 0, maxX = 0, minY = 0, maxY = 0;
        for (std::size_t i = 0; i < sh.commands.size(); i++) {
            const ShapeOriginal::Command &c = sh.commands[i];
            for (std::size_t k = 0; k < c.x.size(); k++) {
                if (first) {
                    minX = maxX = c.x[k];
                    minY = maxY = c.y[k];
                    first = false;
                } else {
                    if (c.x[k] < minX) minX = c.x[k];
                    if (c.x[k] > maxX) maxX = c.x[k];
                    if (c.y[k] < minY) minY = c.y[k];
                    if (c.y[k] > maxY) maxY = c.y[k];
                }
            }
        }
        if (first) {
            return 0;
        }
        return (maxX - minX) * (maxY - minY);
    }
    void measureIcon() {
        bool first = true;
        for (std::size_t i = 0; i < icon_->commands.size(); i++) {
            const ShapeOriginal::Command &c = icon_->commands[i];
            for (std::size_t k = 0; k < c.x.size(); k++) {
                if (first) {
                    iminX_ = imaxX_ = c.x[k];
                    iminY_ = imaxY_ = c.y[k];
                    first = false;
                } else {
                    if (c.x[k] < iminX_) iminX_ = c.x[k];
                    if (c.x[k] > imaxX_) imaxX_ = c.x[k];
                    if (c.y[k] < iminY_) iminY_ = c.y[k];
                    if (c.y[k] > imaxY_) imaxY_ = c.y[k];
                }
            }
        }
    }
    // со сдвигом от краёв (как ClipRenderer::mapSlice, но в пиксели слота).
    static bool mapSlice(const std::vector<float> &grid, float gmin, float gmax,
                         float dst0, float dst1, std::vector<float> &mapped) {
        std::size_t n = grid.size();
        if (n < 2 || gmax <= gmin || dst1 <= dst0) {
            return false;
        }
        mapped.clear();
        mapped.resize(n);
        // Нормируем сетку относительно границ шейпа.
        for (std::size_t i = 0; i < n; i++) {
            float t = (grid[i] - gmin) / (gmax - gmin);
            if (t < 0) t = 0;
            if (t > 1) t = 1;
            if (i == 0) {
                mapped[i] = dst0;
            } else if (i + 1 == n) {
                mapped[i] = dst1;
            } else if (i * 2 < n) {
                mapped[i] = dst0 + (grid[i] - gmin);
            } else {
                mapped[i] = dst1 - (gmax - grid[i]);
            }
            (void)t;
        }
        for (std::size_t i = 0; i + 1 < n; i++) {
            if (mapped[i + 1] < mapped[i]) {
                return false;
            }
        }
        return true;
    }
    // Ребро контура для AA (как ClipRenderer::PolyEdge).
    struct PolyEdge {
        float ax = 0;
        float ay = 0;
        float dx = 0;
        float dy = 0;
        float invLen = 0;
    };
    // Фэн mapped-команды с контурным AA.
    static void drawCommand(const SWFTexture &tex, const ShapeOriginal::Command &mc,
                            std::vector<std::uint32_t> &frame, int w, int h) {
        std::size_t n = mc.x.size();
        if (n < 3 || n > 256 || mc.y.size() < n) {
            return;
        }
        PolyEdge edges[256];
        std::size_t ne = 0;
        for (std::size_t vi = 0; vi < n; vi++) {
            std::size_t vj = (vi + 1) % n;
            float dx = mc.x[vj] - mc.x[vi];
            float dy = mc.y[vj] - mc.y[vi];
            float len = static_cast<float>(std::sqrt(dx * dx + dy * dy));
            if (len < 0.0001f) {
                continue;
            }
            edges[ne].ax = mc.x[vi];
            edges[ne].ay = mc.y[vi];
            edges[ne].dx = dx;
            edges[ne].dy = dy;
            edges[ne].invLen = 1.0f / len;
            ne++;
        }
        for (std::size_t i = 1; i + 1 < n; i++) {
            drawTri(tex, mc, 0, i, i + 1, edges, ne, frame, w, h);
        }
    }
    // Растер треугольника шейпа с его UV (как ClipRenderer::drawTri, но
    // координаты уже в пикселях фреймбуфера — без stage-трансформа).
    // AA — только по контуру команды (edges), не по рёбрам треугольника.
    static void drawTri(const SWFTexture &tex, const ShapeOriginal::Command &c,
                        std::size_t i0, std::size_t i1, std::size_t i2,
                        const PolyEdge *edges, std::size_t ne,
                        std::vector<std::uint32_t> &frame, int w, int h) {
        float ax = c.x[i0];
        float ay = c.y[i0];
        float bx = c.x[i1];
        float by = c.y[i1];
        float cx = c.x[i2];
        float cy = c.y[i2];
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
                float cov = 1.0f;
                if (ne > 0) {
                    float md = 1e30f;
                    for (std::size_t ei = 0; ei < ne; ei++) {
                        const PolyEdge &e = edges[ei];
                        float cr = e.dx * (py - e.ay) - e.dy * (px - e.ax);
                        if (cr < 0) {
                            cr = -cr;
                        }
                        float dd = cr * e.invLen;
                        if (dd < md) {
                            md = dd;
                        }
                    }
                    cov = md + 0.5f;
                    if (cov <= 0.0f) {
                        continue;
                    }
                    if (cov > 1.0f) {
                        cov = 1.0f;
                    }
                }
                float u = l0 * au + l1 * bu + l2 * cu;
                float v = l0 * av + l1 * bv + l2 * cv;
                std::uint32_t src = tex.sampleBilinear(u, v);
                unsigned sa = (src >> 24) & 0xFF;
                if (cov < 1.0f) {
                    sa = static_cast<unsigned>(sa * cov);
                    if (sa == 0) {
                        continue;
                    }
                } else if (sa == 0) {
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
    SupercellSWF *swf_ = nullptr;
    const ShapeOriginal *shape_ = nullptr;
    const ShapeOriginal *icon_ = nullptr;
    const ShapeOriginal *badge_ = nullptr;
    std::string asset_;
    std::string label_;
    int labelAlign_ = 0;
    Rect slot_;
    bool hasSlot_ = false;
    float minX_ = 0;
    float minY_ = 0;
    float maxX_ = 0;
    float maxY_ = 0;
    float iminX_ = 0;
    float iminY_ = 0;
    float imaxX_ = 0;
    float imaxY_ = 0;
    float bminX_ = 0;
    float bminY_ = 0;
    float bmaxX_ = 0;
    float bmaxY_ = 0;
};
