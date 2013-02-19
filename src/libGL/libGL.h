#ifndef LIBGL_H_
#define LIBGL_H_

#include "common/config.h"

void __attribute__ ((constructor)) on_load(void);
void __attribute__ ((destructor)) on_unload(void);

// dlopen library
void* load_lib(const char *path);

void poc();

#endif /* LIBGL_H_ */