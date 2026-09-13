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
        for (auto &e : clip_.exports) {
            if (e.name == rootName_) {
                root_ = e.id;
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
        renderer_.setBarFrame(static_cast<int>(p * 100.0f));
    }
    void setStatusText(const std::string &text) {
        statusText_ = text;
        renderer_.setStatusText(text);
    }
    void setFonts(const std::string &primary, const std::string &fallback) {
        renderer_.setFonts(primary, fallback);
    }
    bool loaded() const { return loaded_; }
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
    std::string statusText_;
    int root_ = -1;
    float progress_ = 0;
    bool loaded_ = false;
};
