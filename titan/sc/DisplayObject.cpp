#pragma once

// DisplayObject — scene-graph transform core (sc engine).
//
// SCC-merge exception to the one-class-per-file rule: DisplayObject <->
// Sprite form an include cycle (DisplayObject::isChildOf needs Sprite
// complete; Sprite inherits DisplayObject). No include order satisfies
// both through MovieClip, so Sprite lives in this TU: DisplayObject,
// then Sprite (base complete), then isChildOf (Sprite complete).
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

// DisplayObject bodies needing the complete Sprite type (defined below).

#include <cstdint>
#include <vector>

namespace titan::sc {

// Sprite — DisplayObject with a child list (sc engine).
//
// Binary layout (verified via IDA):
//   children array @+72, count u16 @+70 (addChild @0x2b0b90 appends via
//   addChildAt @0x687b14; getChildrenArray @0x35ae34; removeAllChildren
//   @0x2bddd8 pops from the back via removeChildAt @0x27bcb4).
// Child bookkeeping on DisplayObject: parent @+48 (null when detached),
// index @+56 (-1 when detached). Reparenting detaches from the old parent
// first (slot +208 call in addChildAt). Stage hooks skipped (platform);
// out-of-range indices clamp (binary assumes valid input).
class Sprite : public DisplayObject {
public:
    // addChild @0x2b0b90: append (addChildAt with count).
    void addChild(DisplayObject* child) { addChildAt(child, childCount()); }
    // addChildAt @0x687b14: detach from old parent, insert at index,
    // reindex followers. Stage handling skipped.
    void addChildAt(DisplayObject* child, int index) {
        if (!child) return;
        if (child->parent_ && child->parent_ != this) {
            child->parent_->removeChild(child);
        } else if (child->parent_ == this) {
            removeAt(child->index_);
        }
        if (index < 0) index = 0;
        if (index > childCount()) index = childCount();
        children_.insert(children_.begin() + index, child);
        child->parent_ = this;
        reindex();
    }
    // removeChildAt @0x27bcb4: detach, shift followers, reindex.
    // Stage hooks skipped.
    void removeChildAt(int index) {
        if (index < 0 || index >= childCount()) return;
        DisplayObject* child = children_[index];
        children_.erase(children_.begin() + index);
        child->parent_ = nullptr;
        child->index_ = -1;
        reindex();
    }
    void removeChild(DisplayObject* child) {
        // removeChild @0x92bbe0: guard (parent match + index != -1),
        // then detach via removeChildAt semantics.
        if (!child || child->parent_ != this || child->index_ == -1) return;
        removeChildAt(child->index_);
    }
    // setChildIndex @0x1ca7fc: in-place reorder within this parent only
    // (a foreign child is a no-op — unlike addChildAt, no reparenting).
    // Out-of-range targets are ignored (binary assumes valid input).
    void setChildIndex(DisplayObject* child, int index) {
        if (!child || child->parent_ != this) return;
        const int cur = child->index_;
        if (cur == index || index < 0 || index >= childCount()) return;
        if (cur < index) {
            for (int i = cur; i < index; ++i) {
                children_[i] = children_[i + 1];
                children_[i]->index_ = i;
            }
        } else {
            for (int i = cur; i > index; --i) {
                children_[i] = children_[i - 1];
                children_[i]->index_ = i;
            }
        }
        children_[index] = child;
        child->index_ = index;
    }
    // removeAllChildren @0x2bddd8.
    void removeAllChildren() {
        for (auto* c : children_) {
            c->parent_ = nullptr;
            c->index_ = -1;
        }
        children_.clear();
    }
    // getChildrenArray @0x35ae34.
    [[nodiscard]] const std::vector<DisplayObject*>& children() const {
        return children_;
    }
    [[nodiscard]] int childCount() const {
        return static_cast<int>(children_.size());
    }

private:
    void removeAt(int index) {
        children_.erase(children_.begin() + index);
        reindex();
    }
    void reindex() {
        for (int i = 0; i < childCount(); ++i) children_[i]->index_ = i;
    }

    std::vector<DisplayObject*> children_; // +72 (+70 count)
};

} // namespace titan::sc

namespace titan::sc {

inline bool DisplayObject::isChildOf(const Sprite* ancestor) const {
    if (ancestor == this) return true;
    const Sprite* p = parent_;
    if (!p) return false; // binary dereferences null here (crash)
    while (p != ancestor) {
        p = p->parent_;
        if (!p) return false;
    }
    return true;
}

} // namespace titan::sc

