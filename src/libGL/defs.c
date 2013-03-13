#include <stdio.h>
#include <dlfcn.h>
//#include <GL/glx.h>

#include "libGL.h"

#define DEF_PROXY(name, ret, args, args2)\
__attribute__((visibility("default")))\
ret name args {\
    ret (*fn)args;\
    *(void **) (&fn) = dlsym(libgl, #name);\
    return fn args2;\
}

#define DEF_PROXY_VOID(name, args, args2)\
__attribute__((visibility("default")))\
void name args {\
    void (*fn)args;\
    *(void **) (&fn) = dlsym(libgl, #name);\
    fn args2;\
}

#include "glx.def"

#undef DEF_PROXY
#undef DEF_PROXY_VOID
