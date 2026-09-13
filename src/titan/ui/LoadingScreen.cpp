#pragma once
#include "MovieClip.cpp"
#include "../../logic/data/Localization.cpp"
#include <cstdint>
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
    }
    void showLoading() {
        stage_ = Stage::Loading;
    }
    void update(float dt) {
        time_ += dt;
    }
    std::string statusText() const {
        return texts_.text("TID_CONNECTING_TO_SERVER", "Loading...");
    }
    void draw(std::vector<std::uint32_t> &frame, int w, int h) const {
        if (stage_ == Stage::Logo && clip_.hasLogo()) {
            clip_.blitLogo(frame, w, h);
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
    Localization texts_;
    std::string assetsDir_;
    bool textsOk_ = false;
    bool clipOk_ = false;
    float time_ = 0;
    Stage stage_ = Stage::Boot;
};
