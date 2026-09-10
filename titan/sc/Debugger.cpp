#pragma once

// Debugger — log/error sink (sc engine).
//
// Binary (ARM64, libg_decrypted.so):
//   error @0x569b9c / warning @0x254e64: printf with color wrappers,
//   optional log-listener callback, bump an error/warning counter.
//   Neither throws nor aborts — the binary logs and CONTINUES.
// Port note: our decode paths throw (std::runtime_error/out_of_range)
// where the binary would log-and-continue; throwing is deliberate
// (a library must not silently corrupt state). Use this Debugger for
// non-fatal diagnostics with binary-compatible counting/formatting.

#include <string>

namespace titan::sc {

class Debugger {
public:
    static void error(const std::string& message);
    static void warning(const std::string& message);
    [[nodiscard]] static int errorCount();
    [[nodiscard]] static int warningCount();
    static void resetCounters();
};

} // namespace titan::sc

// Debugger bodies — error @0x569b9c, warning @0x254e64.


#include <cstdio>

namespace titan::sc {

namespace {

int g_errors = 0;
int g_warnings = 0;

} // namespace

// Binary wraps messages in color escape sequences (resource strings);
// the observable content is the message itself on stdout.
inline void Debugger::error(const std::string& message) {
    ++g_errors;
    std::printf("%s\n", message.c_str());
}

inline void Debugger::warning(const std::string& message) {
    ++g_warnings;
    std::printf("%s\n", message.c_str());
}

inline int Debugger::errorCount() {
    return g_errors;
}

inline int Debugger::warningCount() {
    return g_warnings;
}

inline void Debugger::resetCounters() {
    g_errors = 0;
    g_warnings = 0;
}

} // namespace titan::sc

