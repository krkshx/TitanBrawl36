#pragma once
#include "NativeFont.cpp"
#ifdef TITAN_HAS_FREETYPE
#include "FontEngine.cpp"
#else
#include "BitmapFont.cpp"
#endif
#include "SupercellSWF.cpp"
#include <cmath>
#include <cstdint>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

class ClipRenderer {
public:
    void bind(SupercellSWF *swf) {
        swf_ = swf;
        boxCache_.clear();
        hiddenClips_.clear();
        rebuildIndex();
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
    void renderLogo(std::vector<std::uint32_t> &frame, int w, int h, int rootId, int frameIndex) {
        if (!setupStage(frame, w, h, true)) {
            return;
        }
        renderScale_ = scale_;
        const MovieClipOriginal *mc = findClip(rootId);
        if (!mc || mc->frames.empty()) {
            return;
        }
        // Ориг крутит таймлайн sc_intro от 0 до 99, а не статичный средний кадр.
        int fi = frameIndex;
        if (fi < 0) {
            fi = 0;
        }
        if (fi >= static_cast<int>(mc->frames.size())) {
            fi = static_cast<int>(mc->frames.size()) - 1;
        }
        Matrix2x3 base;
        base.setIdentity();
        drawLogoFrame(*mc, fi, base, nullptr, frame, w, h);
    }
    int logoFrames(int rootId) const {
        const MovieClipOriginal *mc = findClip(rootId);
        if (!mc) {
            return 0;
        }
        return static_cast<int>(mc->frames.size());
    }
    void render(std::vector<std::uint32_t> &frame, int w, int h, int rootId) {
        scaleInnerId_ = -1;
        if (!setupStage(frame, w, h, true)) {
            return;
        }
        renderScale_ = scale_;
        const MovieClipOriginal *root = findClip(rootId);
        if (!root) {
            return;
        }
        Matrix2x3 base;
        base.setIdentity();
        drawClip(*root, 0, base, nullptr, frame, w, h);
    }
    // Рендер поверх готового кадра (без заливки): для набора HUD-клипов меню.
    void renderOnto(std::vector<std::uint32_t> &frame, int w, int h, int rootId) {
        scaleInnerId_ = -1;
        if (!setupStage(frame, w, h, false)) {
            return;
        }
        renderScale_ = scale_;
        const MovieClipOriginal *root = findClip(rootId);
        if (!root) {
            return;
        }
        renderScale_ = scale_;
        Matrix2x3 base;
        base.setIdentity();
        drawClip(*root, 0, base, nullptr, frame, w, h);
    }
    // Рендер с явной базовой матрицей (якорение HUD как в ориге).
    // Stage-маппинг тождественный: всё несёт base.
    // renderScale_ берём из базы (равномерный масштаб a~=d): размер шрифта
    // TextField (f.size — единицы клипа) обязан умножаться на него, иначе
    // текст в лобби рисуется в 1/0.66 раз крупнее и «съезжает» из полей.
    void renderWithBase(std::vector<std::uint32_t> &frame, int w, int h, int rootId,
                        const Matrix2x3 &base) {
        scaleInnerId_ = -1;
        if (!swf_ || frame.empty()) {
            return;
        }
        scale_ = 1;
        ox_ = 0;
        oy_ = 0;
        float bs = base.a >= 0 ? base.a : -base.a;
        float bs2 = base.d >= 0 ? base.d : -base.d;
        renderScale_ = (bs + bs2) * 0.5f;
        if (renderScale_ <= 0) {
            renderScale_ = 1;
        }
        const MovieClipOriginal *root = findClip(rootId);
        if (!root) {
            return;
        }
        drawClip(*root, 0, base, nullptr, frame, w, h);
    }
    // Границы клипа в его локальных единицах (кадр 0), с рекурсией во
    // вложенные клипы: для докинга HUD-контейнеров к краям экрана.
    // boundsOf считает только прямые шейпы — здесь спускаемся глубже.
    bool measureClip(int rootId, float &minX, float &minY, float &maxX, float &maxY) const {
        const MovieClipOriginal *root = findClip(rootId);
        if (!root) {
            return false;
        }
        Matrix2x3 ident;
        ident.setIdentity();
        bool any = false;
        boundsOfDeep(*root, 0, ident, minX, minY, maxX, maxY, any, 0);
        return any && maxX > minX && maxY > minY;
    }
    void boundsOfDeep(const MovieClipOriginal &clip, int fi, const Matrix2x3 &parent,
                      float &minX, float &minY, float &maxX, float &maxY, bool &any, int depth) const {
        if (depth > 6 || clip.frames.empty()) {
            return;
        }
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
            Matrix2x3 local = elementMatrix(el.matrix, clip.bankIndex);
            Matrix2x3 world = local;
            world.multiply(parent);
            if (const ShapeOriginal *sh = findShape(ch.id)) {
                for (std::size_t ci = 0; ci < sh->commands.size(); ci++) {
                    const ShapeOriginal::Command &c = sh->commands[ci];
                    for (std::size_t vi = 0; vi < c.x.size(); vi++) {
                        float x = world.applyX(c.x[vi], c.y[vi]);
                        float y = world.applyY(c.x[vi], c.y[vi]);
                        if (!any) {
                            minX = maxX = x;
                            minY = maxY = y;
                            any = true;
                        } else {
                            if (x < minX) minX = x;
                            if (x > maxX) maxX = x;
                            if (y < minY) minY = y;
                            if (y > maxY) maxY = y;
                        }
                    }
                }
            } else if (const MovieClipOriginal *sub = findClip(ch.id)) {
                boundsOfDeep(*sub, 0, world, minX, minY, maxX, maxY, any, depth + 1);
            } else if (const TextFieldOriginal *f = findField(ch.id)) {
                float cx0 = world.applyX(static_cast<float>(f->left), static_cast<float>(f->top));
                float cy0 = world.applyY(static_cast<float>(f->left), static_cast<float>(f->top));
                float cx1 = world.applyX(static_cast<float>(f->right), static_cast<float>(f->bottom));
                float cy1 = world.applyY(static_cast<float>(f->right), static_cast<float>(f->bottom));
                float loX = cx0 < cx1 ? cx0 : cx1;
                float hiX = cx0 < cx1 ? cx1 : cx0;
                float loY = cy0 < cy1 ? cy0 : cy1;
                float hiY = cy0 < cy1 ? cy1 : cy0;
                if (!any) {
                    minX = loX;
                    maxX = hiX;
                    minY = loY;
                    maxY = hiY;
                    any = true;
                } else {
                    if (loX < minX) minX = loX;
                    if (hiX > maxX) maxX = hiX;
                    if (loY < minY) minY = loY;
                    if (hiY > maxY) maxY = hiY;
                }
            }
        }
    }
    float stageScale(int w, int h) const {
        float s = w / 1288.0f;
        float s2 = h / 768.0f;
        if (s2 > s) {
            s = s2;
        }
        return s;
    }
    // Тексты кнопок меню (GameButton::setTextByTID в ориге): parentClipId -> текст.
    // Поля 'txt' внутри этих клипов рисуются заданным текстом в их же стиле.
    void setButtonText(int parentClipId, const std::string &text) {
        buttonTexts_[parentClipId] = text;
    }
    void clearButtonTexts() {
        buttonTexts_.clear();
    }
    // Текст именованного поля по id (title/body попапов).
    void setFieldText(int fieldId, const std::string &text) {
        fieldTexts_[fieldId] = text;
    }
    void clearFieldTexts() {
        fieldTexts_.clear();
    }
    // Живое поле ввода (EnterNamePopup::GameInputField): текст + caret.
    void setLiveField(int fieldId) {
        liveFieldId_ = fieldId;
    }
    void setLiveText(const std::string &text) {
        liveText_ = text;
    }
    void setLiveCaret(bool on) {
        liveCaret_ = on;
    }
    void clearLive() {
        liveFieldId_ = -1;
        liveText_.clear();
        liveCaret_ = false;
    }
    // Кадр клипа (состояния кнопок: button_continue f0/f1).
    void setClipFrame(int clipId, int frame) {
        clipFrames_[clipId] = frame;
    }
    void clearClipFrames() {
        clipFrames_.clear();
    }
    // Скрытые клипы (игра гасит ивент-кнопки по состоянию событий:
    // HomePage прячет неактивные event-кнопки через visible=false).
    // Офлайн-дефолт задаёт MenuUi (потом — по парсу событий home).
    void setHiddenClip(int clipId, bool hidden) {
        if (hidden) {
            hiddenClips_[clipId] = true;
        } else {
            hiddenClips_.erase(clipId);
        }
    }
    void clearHidden() {
        hiddenClips_.clear();
    }
    bool isHidden(int clipId) const {
        return hiddenClips_.find(clipId) != hiddenClips_.end();
    }
    // Рендер с явной базой + центрирование попапа (EnterNamePopup).
    void renderCentered(std::vector<std::uint32_t> &frame, int w, int h, int rootId, float s) {
        scaleInnerId_ = -1;
        if (!swf_ || frame.empty()) {
            return;
        }
        scale_ = 1;
        ox_ = 0;
        oy_ = 0;
        const MovieClipOriginal *root = findClip(rootId);
        if (!root) {
            return;
        }
        Matrix2x3 base;
        base.setIdentity();
        base.a = s;
        base.d = s;
        base.x = w * 0.5f;
        base.y = h * 0.5f;
        renderScale_ = s;
        drawClip(*root, 0, base, nullptr, frame, w, h);
    }
    int measureLiveWidth() {
#ifdef TITAN_HAS_FREETYPE
        if (liveFieldId_ < 0 || liveText_.empty()) {
            return 0;
        }
        const TextFieldOriginal *f = findField(liveFieldId_);
        if (!f) {
            return 0;
        }
        int px = static_cast<int>(f->size * renderScale_);
        if (px < 1) {
            px = 1;
        }
        return fonts_.measure(liveText_, px);
#else
        return 0;
#endif
    }
    // Ширина поля ввода в пикселях (для checkMaxPixelWidth).
    int liveFieldWidth() {
        if (liveFieldId_ < 0) {
            return 0;
        }
        const TextFieldOriginal *f = findField(liveFieldId_);
        if (!f) {
            return 0;
        }
        return static_cast<int>((f->right - f->left) * renderScale_);
    }
    // Фон как в ориге (HomeScreen::enter): cover по границам ВНУТРЕННЕГО
    // клипа (getMovieClipByName, обычно 'bg_colour'), scale вешается на него,
    // корень ставится в центр. Иконки-частицы при этом не масштабируются.
    void renderCover(std::vector<std::uint32_t> &frame, int w, int h, int rootId,
                     const std::string &innerName) {
        scaleInnerId_ = -1;
        scaleInner_ = 1;
        if (!swf_ || frame.empty()) {
            return;
        }
        for (std::size_t i = 0; i < frame.size(); i++) {
            frame[i] = 0xFF000000u;
        }
        const MovieClipOriginal *root = findClip(rootId);
        if (!root) {
            return;
        }
        const MovieClipOriginal *inner = findChildByName(*root, innerName, 0);
        if (!inner) {
            inner = root;
        }
        float minX = 0, minY = 0, maxX = 0, maxY = 0;
        {
            Matrix2x3 ident;
            ident.setIdentity();
            bool any = false;
            boundsOf(*inner, 0, ident, minX, minY, maxX, maxY, any);
            if (!any || maxX <= minX || maxY <= minY) {
                render(frame, w, h, rootId);
                return;
            }
        }
        float bw = maxX - minX;
        float bh = maxY - minY;
        float s = w / bw;
        float s2 = h / bh;
        if (s2 > s) {
            s = s2;
        }
        scaleInnerId_ = inner->id;
        scaleInner_ = s;
        // Важно: базовая матрица уже несёт translate+scale, поэтому
        // stage-маппинг (toX/toY) — тождественный, иначе двойной трансформ.
        scale_ = 1;
        ox_ = 0;
        oy_ = 0;
        renderScale_ = s;
        Matrix2x3 base;
        base.setIdentity();
        base.x = w * 0.5f;
        base.y = h * 0.5f;
        drawClip(*root, 0, base, nullptr, frame, w, h);
        scaleInnerId_ = -1;
    }
    const MovieClipOriginal *findChildByName(const MovieClipOriginal &clip, const std::string &name, int depth) const {
        if (depth > 4 || name.empty()) {
            return nullptr;
        }
        for (std::size_t i = 0; i < clip.children.size(); i++) {
            if (clip.children[i].name == name) {
                if (const MovieClipOriginal *c = findClip(clip.children[i].id)) {
                    return c;
                }
            }
        }
        for (std::size_t i = 0; i < clip.children.size(); i++) {
            if (const MovieClipOriginal *sub = findClip(clip.children[i].id)) {
                if (const MovieClipOriginal *f = findChildByName(*sub, name, depth + 1)) {
                    return f;
                }
            }
        }
        return nullptr;
    }
    // Границы одного клипа (кадр fi) в его локальных единицах.
    void boundsOf(const MovieClipOriginal &clip, int fi, const Matrix2x3 &parent,
                  float &minX, float &minY, float &maxX, float &maxY, bool &any) const {
        if (clip.frames.empty()) {
            return;
        }
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
            Matrix2x3 local = elementMatrix(el.matrix, clip.bankIndex);
            Matrix2x3 world = local;
            world.multiply(parent);
            if (const ShapeOriginal *sh = findShape(ch.id)) {
                for (std::size_t ci = 0; ci < sh->commands.size(); ci++) {
                    const ShapeOriginal::Command &c = sh->commands[ci];
                    for (std::size_t vi = 0; vi < c.x.size(); vi++) {
                        float x = world.applyX(c.x[vi], c.y[vi]);
                        float y = world.applyY(c.x[vi], c.y[vi]);
                        if (!any) {
                            minX = maxX = x;
                            minY = maxY = y;
                            any = true;
                        } else {
                            if (x < minX) minX = x;
                            if (x > maxX) maxX = x;
                            if (y < minY) minY = y;
                            if (y > maxY) maxY = y;
                        }
                    }
                }
            }
        }
    }
    // Подпись кнопки как в ориге (GameButton::setText, TencentLoginState::enter):
    // центрированный текст игровым шрифтом с обводкой, ужатый под прямоугольник
    // (аналог MovieClipHelper::autoAdjustText).
    void drawLabel(std::vector<std::uint32_t> &frame, int w, int h, const std::string &labelText,
                   float boxX, float boxY, float boxW, float boxH,
                   std::uint32_t color, bool outline, std::uint32_t outlineColor) {
        if (labelText.empty() || boxW < 4 || boxH < 4) {
            return;
        }
        renderScale_ = scale_;
#ifdef TITAN_HAS_FREETYPE
        int px = static_cast<int>(boxH * 0.55f);
        if (px < 1) {
            px = 1;
        }
        fonts_.drawCentered(frame, w, h, labelText, boxX, boxY, boxW, boxH, px, color, outline, outlineColor);
#else
        int textW = BitmapFont::measure(labelText);
        if (textW <= 0) {
            return;
        }
        int sc = static_cast<int>(boxW / textW);
        int scH = static_cast<int>(boxH / 7);
        if (scH < sc) {
            sc = scH;
        }
        if (sc < 1) {
            sc = 1;
        }
        int sw = textW * sc;
        int sh = 7 * sc;
        int dx = static_cast<int>(boxX + (boxW - sw) * 0.5f);
        int dy = static_cast<int>(boxY + (boxH - sh) * 0.5f);
        if (outline) {
            BitmapFont::drawText(frame, w, h, labelText, dx - sc, dy, sc, outlineColor);
            BitmapFont::drawText(frame, w, h, labelText, dx + sc, dy, sc, outlineColor);
            BitmapFont::drawText(frame, w, h, labelText, dx, dy - sc, sc, outlineColor);
            BitmapFont::drawText(frame, w, h, labelText, dx, dy + sc, sc, outlineColor);
        }
        BitmapFont::drawText(frame, w, h, labelText, dx, dy, sc, color);
#endif
    }
    // Поле ввода (экран ника в ориге): текст от левого края с отступом,
    // ужатый по ширине; caret рисует вызывающий по measure().
    void drawLabelLeft(std::vector<std::uint32_t> &frame, int w, int h, const std::string &labelText,
                       float boxX, float boxY, float boxW, float boxH, float padX,
                       std::uint32_t color, bool outline, std::uint32_t outlineColor) {
        if (boxW < 4 || boxH < 4) {
            return;
        }
        renderScale_ = scale_;
#ifdef TITAN_HAS_FREETYPE
        int px = static_cast<int>(boxH * 0.55f);
        if (px < 1) {
            px = 1;
        }
        while (px > 1 && fonts_.measure(labelText, px) > static_cast<int>(boxW - padX * 2)) {
            px--;
        }
        fonts_.drawLeft(frame, w, h, labelText, boxX + padX, boxY, boxW - padX * 2, boxH, px, color, outline, outlineColor);
#else
        int textW = BitmapFont::measure(labelText);
        if (textW <= 0) {
            return;
        }
        int sc = static_cast<int>((boxW - padX * 2) / textW);
        int scH = static_cast<int>(boxH / 7);
        if (scH < sc) {
            sc = scH;
        }
        if (sc < 1) {
            sc = 1;
        }
        int sh = 7 * sc;
        int dx = static_cast<int>(boxX + padX);
        int dy = static_cast<int>(boxY + (boxH - sh) * 0.5f);
        if (outline) {
            BitmapFont::drawText(frame, w, h, labelText, dx - sc, dy, sc, outlineColor);
            BitmapFont::drawText(frame, w, h, labelText, dx + sc, dy, sc, outlineColor);
            BitmapFont::drawText(frame, w, h, labelText, dx, dy - sc, sc, outlineColor);
            BitmapFont::drawText(frame, w, h, labelText, dx, dy + sc, sc, outlineColor);
        }
        BitmapFont::drawText(frame, w, h, labelText, dx, dy, sc, color);
#endif
    }
    int measureLabel(const std::string &text, float boxH) const {
#ifdef TITAN_HAS_FREETYPE
        int px = static_cast<int>(boxH * 0.55f);
        if (px < 1) {
            px = 1;
        }
        return fonts_.measure(text, px);
#else
        (void)text;
        (void)boxH;
        return 0;
#endif
    }
