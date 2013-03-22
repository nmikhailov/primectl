#include <stdio.h>
#include <dlfcn.h>

#include "libGL.h"

#define DEF_FRWD(name)\
void* name() __attribute__((visibility("default"),ifunc("getaddr_" #name)));\
void* getaddr_##name() {\
    static void* fn = NULL;\
    if (fn == NULL) {\
        fn = dlsym(libgl, #name);\
    }\
    return fn;\
}

#include "glx.def"

#undef DEF_FRWD
