#include <stdio.h>
#include <dlfcn.h>

extern void *libgl;

#define DEF_PROXY(name, ret, args, args2)\
ret name args {\
    void *fn = dlsym(libgl, #name);\
    return ((ret (*)args)fn)args2;\
}

#define DEF_PROXY_VOID(name, args, args2)\
void name args {\
    void *fn = dlsym(libgl, #name);\
    ((ret (*)args)fn)args2;\
}

#include "defs.h"

#undef DEF_PROXY
