#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdarg.h>

#include "common/config.h"
#include "libGL.h"
#include "dbus.h"

#define ENV_VARIABLE "DRI_PRIME"

static void dbg_printf(int lvl, const char *fmt, ...) {
    if (lvl > 0 || getenv("PLGL_DEBUG")) {
        va_list ap;
        va_start(ap, fmt);
        fprintf(stderr, "[PLGL] ");
        vfprintf(stderr, fmt, ap);
        fprintf(stderr, "\n");
        va_end(ap);
    }
}

void on_load(void) {
    // Connect to dbus
    int new_driprime, flag_connected = 0;
    if (!dbus_send_hook_load(getpid(), &new_driprime)) {
        flag_connected = 1;
    } else {
        dbg_printf(1, "Error, can't connect to dbus server");
    }

    if (flag_connected) {
        if (new_driprime != -1) {
            // New value
            char buff[64];
            snprintf(buff, sizeof(buff), "%d", new_driprime);
            setenv(ENV_VARIABLE, buff, 1);

            dbg_printf(0, ENV_VARIABLE " is set to: %s", buff);
        } else {
            unsetenv(ENV_VARIABLE);

            dbg_printf(0, ENV_VARIABLE " is unset");
        }
    } else {
        // Connection to dbus failed
        dbg_printf(0, "%s unchanged: %s", ENV_VARIABLE, getenv(ENV_VARIABLE));
    }

    // Load original libGL
    libgl = load_lib(LIBGL_DEFAULT);

    dbg_printf(0, "Loaded");
}

void* load_lib(const char *path) {
    dbg_printf(0, "Loading %s", path);

    // libGL requires RTLD_GLOBAL: http://dri.sourceforge.net/doc/DRIuserguide.html
    void *lib = dlopen(path, RTLD_NOW | RTLD_GLOBAL);
    if (!lib) {
        dbg_printf(0, "Can't load %s, error: %s. Abort.", path, dlerror());
        exit(EXIT_FAILURE);
    }
    return lib;
}

void on_unload(void) {
    dbg_printf(0, "%s", "Unloading...");
    dlclose(libgl);
}

