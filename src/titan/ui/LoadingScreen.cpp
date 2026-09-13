#pragma once
#include "MovieClip.cpp"
#include "../../logic/data/Localization.cpp"
#include <cstdint>
#include <string>
#include <vector>

class LoadingScreen {
public:
    bool loadTexts(const std::string &assetsDir) {
        assetsDir_ = assetsDir;
        textsOk_ = texts_.load(assetsDir);
        clip_.setStatusText(statusText());
        return textsOk_;
    }
    bool loadClip(const std::string &scPath) {
        clip_.setFonts(assetsDir_ + "/font/LilitaOne-Regular.ttf", "/usr/share/fonts/noto/NotoSans-Regular.ttf");
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
    void update(float dt) {
        time_ += dt;
    }
    std::string statusText() const {
        return texts_.text("TID_CONNECTING_TO_SERVER", "Loading...");
    }
    void draw(std::vector<std::uint32_t> &frame, int w, int h) const {
        if (clip_.loaded()) {
            clip_.blit(frame, w, h);
        } else {
            for (auto &p : frame) {
                p = 0xFF101418u;
            }
        }
    }
private:
    MovieClip clip_;
    Localization texts_;
    std::string assetsDir_;
    bool textsOk_ = false;
    bool clipOk_ = false;
    float time_ = 0;
};
