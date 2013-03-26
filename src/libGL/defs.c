#include <stdio.h>
#include <dlfcn.h>

#include "libGL.h"


#define DEF_FRWD(name)\
void name(void) __attribute__((visibility("default"), ifunc("getaddr_" #name)));\
void* getaddr_##name() {\
    if (libgl == NULL)\
        return 0;\
    static void* fn = NULL;\
    if (fn == NULL) {\
        fn = dlsym(libgl, #name);\
    }\
    return fn;\
}

#include "glx.def"

#undef DEF_FRWD
