#include "utils.h"

#include <stdarg.h>

void m_fprintf(FILE *stream, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    fprintf(stream, "%s", "PLGL: ");
    vfprintf(stream, fmt, args);
    va_end(args);
}
