#include <stdlib.h>
#include <string.h>

#include <GL/gl.h>
#include <GL/glx.h>

#include "test_common.h"

void init_functions() {
    gldefs d = {
        .glViewport = glViewport,
        .glClearColor = glClearColor,
        .glClear = glClear,
        .glMatrixMode = glMatrixMode,
        .glLoadIdentity = glLoadIdentity,
        .glOrtho = glOrtho,
        .glPushMatrix = glPushMatrix,
        .glScalef = glScalef,
        .glBegin = glBegin,
        .glEnd = glEnd,
        .glColor3f = glColor3f,
        .glVertex2f = glVertex2f,
        .glPopMatrix = glPopMatrix,
        .glReadPixels = glReadPixels,
        .glGetError = glGetError,

        // glx
        .glXGetProcAddressARB = glXGetProcAddressARB,
        .glXChooseFBConfig = glXChooseFBConfig,
        .glXCreatePbuffer = glXCreatePbuffer,
        .glXMakeContextCurrent = glXMakeContextCurrent,
    };

    memcpy(&gl, &d, sizeof(gldefs));
}
