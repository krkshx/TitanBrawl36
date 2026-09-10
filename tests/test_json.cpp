// Self-check for the JSON subsystem (parse + serialize) and AnalyticEvent.

#include "titan/game/AnalyticEvent.hpp"
#include "titan/game/LogicJSONArray.hpp"
#include "titan/game/LogicJSONBoolean.hpp"
#include "titan/game/LogicJSONNull.hpp"
#include "titan/game/LogicJSONNumber.hpp"
#include "titan/game/LogicJSONObject.hpp"
#include "titan/game/LogicJSONParser.hpp"
#include "titan/game/LogicJSONString.hpp"

#include <cstdio>

using namespace titan;

static int failures = 0;
#define CHECK(cond) do { \
    if (!(cond)) { ++failures; std::printf("FAIL %d: %s\n", __LINE__, #cond); } \
} while (0)

int main() {
    // All node types parse (objects, arrays, nesting, BOM, whitespace).
    {
        auto n = LogicJSONParser::parse("{\"a\":1,\"b\":true,\"c\":null,"
                                        "\"d\":\"x\",\"e\":[1,2],\"f\":{\"g\":-3}}");
        CHECK(n && n->type() == LogicJSONNode::Type::Object);
        const auto* o = static_cast<const LogicJSONObject*>(n.get());
        CHECK(o->size() == 6);
        const auto* a = static_cast<const LogicJSONNumber*>(o->get("a"));
        CHECK(a && !a->isFloat() && a->intValue() == 1);
        const auto* b = static_cast<const LogicJSONBoolean*>(o->get("b"));
        CHECK(b && b->value());
        CHECK(o->get("c")->type() == LogicJSONNode::Type::Null);
        const auto* e = static_cast<const LogicJSONArray*>(o->get("e"));
        CHECK(e && e->size() == 2);
        const auto* f = static_cast<const LogicJSONObject*>(o->get("f"));
        const auto* g = static_cast<const LogicJSONNumber*>(f->get("g"));
        CHECK(g && g->intValue() == -3);
    }
    // BOM + empty forms.
    {
        auto n = LogicJSONParser::parse("\xEF\xBB\xBF{}");
        CHECK(n && n->type() == LogicJSONNode::Type::Object);
        auto a = LogicJSONParser::parse("[]");
        CHECK(a && a->type() == LogicJSONNode::Type::Array);
    }
    // Floats go through float precision (binary stores float).
    {
        auto n = LogicJSONParser::parse("1.5");
        const auto* v = static_cast<const LogicJSONNumber*>(n.get());
        CHECK(v && v->isFloat() && v->doubleValue() == 1.5);
    }
    // Escapes round-trip (binary escape set incl. \/ ).
    {
        auto n = LogicJSONParser::parse("\"a\\/b\\n\\u0041\"");
        const auto* s = static_cast<const LogicJSONString*>(n.get());
        CHECK(s && s->value() == "a/b\nA");
        CHECK(LogicJSONParser::stringify(*n) == "\"a\\/b\\nA\"");
    }
    // Errors -> null (binary warns + nulls).
    {
        CHECK(LogicJSONParser::parse("") == nullptr);
        CHECK(LogicJSONParser::parse("{") == nullptr);
        CHECK(LogicJSONParser::parse("{\"a\"}") == nullptr);
        CHECK(LogicJSONParser::parse("[1,]") == nullptr);
        CHECK(LogicJSONParser::parse("nul") == nullptr);
        CHECK(LogicJSONParser::parse("+1") == nullptr);
    }
    // Compact serialization, no spaces.
    {
        LogicJSONObject o;
        o.put("x", std::make_unique<LogicJSONNumber>(2));
        o.put("y", std::make_unique<LogicJSONBoolean>(true));
        CHECK(LogicJSONParser::stringify(o) == "{\"x\":2,\"y\":true}");
    }
    // AnalyticEvent (@0x2787dc/@0x945888) round-trips through JSON.
    {
        AnalyticEvent back;
        ByteStream s;
        {
            AnalyticEvent e;
            e.name_ = "login";
            e.params_ = std::make_unique<LogicJSONObject>();
            e.params_->put("mode", std::make_unique<LogicJSONString>("prod"));
            e.params_->put("n", std::make_unique<LogicJSONNumber>(3));
            e.encode(s);
        }
        ByteStream d;
        d.setBuffer(s.data(), s.size());
        back.decode(d);
        CHECK(back.name_ == "login");
        const auto* mode = static_cast<const LogicJSONString*>(
            back.params_->get("mode"));
        CHECK(mode && mode->value() == "prod");
        const auto* n = static_cast<const LogicJSONNumber*>(
            back.params_->get("n"));
        CHECK(n && n->intValue() == 3);
    }

    if (failures == 0) std::puts("json: all ok");
    return failures == 0 ? 0 : 1;
}
