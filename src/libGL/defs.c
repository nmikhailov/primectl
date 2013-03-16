#include <stdio.h>
#include <dlfcn.h>
#include <GL/glx.h>

#include "libGL.h"

#define BRACE_START (
#define BRACE_END )

#define DEF_PROXY(name, ret, args, args2, return)\
__attribute__((visibility("default")))\
ret name args {\
    static ret (*fn)args = NULL;\
    if (fn == NULL) {\
        *(void **) (&fn) = dlsym(libgl, #name);\
    }\
    return fn args2;\
}

#include "glx.def"

#undef DEF_PROXY

#undef BRACE_END
#undef BRACE_START
