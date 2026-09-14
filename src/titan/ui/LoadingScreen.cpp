#pragma once
#include "MovieClip.cpp"
#include "../data/Localization.cpp"
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

class LoadingScreen {
public:
    enum class Stage {
        Boot,
        Logo,
        Loading
    };
    bool loadTexts(const std::string &assetsDir) {
        assetsDir_ = assetsDir;
        textsOk_ = texts_.load(assetsDir);
        clip_.setStatusText(statusText());
        return textsOk_;
    }
    bool loadClip(const std::string &scPath) {
        clip_.setAssetDir(assetsDir_);
        clipOk_ = clip_.load(scPath);
        clip_.setStatusText(statusText());
        return clipOk_;
    }
    bool loaded() const {
        return clipOk_;
    }
    void setProgress(float p) {
        clip_.setProgress(p);
        clip_.setStatusText(statusText());
    }
    void setConnecting() {
        connecting_ = true;
        clip_.setStatusText(statusText());
    }
    bool connecting() const {
        return connecting_;
    }
    std::string text(const std::string &tid, const std::string &fallback) const {
        return texts_.text(tid, fallback);
    }
    const Localization &localization() const {
        return texts_;
    }
    float progress() const {
        return clip_.progress();
    }
    void setStage(Stage stage) {
        stage_ = stage;
    }
    Stage stage() const {
        return stage_;
    }
    void showLogo() {
        stage_ = Stage::Logo;
        logoFrame_ = 0;
        clearStatusOverride();
    }
    void setLogoFrame(int fi) {
        logoFrame_ = fi;
    }
    int logoFrame() const {
        return logoFrame_;
    }
    int logoFrames() const {
        return clip_.logoFrames();
    }
    void showLoading() {
        stage_ = Stage::Loading;
        connecting_ = false;
        clearStatusOverride();
        clip_.setStatusText(statusText());
    }
    void setStatusOverride(const std::string &text) {        override_ = text;
        hasOverride_ = true;
        clip_.setStatusText(statusText());
    }
    void clearStatusOverride() {
        hasOverride_ = false;
        override_.clear();
        clip_.setStatusText(statusText());
    }
    bool hasStatusOverride() const {
        return hasOverride_;
    }
    void drawLabel(std::vector<std::uint32_t> &frame, int w, int h, const std::string &labelText,
                   float boxX, float boxY, float boxW, float boxH,
                   std::uint32_t color, bool outline, std::uint32_t outlineColor) {
        clip_.drawLabel(frame, w, h, labelText, boxX, boxY, boxW, boxH, color, outline, outlineColor);
    }
    void drawLabelLeft(std::vector<std::uint32_t> &frame, int w, int h, const std::string &labelText,
                       float boxX, float boxY, float boxW, float boxH, float padX,
                       std::uint32_t color, bool outline, std::uint32_t outlineColor) {
        clip_.drawLabelLeft(frame, w, h, labelText, boxX, boxY, boxW, boxH, padX, color, outline, outlineColor);
    }
    int measureLabel(const std::string &text, float boxH) const {
        return clip_.measureLabel(text, boxH);
    }
    void renderCentered(std::vector<std::uint32_t> &frame, int w, int h, int clipId, float s) const {
        clip_.renderCentered(frame, w, h, clipId, s);
    }
    void setFieldText(int fieldId, const std::string &text) {
        clip_.setFieldText(fieldId, text);
    }
    void clearFieldTexts() {
        clip_.clearFieldTexts();
    }
    void setLiveField(int fieldId) {
        clip_.setLiveField(fieldId);
    }
    void setLiveText(const std::string &text) {
        clip_.setLiveText(text);
    }
    void setLiveCaret(bool on) {
        clip_.setLiveCaret(on);
    }
    void clearLive() {
        clip_.clearLive();
    }
    void setClipFrame(int clipId, int frame) {
        clip_.setClipFrame(clipId, frame);
    }
    void clearClipFrames() {
        clip_.clearClipFrames();
    }
    int measureLiveWidth() {
        return clip_.measureLiveWidth();
    }
    int liveFieldWidth() {
        return clip_.liveFieldWidth();
    }
    void update(float dt) {
        time_ += dt;
    }
    std::string statusText() const {
        if (hasOverride_) {
            return override_;
        }
        if (connecting_) {
            return texts_.text("TID_CONNECTING_TO_SERVER", "Connecting to server...");
        }
        char buf[16];
        std::snprintf(buf, sizeof(buf), "%d%%", static_cast<int>(clip_.progress() * 100.0f));
        return std::string(buf);
    }
    void draw(std::vector<std::uint32_t> &frame, int w, int h) const {
        if (stage_ == Stage::Logo && clip_.hasLogo()) {
            clip_.blitLogo(frame, w, h, logoFrame_);
            return;
        }
        if (stage_ != Stage::Boot && clip_.loaded()) {
            clip_.blit(frame, w, h);
            return;
        }
        for (std::size_t i = 0; i < frame.size(); i++) {
            frame[i] = 0xFF000000u;
        }
    }
private:
    MovieClip clip_;
    int logoFrame_ = 0;
    bool connecting_ = false;
    bool hasOverride_ = false;
    std::string override_;
    Localization texts_;
    std::string assetsDir_;
    bool textsOk_ = false;
    bool clipOk_ = false;
    float time_ = 0;
    Stage stage_ = Stage::Boot;
};
