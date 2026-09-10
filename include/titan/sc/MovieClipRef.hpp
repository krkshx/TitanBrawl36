#pragma once

// MovieClipRef — (swf file name, export name) value pair (sc engine).
// C2(MovieClip*) @0x254f7c resolves both through the clip's SWF;
// C2(file, export) @0x2416ec takes them directly.

#include <string>

namespace titan::sc {

class MovieClipRef {
public:
    MovieClipRef() = default;
    MovieClipRef(std::string fileName, std::string exportName)
        : fileName_(std::move(fileName)), exportName_(std::move(exportName)) {}

    [[nodiscard]] const std::string& fileName() const { return fileName_; }
    [[nodiscard]] const std::string& exportName() const { return exportName_; }

private:
    std::string fileName_;
    std::string exportName_;
};

} // namespace titan::sc
