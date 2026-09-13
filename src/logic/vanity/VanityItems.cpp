#pragma once
#include "VanityItemEntry.cpp"

struct VanityItems {
    std::vector<VanityItemEntry> items;
    static VanityItems decode(ByteStream &s) {
        VanityItems e;
        std::int32_t n = s.readVInt();
        for (std::int32_t i = 0; i < n; i++) {
            e.items.push_back(VanityItemEntry::decode(s));
        }
        return e;
    }
};
