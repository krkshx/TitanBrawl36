#pragma once

// MovieClip — named-children Sprite (sc engine, minimal port).
//
// Binary keeps parallel children @+112 / names @+120 arrays (counts u16
// @+150; frames @+128). Our port keys names by pointer: identical
// observable behavior, robust across removals.
//   getNameOfChild @0x266cb0: linear pointer search, "" on miss (binary
//     returns a default resource string).
//   setChildVisible @0x24417c: case-insensitive name match, sets the
//     child's visible flag @+8. Frame playback stays pending.

#include "titan/sc/Sprite.hpp"

#include <map>
#include <string>

#if defined(_WIN32)
#include <string.h>
#define sc_strcasecmp _stricmp
#else
#include <strings.h>
#define sc_strcasecmp strcasecmp
#endif

namespace titan::sc {

class MovieClip : public Sprite {
public:
    void addNamedChild(DisplayObject* child, const std::string& name) {
        addChild(child);
        if (child) names_[child] = name;
    }
    [[nodiscard]] std::string getNameOfChild(const DisplayObject* child) const {
        const auto it = names_.find(child);
        return it != names_.end() ? it->second : "";
    }
    // setChildVisible @0x24417c.
    void setChildVisible(const std::string& name, bool visible) {
        for (auto* c : children()) {
            const auto it = names_.find(c);
            if (it != names_.end()
                && sc_strcasecmp(it->second.c_str(), name.c_str()) == 0) {
                c->setVisible(visible);
                return;
            }
        }
    }

private:
    std::map<const DisplayObject*, std::string> names_;
};

} // namespace titan::sc
