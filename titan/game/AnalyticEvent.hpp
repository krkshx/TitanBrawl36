#pragma once

// AnalyticEvent::encode @0x2787dc, decode @0x945888.
// Wire: stringref name @+8, string (nullable) of the params JSONObject
// (@+24) serialized via createJSONString @0x595724.
// Decode reads both strings and re-parses the JSON (errors loudly, like
// the binary's Debugger::error).

#include "titan/game/LogicJSONObject.hpp"
#include "titan/game/LogicJSONParser.hpp"
#include "titan/messages/Nested.cpp"

#include <memory>
#include <stdexcept>
#include <string>

namespace titan {

class AnalyticEvent : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeStringReference(name_);
        if (!params_) throw pending_reverse("AnalyticEvent needs params");
        const std::string json = LogicJSONParser::stringify(*params_);
        s.writeString(&json);
    }
    void decode(ByteStream& s) override {
        name_ = s.readStringReference();
        auto text = s.readString();
        if (!text) throw std::runtime_error("AnalyticEvent: null params");
        auto node = LogicJSONParser::parse(*text);
        auto* obj = dynamic_cast<LogicJSONObject*>(node.get());
        if (!obj) throw std::runtime_error("AnalyticEvent: params not an object");
        node.release();
        params_.reset(obj);
    }

    std::string name_; // +8 (embedded String)
    std::unique_ptr<LogicJSONObject> params_; // +24 (required)
};

} // namespace titan
