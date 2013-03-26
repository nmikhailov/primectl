#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include "test_common.h"


// Ignore warnings about pointer cast
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-pedantic"

void init_functions() {
    void *libgl = dlopen("libGL.so", RTLD_LAZY);
    gldefs d = {
        .glViewport = dlsym(libgl, "glViewport"),
        .glClearColor = dlsym(libgl, "glClearColor"),
        .glClear = dlsym(libgl, "glClear"),
        .glMatrixMode = dlsym(libgl, "glMatrixMode"),
        .glLoadIdentity = dlsym(libgl, "glLoadIdentity"),
        .glOrtho = dlsym(libgl, "glOrtho"),
        .glPushMatrix = dlsym(libgl, "glPushMatrix"),
        .glScalef = dlsym(libgl, "glScalef"),
        .glBegin = dlsym(libgl, "glBegin"),
        .glEnd = dlsym(libgl, "glEnd"),
        .glColor3f = dlsym(libgl, "glColor3f"),
        .glVertex2f = dlsym(libgl, "glVertex2f"),
        .glPopMatrix = dlsym(libgl, "glPopMatrix"),
        .glReadPixels = dlsym(libgl, "glReadPixels"),
        .glGetError = dlsym(libgl, "glGetError"),

        // glx
        .glXGetProcAddressARB = dlsym(libgl, "glXGetProcAddressARB"),
        .glXChooseFBConfig = dlsym(libgl, "glXChooseFBConfig"),
        .glXCreatePbuffer = dlsym(libgl, "glXCreatePbuffer"),
        .glXMakeContextCurrent = dlsym(libgl, "glXMakeContextCurrent"),
    };
    memcpy(&gl, &d, sizeof(gldefs));
}
#pragma GCC diagnostic pop