private:
    bool setupStage(std::vector<std::uint32_t> &frame, int w, int h, bool clear) {
        if (!swf_ || frame.empty()) {
            return false;
        }
        if (clear) {
            for (std::size_t i = 0; i < frame.size(); i++) {
                frame[i] = 0xFF000000u;
            }
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
        auto it = clipIndex_.find(id);
        if (it != clipIndex_.end()) {
            return it->second;
        }
        return nullptr;
    }
    const ShapeOriginal *findShape(int id) const {
        auto it = shapeIndex_.find(id);
        if (it != shapeIndex_.end()) {
            return it->second;
        }
        return nullptr;
    }
    const TextFieldOriginal *findField(int id) const {
        auto it = fieldIndex_.find(id);
        if (it != fieldIndex_.end()) {
            return it->second;
        }
        return nullptr;
    }
    void rebuildIndex() {
        clipIndex_.clear();
        shapeIndex_.clear();
        fieldIndex_.clear();
        if (!swf_) {
            return;
        }
        for (std::size_t i = 0; i < swf_->clips.size(); i++) {
            clipIndex_[swf_->clips[i].id] = &swf_->clips[i];
        }
        for (std::size_t i = 0; i < swf_->shapes.size(); i++) {
            shapeIndex_[swf_->shapes[i].id] = &swf_->shapes[i];
        }
        for (std::size_t i = 0; i < swf_->fields.size(); i++) {
            fieldIndex_[swf_->fields[i].id] = &swf_->fields[i];
        }
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
    void drawLogoFrame(const MovieClipOriginal &clip, int frameIndex, const Matrix2x3 &parent, const ColorTransform *parentCt, std::vector<std::uint32_t> &frame, int w, int h) {        if (clip.frames.empty()) {
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
            Matrix2x3 local = elementMatrix(el.matrix, clip.bankIndex);
            Matrix2x3 world = local;
            world.multiply(parent);
            ColorTransform eff;
            const ColorTransform *ct = parentCt;
            if (el.color != 65535 && el.color >= 0) {
                const ColorTransform *own = swf_->colorAt(clip.bankIndex, el.color);
                if (own) {
                    if (ct) {
                        eff = ColorTransform::combine(*ct, *own);
                    } else {
                        eff = *own;
                    }
                    ct = &eff;
                }
            }
            if (findClip(ch.id)) {
                drawLogoFrame(*findClip(ch.id), 0, world, ct, frame, w, h);
            } else if (findShape(ch.id)) {
                drawShape(*findShape(ch.id), local, parent, ct, frame, w, h);
            }
        }
    }
public:
    // Оживление таймлайнов (MovieClipPlayer в либе): по времени крутятся
    // ТОЛЬКО безлейбловые клипы (настоящие лупы: частицы, спиннеры).
    // Клипы с лейблами кадров (button idle/ready, hamburger appear/
    // disappear) — стейт-машины: ориг ставит их через gotoAndStop(label),
    // дефолт — кадр 0. Крутить их по wall-clock = мигающие кнопки.
    void setAnimate(bool on, std::int64_t ms) {
        animate_ = on;
        animMs_ = ms;
    }
    static bool hasLabeledFrame(const MovieClipOriginal &clip) {
        for (std::size_t i = 0; i < clip.frames.size(); i++) {
            if (clip.frames[i].hasLabel && !clip.frames[i].label.empty()) {
                return true;
            }
        }
        return false;
    }
    void drawClip(const MovieClipOriginal &clip, int frameIndex, const Matrix2x3 &parent, const ColorTransform *parentCt, std::vector<std::uint32_t> &frame, int w, int h) {
        if (clip.frames.empty()) {
            return;
        }
        int fi = frameIndex;
        auto cfit = clipFrames_.find(clip.id);
        if (cfit != clipFrames_.end()) {
            fi = cfit->second;
        } else if (animate_ && clip.frames.size() > 1 && !hasLabeledFrame(clip)) {
            int fps = clip.fps > 0 ? clip.fps : 30;
            fi = static_cast<int>((animMs_ * fps / 1000) % static_cast<int>(clip.frames.size()));
        }
        if (fi < 0) {
            fi = 0;
        }
        if (fi >= static_cast<int>(clip.frames.size())) {
            fi = static_cast<int>(clip.frames.size()) - 1;
        }
        const MovieClipOriginal::Frame &fr = clip.frames[static_cast<std::size_t>(fi)];
        // Стек вариантов одного слота (pro_league/championship/ranked на
        // одной матрице): игра кажет один, остальным ставит visible=false
        // по состоянию событий. Статично рисуем первый (см. isStackedVariant).
        std::vector<SeenSlot> seenSlot_;
        seenSlot_.reserve(fr.elements.size());
        for (std::size_t k = 0; k < fr.elements.size(); k++) {
            const MovieClipOriginal::Element &el = fr.elements[k];
            if (el.child < 0 || el.child >= static_cast<int>(clip.children.size())) {
                continue;
            }
            const MovieClipOriginal::Child &ch = clip.children[static_cast<std::size_t>(el.child)];
            if (!ch.name.empty() && skipClip(ch.name)) {
                continue;
            }
            if (isHidden(ch.id)) {
                continue;
            }
            Matrix2x3 localEarly = elementMatrix(el.matrix, clip.bankIndex);
            if (el.matrix != 65535 &&
                isStackedVariant(seenSlot_, el.matrix, ch.id, localEarly.x, localEarly.y)) {
                continue;
            }
            if (el.matrix != 65535) {
                SeenSlot ss;
                ss.matrix = el.matrix;
                ss.childId = ch.id;
                ss.tx = localEarly.x;
                ss.ty = localEarly.y;
                seenSlot_.push_back(ss);
            }
            Matrix2x3 local = elementMatrix(el.matrix, clip.bankIndex);
            // Cover фон: внутренний клип масштабируется (setScale в ориге).
            if (ch.id == scaleInnerId_ && scaleInner_ > 0) {
                local.a *= scaleInner_;
                local.b *= scaleInner_;
                local.c *= scaleInner_;
                local.d *= scaleInner_;
                local.x *= scaleInner_;
                local.y *= scaleInner_;
            }
            Matrix2x3 world = local;
            world.multiply(parent);
            ColorTransform eff;
            const ColorTransform *ct = parentCt;
            if (el.color != 65535 && el.color >= 0) {
                const ColorTransform *own = swf_->colorAt(clip.bankIndex, el.color);
                if (own) {
                    if (ct) {
                        eff = ColorTransform::combine(*ct, *own);
                    } else {
                        eff = *own;
                    }
                    ct = &eff;
                }
            }
            if (findClip(ch.id)) {
                if (ch.name == "loading_bar") {
                    drawLoadingBar(*findClip(ch.id), world, ct, frame, w, h);
                } else if (ch.name == "progress_bar") {
                    drawClip(*findClip(ch.id), progressFrame(*findClip(ch.id)), world, ct, frame, w, h);
                } else {
                    drawClip(*findClip(ch.id), 0, world, ct, frame, w, h);
                }
            } else if (findShape(ch.id)) {
                drawShape(*findShape(ch.id), local, parent, ct, frame, w, h);
            } else if (findField(ch.id)) {
                if (ch.name == statusName_) {
                    drawField(*findField(ch.id), world, statusText_, frame, w, h);
                } else if (ch.id == liveFieldId_) {
                    drawLiveField(*findField(ch.id), world, frame, w, h);
                } else if (ch.name == "txt") {
                    auto bit = buttonTexts_.find(clip.id);
                    if (bit != buttonTexts_.end() && !bit->second.empty()) {
                        drawField(*findField(ch.id), world, bit->second, frame, w, h);
                    }
                } else {
                    auto fit = fieldTexts_.find(ch.id);
                    if (fit != fieldTexts_.end() && !fit->second.empty()) {
                        drawField(*findField(ch.id), world, fit->second, frame, w, h);
                    }
                }
            }
        }
    }
    void drawLoadingBar(const MovieClipOriginal &bar, const Matrix2x3 &parent, const ColorTransform *parentCt, std::vector<std::uint32_t> &frame, int w, int h) {
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
            Matrix2x3 world = elementMatrix(el.matrix, bar.bankIndex);
            world.multiply(parent);
            if (findField(ch.id)) {
                // Единственный текст загрузки — оригинальное поле 'text' (id 24):
                // до 50% показывает N%, после — TID_CONNECTING_TO_SERVER.
                // Своего второго текста поверх бара не рисуем (как в ориге).
                if (ch.name == statusName_) {
                    drawField(*findField(ch.id), world, statusText_, frame, w, h);
                }
            } else if (findClip(ch.id) && ch.name == "progress_bar") {
                drawClip(*findClip(ch.id), progressFrame(*findClip(ch.id)), world, parentCt, frame, w, h);
            } else if (findClip(ch.id)) {
                drawClip(*findClip(ch.id), 0, world, parentCt, frame, w, h);
            }
        }
    }

    Matrix2x3 elementMatrix(int el, int bank) const {
        Matrix2x3 local;
        local.setIdentity();
        if (el != 65535 && el >= 0) {
            local = *swf_->matrixAt(bank, el);
        }
        return local;
    }
    // У клипа есть видимый текст: поле 'txt' с заданным текстом кнопки
    // или именованное поле из fieldTexts_. Рекурсивно (текст PLAY лежит
    // в brawl_button внутри brawl_container). Глубина 3 — хватает.
    bool clipHasText(int clipId) const {
        return clipHasTextDeep(clipId, 0);
    }
    bool clipHasTextDeep(int clipId, int depth) const {
        const MovieClipOriginal *c = findClip(clipId);
        if (!c || c->frames.empty() || depth > 3) {
            return false;
        }
        const MovieClipOriginal::Frame &fr = c->frames[0];
        for (std::size_t k = 0; k < fr.elements.size(); k++) {
            const MovieClipOriginal::Element &el = fr.elements[k];
            if (el.child < 0 || el.child >= static_cast<int>(c->children.size())) {
                continue;
            }
            const MovieClipOriginal::Child &ch = c->children[static_cast<std::size_t>(el.child)];
            if (ch.name == "txt") {
                auto bit = buttonTexts_.find(c->id);
                if (bit != buttonTexts_.end() && !bit->second.empty()) {
                    return true;
                }
            } else if (findField(ch.id)) {
                auto fit = fieldTexts_.find(ch.id);
                if (fit != fieldTexts_.end() && !fit->second.empty()) {
                    return true;
                }
            } else if (findClip(ch.id)) {
                if (clipHasTextDeep(ch.id, depth + 1)) {
                    return true;
                }
            }
        }
        return false;
    }
    struct ChildBox {
        bool ok = false;
        float w = 0;
        float h = 0;
    };
    ChildBox childBox(int id) const {
        auto it = boxCache_.find(id);
        if (it != boxCache_.end()) {
            return it->second;
        }
        ChildBox b;
        float x0 = 0, y0 = 0, x1 = 0, y1 = 0;
        if (measureClip(id, x0, y0, x1, y1)) {
            b.ok = true;
            b.w = x1 - x0;
            b.h = y1 - y0;
        }
        boxCache_[id] = b;
        return b;
    }
    // Стек вариантов одного слота: игра кажет один, остальным ставит
    // visible=false по состоянию событий.
    // Слот кадра: матрица + локальная позиция + ребёнок.
    struct SeenSlot {
        int matrix = 0;
        int childId = 0;
        float tx = 0;
        float ty = 0;
    };
    // 1) ТОЧНАЯ матрица + разный id = варианты всегда (pro_league/
    //    championship/ranked на 60756, raid/warning на 60753). Зеркала
    //    безопасны: у них один id на всех повторах (ph_* = 10439).
    // 2) БЛИЗКАЯ позиция (±8) + разный id = вариант, только если у
    //    уже нарисованного есть текст, а у нового нет (brawl с PLAY
    //    против пустого spectate). Декор-стеки без текста (бар+тикет)
    //    рисуются оба — как в ориге.
    // Одинаковый id дважды — намеренный дабл-дро (свечение).
    // Identity-матрицы (65535) исключены вызывающим.
    bool isStackedVariant(const std::vector<SeenSlot> &seen, int matrix, int childId,
                          float tx, float ty) const {
        for (std::size_t i = 0; i < seen.size(); i++) {
            const SeenSlot &s = seen[i];
            if (s.childId == childId) {
                continue;
            }
            if (s.matrix == matrix) {
                return true;
            }
            float dx = s.tx > tx ? s.tx - tx : tx - s.tx;
            float dy = s.ty > ty ? s.ty - ty : ty - s.ty;
            if (dx >= 8.0f || dy >= 8.0f) {
                continue;
            }
            if (clipHasText(s.childId) && !clipHasText(childId)) {
                return true;
            }
        }
        return false;
    }
    static bool useNativeFont(const TextFieldOriginal &f) {
        if (f.deviceFont) {
            return true;
        }
        return NativeFont::isSystemFamily(f.font);
    }
    // Горизонтальный align поля либы (TextField::get_horizontal_align,
    // SupercellFlash): bit0=Right, bit1=Center, bit2=Justify, иначе Left.
    // В ui.sc: 18=center, 16=left, 2=center. Раньше всё рисовалось
    // по центру — подписи уезжали («кривые»). Вертикаль в лобби
    // однострочная — оставляем центрирование по Y как в ориге.
    // Возврат: 0=left, 1=center, 2=right.
    static int alignH(const TextFieldOriginal &f) {
        if (f.align & 1) {
            return 2;
        }
        if (f.align & 2) {
            return 1;
        }
        if (f.align & 4) {
            return 1;
        }
        return 0;
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
    // Ребро границы полигона в пикселях экрана (для AA только по контуру).
    struct PolyEdge {
        float ax = 0;
        float ay = 0;
        float dx = 0;
        float dy = 0;
        float invLen = 0;
    };
    static bool mapSlice(const float *grid, std::size_t n, float scale, float trans, float *mapped) {
        if (n < 2 || n > 16) {
            return false;
        }
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
        // 9-slice здесь НЕ применяем: scaling grid либы (сабтег 31)
        // принадлежит MovieClip, а не Shape. Старый код тянул
        // эвристический shape.nineSlice при любом осевом stage-масштабе
        // и волнил весь HUD. Неравномерную растяжку в слот делает
        // UiButton::draw по своей сетке; Cover-фон масштабируется
        // целиком через scaleInner_. Поэтому всегда прямой проход.
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
            if (n < 3 || n > 256 || c.u.size() < n || c.v.size() < n) {
                continue;
            }
            // Вершины в пикселях экрана один раз (не на треугольник).
            float sx[256];
            float sy[256];
            for (std::size_t vi = 0; vi < n; vi++) {
                sx[vi] = toX(world.applyX(c.x[vi], c.y[vi]));
                sy[vi] = toY(world.applyY(c.x[vi], c.y[vi]));
            }
            // Граница полигона для AA (см. drawTri): только контур,
            // диагонали фэна — не рёбра, иначе швы на каждой спице.
            PolyEdge edges[256];
            std::size_t ne = 0;
            for (std::size_t vi = 0; vi < n; vi++) {
                std::size_t vj = (vi + 1) % n;
                float dx = sx[vj] - sx[vi];
                float dy = sy[vj] - sy[vi];
                float len = static_cast<float>(std::sqrt(dx * dx + dy * dy));
                if (len < 0.0001f) {
                    continue;
                }
                edges[ne].ax = sx[vi];
                edges[ne].ay = sy[vi];
                edges[ne].dx = dx;
                edges[ne].dy = dy;
                edges[ne].invLen = 1.0f / len;
                ne++;
            }
            for (std::size_t i = 1; i + 1 < n; i++) {
                drawTri(tex, c, 0, i, i + 1, sx, sy, edges, ne, ct, frame, w, h);
            }
        }
    }
    void drawTri(const SWFTexture &tex, const ShapeOriginal::Command &c, std::size_t i0, std::size_t i1, std::size_t i2, const float *sx, const float *sy, const PolyEdge *edges, std::size_t ne, const ColorTransform *ct, std::vector<std::uint32_t> &frame, int w, int h) {
        float ax = sx[i0];
        float ay = sy[i0];
        float bx = sx[i1];
        float by = sy[i1];
        float cx = sx[i2];
        float cy = sy[i2];
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
        // Покрытие для ровных краёв (AA как у GPU в ориге) — ТОЛЬКО по
        // границе полигона: расстояние до ближайшего ребра контура.
        // Диагонали фэна рёбрами не считаются, иначе тёмные швы на каждой
        // спице (кресты через весь фон и грани на панелях).
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
                if (ct) {
                    src = ct->apply(src);
                }
                unsigned sa = (src >> 24) & 0xFF;
                if (sa == 0) {
                    continue;
                }
                // Непрозрачные тексели без трансформов: швы между соседними
                // квадами (9-slice панели!) нельзя блендить — иначе тёмная
                // сетка по каждому стыку (как MSAA-резолв в ориге: сэмпл
                // либо целиком фигуры, либо фон). Порог 0.5 по покрытию.
                // Полупрозрачное — блендим с покрытием как раньше.
                std::size_t kk = static_cast<std::size_t>(y) * static_cast<std::size_t>(w) + static_cast<std::size_t>(x);
                if (sa == 255 && !ct) {
                    if (cov < 0.5f) {
                        continue;
                    }
                    frame[kk] = 0xFF000000u | (src & 0x00FFFFFFu);
                    continue;
                }
                if (cov < 1.0f) {
                    sa = static_cast<unsigned>(sa * cov);
                    if (sa == 0) {
                        continue;
                    }
                }
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
    // Живое поле ввода ника: текст из GameInputField + caret, стиль поля.
    void drawLiveField(const TextFieldOriginal &f, const Matrix2x3 &m, std::vector<std::uint32_t> &frame, int w, int h) {
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
        if (!liveText_.empty()) {
            drawField(f, m, liveText_, frame, w, h);
        }
        if (!liveCaret_) {
            return;
        }
#ifdef TITAN_HAS_FREETYPE
        int px = static_cast<int>(f.size * renderScale_);
        if (px < 1) {
            px = 1;
        }
        int tw = liveText_.empty() ? 0 : fonts_.measure(liveText_, px);
        int cx = static_cast<int>(x0 + (boxW - tw) * 0.5f + tw);
        int top = static_cast<int>(y0 + boxH * 0.2f);
        int bot = static_cast<int>(y0 + boxH * 0.8f);
        for (int y = top; y < bot; y++) {
            for (int x = cx; x < cx + 2; x++) {
                if (x < 0 || y < 0 || x >= w || y >= h) {
                    continue;
                }
                frame[static_cast<std::size_t>(y) * static_cast<std::size_t>(w) + static_cast<std::size_t>(x)] = 0xFF222222u;
            }
        }
#else
        (void)w;
        (void)h;
#endif
    }
    void drawField(const TextFieldOriginal &f, const Matrix2x3 &m, const std::string &text, std::vector<std::uint32_t> &frame, int w, int h) {        if (text.empty()) {
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
        int px = static_cast<int>(f.size * renderScale_);
        if (px < 1) {
            px = 1;
        }
        std::uint32_t col = static_cast<std::uint32_t>(f.color);
        std::uint32_t ocol = outlineColorFor(f);
        int ah = alignH(f);
        if (useNativeFont(f)) {
            if (ah == 0) {
                nativeFonts_.drawLeft(frame, w, h, text, x0, y0, boxW, boxH, px, col, f.outline, ocol);
            } else if (ah == 2) {
                nativeFonts_.drawRight(frame, w, h, text, x0, y0, boxW, boxH, px, col, f.outline, ocol);
            } else {
                nativeFonts_.drawCentered(frame, w, h, text, x0, y0, boxW, boxH, px, col, f.outline, ocol);
            }
        } else {
            if (ah == 0) {
                fonts_.drawLeft(frame, w, h, text, x0, y0, boxW, boxH, px, col, f.outline, ocol);
            } else if (ah == 2) {
                fonts_.drawRight(frame, w, h, text, x0, y0, boxW, boxH, px, col, f.outline, ocol);
            } else {
                fonts_.drawCentered(frame, w, h, text, x0, y0, boxW, boxH, px, col, f.outline, ocol);
            }
        }
#else
        int tw = BitmapFont::measure(text);
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
            BitmapFont::drawText(frame, w, h, text, dx - sc, dy, sc, ocol);
            BitmapFont::drawText(frame, w, h, text, dx + sc, dy, sc, ocol);
            BitmapFont::drawText(frame, w, h, text, dx, dy - sc, sc, ocol);
            BitmapFont::drawText(frame, w, h, text, dx, dy + sc, sc, ocol);
        }
        BitmapFont::drawText(frame, w, h, text, dx, dy, sc, col);
#endif
    }
    float toX(float dx) const {
        return ox_ + dx * scale_;
    }
    float toY(float dy) const {
        return oy_ + dy * scale_;
    }
    SupercellSWF *swf_ = nullptr;
    // O(1)-индексы либы (id -> объект): горячий цикл без линейных поисков.
    std::unordered_map<int, const MovieClipOriginal *> clipIndex_;
    std::unordered_map<int, const ShapeOriginal *> shapeIndex_;
    std::unordered_map<int, const TextFieldOriginal *> fieldIndex_;
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
    // Cover фон: id внутреннего клипа + его масштаб (см. renderCover).
    int scaleInnerId_ = -1;
    float scaleInner_ = 1;
    // Масштаб последнего рендера для метрик текста.
    float renderScale_ = 1;
    // Тексты кнопок меню по parentClipId (см. setButtonText).
    std::map<int, std::string> buttonTexts_;
    // Тексты именованных полей по fieldId.
    std::map<int, std::string> fieldTexts_;
    // Кадры клипов (состояния кнопок).
    std::map<int, int> clipFrames_;
    // Скрытые клипы (см. setHiddenClip).
    std::map<int, bool> hiddenClips_;
    // Живое поле ввода ника.
    int liveFieldId_ = -1;
    std::string liveText_;
    bool liveCaret_ = false;
    bool animate_ = false;
    std::int64_t animMs_ = 0;
    // Кэш размеров детей для детекта стек-вариантов (чистится в bind).
    mutable std::unordered_map<int, ChildBox> boxCache_;
};
