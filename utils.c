#include "utils.h"

#include <stdarg.h>

extern int M_DEBUG;

void dbg_fprintf(FILE *stream, const char *fmt, ...) {
    if (M_DEBUG) {
        va_list args;
        va_start(args, fmt);

        fprintf(stream, "ProxyGL: ");
        fprintf(stream, fmt, args);
    }
}

