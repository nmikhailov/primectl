#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "libgl.h"
#include "utils.h"

// Original libGL
void *libgl;

// Debug state
int debug_enabled;

void load(void) {
    // Set debug level
    debug_enabled = getenv("PLGL_DEBUG") != NULL;

    // Process
    dbg_printf("%s\n", "Loading libraries...");

    // Connect to server
    // ...
    poc();

    // Load original libGL
    libgl = load_lib(LIBGL_DEFAULT);

    dbg_printf("%s\n", "Loaded");
}


void* load_lib(const char *path) { // Load shared library
    dbg_printf("Loading %s\n", path);
    void *lib = dlopen(path, RTLD_NOW | RTLD_GLOBAL);
    if (!lib) {
        err_printf("Can't load %s, error: %s. Exiting.\n", path, dlerror());
        exit(1);
    }
    return lib;
}

void poc() {
    // Proof of concept
    FILE *f = fopen("/proc/self/cmdline", "r");
    char buff[1024];
    fread(buff, 1024, 1, f);
    dbg_printf("current process %s\n", buff);

    if (strcmp(buff, "glxinfo") == 0) {
        setenv("DRI_PRIME", "1", 1);
        dbg_printf("%s\n", "DRI_PRIME is set");
    }
    fclose(f);
}

void unload(void) {
    dbg_printf("%s\n", "Unloading...");
    // Unload libraries
    dlclose(libgl);
    dbg_printf("%s\n", "Unloaded");
}

