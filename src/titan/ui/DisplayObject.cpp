#pragma once
#include <cstdint>
#include <string>

class DisplayObject {
public:
    DisplayObject() = default;
    virtual ~DisplayObject() = default;
    void setPosition(float x, float y) { x_ = x; y_ = y; }
    void setScale(float x, float y) { scaleX_ = x; scaleY_ = y; }
    void setRotation(float r) { rotation_ = r; }
    void setVisible(bool v) { visible_ = v; }
    void setName(const std::string &n) { name_ = n; }
    float x() const { return x_; }
    float y() const { return y_; }
    float scaleX() const { return scaleX_; }
    float scaleY() const { return scaleY_; }
    float rotation() const { return rotation_; }
    bool visible() const { return visible_; }
    const std::string &name() const { return name_; }
    void transformPoint(float px, float py, float &ox, float &oy) const {
        float sx = px * scaleX_;
        float sy = py * scaleY_;
        ox = sx + x_;
        oy = sy + y_;
    }
private:
    float x_ = 0;
    float y_ = 0;
    float scaleX_ = 1;
    float scaleY_ = 1;
    float rotation_ = 0;
    bool visible_ = true;
    std::string name_;
};
