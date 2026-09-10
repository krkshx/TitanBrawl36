#pragma once

// DisplayObject — scene-graph transform core (sc engine).
//
// Binary layout (from C2 @0x529c3c; all offsets verified via IDA):
//   scaleX @+16 (get @0x56a36c, set @0x65cf68),
//   scaleY @+28 (get @0x8a2ad8, set @0x40e228),
//   setScale @0x6f5a94 writes both,
//   x @+32 (get @0x29ea04, set @0x90a0e8),
//   y @+36 (get @0x8e8710, setXY @0x3a324c writes both).
// (+20/+24 between the scales are untouched here — rotation/skew live
// there; mapped in a later wave. Children list likewise pending.)

namespace titan::sc {

class Sprite;

class DisplayObject {
public:
    DisplayObject() = default;
    virtual ~DisplayObject() = default;

    // getX @0x29ea04 / setX @0x90a0e8.
    [[nodiscard]] float getX() const { return x_; }
    void setX(float v) { x_ = v; }
    // getY @0x8e8710.
    [[nodiscard]] float getY() const { return y_; }
    void setY(float v) { y_ = v; }
    // setXY @0x3a324c.
    void setXY(float x, float y) {
        x_ = x;
        y_ = y;
    }
    // getScaleX @0x56a36c / setScaleX @0x65cf68.
    [[nodiscard]] float getScaleX() const { return scaleX_; }
    void setScaleX(float v) { scaleX_ = v; }
    // getScaleY @0x8a2ad8 / setScaleY @0x40e228.
    [[nodiscard]] float getScaleY() const { return scaleY_; }
    void setScaleY(float v) { scaleY_ = v; }
    // setScale @0x6f5a94.
    void setScale(float v) {
        scaleX_ = v;
        scaleY_ = v;
    }
    // Visible flag @+8 (C2 sets 1; setVisibleRecursive @0x9448f4 writes it —
    // the binary does not actually recurse despite the name).
    [[nodiscard]] bool isVisible() const { return visible_; }
    void setVisible(bool v) { visible_ = v; }
    void setVisibleRecursive(bool v) { visible_ = v; }

    // Child bookkeeping (used by Sprite): parent @+48 (null detached),
    // index @+56 (-1 detached). See Sprite::addChildAt @0x687b14.
    // Public like all port data fields (never serialized).
    Sprite* parent_ = nullptr; // +48
    int index_ = -1; // +56

    // isChildOf @0x2d8b40: self, else walk +48 chain; null reached ->
    // false. (A parentless non-self object would crash the binary
    // dereferencing null — ours returns false safely.)
    // Out-of-line (needs complete Sprite).
    [[nodiscard]] bool isChildOf(const Sprite* ancestor) const;

private:
    bool visible_ = true; // +8
    float scaleX_ = 1.0f; // +16 (C2 defaults from identity const)
    float x_ = 0.0f; // +32
    float y_ = 0.0f; // +36
    // NOTE: binary keeps scaleY at +28 with +20/+24 between (see above);
    // the port stores it adjacent — wire state is never serialized.
    float scaleY_ = 1.0f; // +28 in the binary
};

} // namespace titan::sc
