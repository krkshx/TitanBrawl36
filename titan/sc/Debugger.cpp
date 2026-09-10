// Debugger bodies — error @0x569b9c, warning @0x254e64.

#include "titan/sc/Debugger.hpp"

#include <cstdio>

namespace titan::sc {

namespace {

int g_errors = 0;
int g_warnings = 0;

} // namespace

// Binary wraps messages in color escape sequences (resource strings);
// the observable content is the message itself on stdout.
void Debugger::error(const std::string& message) {
    ++g_errors;
    std::printf("%s\n", message.c_str());
}

void Debugger::warning(const std::string& message) {
    ++g_warnings;
    std::printf("%s\n", message.c_str());
}

int Debugger::errorCount() {
    return g_errors;
}

int Debugger::warningCount() {
    return g_warnings;
}

void Debugger::resetCounters() {
    g_errors = 0;
    g_warnings = 0;
}

} // namespace titan::sc
