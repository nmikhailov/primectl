#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>

void dbg_fprintf(FILE *stream, const char *fmt, ...);

#define dbg_printf(fmt, ...) dbg_fprintf(stdout, __VA_ARGS__)


#endif /* UTILS_H_ */
