#pragma once

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

#include "titan/sc/DisplayObject.hpp"

#include <cstdint>
#include <vector>

namespace titan::sc {

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
