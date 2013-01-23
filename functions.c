#include <stdio.h>
#include <dlfcn.h>
#include <GL/gl.h>
#include <GL/glx.h>

extern void *library_libgl;

#define DEF_PROXY(name, ret, args, args2)\
ret name args {\
	puts(#name);\
	void *fn = dlsym(library_libgl, #name);\
	return ((ret (*)args)fn)args2;\
}

#include "functions.h"

#undef DEF_PROXY
