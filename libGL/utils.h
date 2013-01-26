#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>

void m_fprintf(FILE *stream, const char *fmt, ...);

#define dbg_printf(fmt, ...) if (debug_enabled) m_fprintf(stdout, fmt, __VA_ARGS__)
#define err_printf(fmt, ...) m_fprintf(stderr, fmt, __VA_ARGS__)

#endif /* UTILS_H_ */
