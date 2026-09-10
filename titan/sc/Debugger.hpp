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
