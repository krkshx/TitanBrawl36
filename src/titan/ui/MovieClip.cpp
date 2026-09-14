#pragma once
#include "DisplayObject.cpp"
#include "../gfx/ClipRenderer.cpp"
#include "../gfx/SupercellSWF.cpp"
#include <cstdint>
#include <string>
#include <vector>

class MovieClip : public DisplayObject {
public:
    bool load(const std::string &scPath) {
        if (!clip_.load(scPath)) {
            return false;
        }
        std::string texPath = scPath;
        std::size_t dot = texPath.rfind('.');
        if (dot != std::string::npos) {
            texPath = texPath.substr(0, dot) + "_tex.sc";
            clip_.loadTexture(texPath);
        }
        root_ = -1;
        logo_ = -1;
        for (std::size_t i = 0; i < clip_.exports.size(); i++) {
            if (clip_.exports[i].name == rootName_) {
                root_ = clip_.exports[i].id;
            }
            if (clip_.exports[i].name == logoName_) {
                logo_ = clip_.exports[i].id;
            }
        }
        renderer_.bind(&clip_);
        renderer_.setStatusText(statusText_);
        loaded_ = root_ >= 0;
        return loaded_;
    }
    void setRoot(const std::string &name) {
        rootName_ = name;
    }
    void setProgress(float p) {
        if (p < 0) {
            p = 0;
        }
        if (p > 1) {
            p = 1;
        }
        progress_ = p;
        renderer_.setProgress(p);
    }
    void setStatusText(const std::string &text) {
        statusText_ = text;
        renderer_.setStatusText(text);
    }
    void setFonts(const std::vector<std::string> &paths) {
        renderer_.setFonts(paths);
    }
    void setSystemFonts(const std::vector<std::string> &paths) {
        renderer_.setSystemFonts(paths);
    }
    void setAssetDir(const std::string &assetsDir) {
        renderer_.setAssetDir(assetsDir);
    }
    void drawLabel(std::vector<std::uint32_t> &frame, int w, int h, const std::string &labelText,
                   float boxX, float boxY, float boxW, float boxH,
                   std::uint32_t color, bool outline, std::uint32_t outlineColor) {
        renderer_.drawLabel(frame, w, h, labelText, boxX, boxY, boxW, boxH, color, outline, outlineColor);
    }
    bool loaded() const { return loaded_; }
    bool hasLogo() const { return logo_ >= 0; }
    int logoFrames() const {
        if (logo_ < 0) {
            return 0;
        }
        return renderer_.logoFrames(logo_);
    }
    void blitLogo(std::vector<std::uint32_t> &frame, int w, int h, int frameIndex) const {
        if (logo_ < 0 || frame.empty()) {
            return;
        }
        renderer_.renderLogo(frame, w, h, logo_, frameIndex);
    }
    float progress() const { return progress_; }
    const std::string &statusText() const { return statusText_; }
    void blit(std::vector<std::uint32_t> &frame, int w, int h) const {
        if (!loaded_ || frame.empty()) {
            return;
        }
        renderer_.render(frame, w, h, root_);
    }
private:
    SupercellSWF clip_;
    mutable ClipRenderer renderer_;
    std::string rootName_ = "loading_screen";
    std::string logoName_ = "sc_intro";
    std::string statusText_;
    int root_ = -1;
    int logo_ = -1;
    float progress_ = 0;
    bool loaded_ = false;
};
