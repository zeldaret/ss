#include "s/s_Print.h"

#include "rvl/OS/OSError.h"

#include <cstdarg>
#include <cstdio>
#include <stdio.h>

namespace sLib {

typedef int (*vprintf_sig)(const char *format, va_list args);

static vprintf_sig sPrintfFunc = &std::vprintf;

static vprintf_sig getVprintfFunc() {
    return sPrintfFunc;
}

static void my_vprintf(const char *format, va_list args) {
    (getVprintfFunc())(format, args);
}

void printf(const char *msg, ...) {
    va_list l;
    va_start(l, msg);
    vprintf(msg, l);
    va_end(l);
}

} // namespace sLib

// Overriding the weakly linked OS functions...
void OSReport(const char *msg, ...) {
    va_list l;
    va_start(l, msg);
    OSVReport(msg, l);
    va_end(l);
}

void OSVReport(const char *msg, va_list args) {
    sLib::my_vprintf(msg, args);
}
