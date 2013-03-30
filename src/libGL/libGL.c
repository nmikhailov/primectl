#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "common/config.h"
#include "libGL.h"
#include "dbus.h"

void on_load(void) {
    // Connect to dbus
    int new_driprime, flag_connected = 0;
    if (!dbus_send_hook_load(getpid(), &new_driprime)) {
        flag_connected = 1;
    } else {
        fprintf(stderr, "PLGL: Error, can't connect to dbus server");
    }

    if (flag_connected) {
        if (new_driprime != -1) {
            // New value
            char buff[64];
            snprintf(buff, sizeof(buff), "%d", new_driprime);
            setenv("DRI_PRIME", buff, 1);

            fprintf(stderr, "DRI_PRIME is set to: %s", buff);
        } else {
            fprintf(stderr, "DRI_PRIME");

            fprintf(stderr, "DRI_PRIME is unset");
        }
    } else {
        // Connection failed or no change required
        fprintf(stderr, "DRI_PRIME unchanged: %s", getenv("DRI_PRIME"));
    }

    // Load original libGL
    libgl = load_lib(LIBGL_DEFAULT);

    fprintf(stderr, "Loaded");
}

void* load_lib(const char *path) { // Load shared library
    fprintf(stderr, "Loading %s", path);

    // libGL requires RTLD_GLOBAL: http://dri.sourceforge.net/doc/DRIuserguide.html
    void *lib = dlopen(path, RTLD_NOW | RTLD_GLOBAL);
    if (!lib) {
        fprintf(stderr, "Can't load %s, error: %s. Abort.", path, dlerror());
        exit(1);
    }
    return lib;
}

void on_unload(void) {
    fprintf(stderr, "%s", "Unloading...");

    // Unload libraries
    dlclose(libgl);
}

