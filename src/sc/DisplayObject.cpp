// DisplayObject bodies needing the complete Sprite type.

#include "titan/sc/DisplayObject.hpp"
#include "titan/sc/Sprite.hpp"

namespace titan::sc {

bool DisplayObject::isChildOf(const Sprite* ancestor) const {
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
