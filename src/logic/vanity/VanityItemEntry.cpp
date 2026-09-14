#pragma once
#include "VanityItemProp.cpp"
#include "../../titan/core/ByteStreamHelper.cpp"
#include <vector>

struct VanityItemEntry {
    std::int32_t ref = 0;
    std::vector<VanityItemProp> props;
    static VanityItemEntry decode(ByteStream &s) {
        VanityItemEntry e;
        e.ref = ByteStreamHelper::readDataReference(&s);
        std::int32_t n = s.readVInt();
        for (std::int32_t i = 0; i < n; i++) {
            e.props.push_back(VanityItemProp::decode(s));
        }
        return e;
    }
};
