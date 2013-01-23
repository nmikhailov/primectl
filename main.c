// My LibGL
#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils.h"


#define LIBGL_PATH "/usr/lib/libGL.so"
#define LIBGLAPI_PATH "/usr/lib/libglapi.so"

void __attribute__ ((constructor)) load(void);
void __attribute__ ((destructor)) unload(void);

// Original shared libraries
void *library_libgl, *library_libglapi;

// Debug state
int M_DEBUG;


void load_libraries();
void poc();

void load(void) {
    // Set debug level
    M_DEBUG = getenv("PLGL_DEBUG") != NULL;

    // Process
    dbg_printf("%s\n", "Proxy libGL loading...");

    // Load original libs
    load_libraries();

    // Connect to server
    // ...
    poc();

    dbg_printf("%s\n", "Proxy libGL loaded.");
}

void load_libraries() {
    library_libgl = dlopen(LIBGL_PATH, RTLD_LAZY | RTLD_GLOBAL);
    if (!library_libgl) {
        fprintf(stderr, "Can't load libGL(%s), error: %s. Exiting.\n", LIBGL_PATH, dlerror());
        exit(1);
    }

    library_libglapi = dlopen(LIBGLAPI_PATH, RTLD_LAZY | RTLD_GLOBAL);
    if (!library_libglapi) {
        fprintf(stderr, "Can't load libglapi(%s), error: %s. Exiting.\n", LIBGLAPI_PATH, dlerror());
        exit(1);
    }
}

void poc() {
    // Proof of concept
    FILE *f = fopen("/proc/self/cmdline", "r");
    char buff[1024];
    fread(buff, 1024, 1, f);
    puts(buff);

    if (strcmp(buff, "glxinfo") == 0) {
        setenv("DRI_PRIME", "1", 1);
        puts("DRI_PRIME is set");
    }
    fclose(f);
}

void unload(void) {
    dbg_printf("%s\n", "Proxy libGL unloading...");

    // Unload original libGL and libglapi
    dlclose(library_libgl);
    dlclose(library_libglapi);

    dbg_printf("%s\n", "Proxy libGL unloaded");
}

