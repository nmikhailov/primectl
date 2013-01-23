// My LibGL
#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "libgl.h"
#include "utils.h"

// Original shared libraries
void **libraries;
size_t libraries_size;

// Debug state
int debug_enabled;

void load(void) {
    // Set debug level
    debug_enabled = getenv("PLGL_DEBUG") != NULL;

    // Process
    dbg_printf("%s\n", "Loading libraries...");

    // Load original libs
    load_all();

    // Connect to server
    // ...
    poc();

    dbg_printf("%s\n", "Loaded");
}

void load_all() {
    // Colon-separated libs string
    char *libs;

    if (getenv("PLGL_LIBS") != NULL ) {
        libs = strdup(getenv("PLGL_LIBS")); // User defined
    } else {
        libs = strdup(LIBS_DEFAULT); // Default value
    }

    // Parse string & load libs
    libraries = NULL;
    libraries_size = 0;

    char *token = strtok(libs, ":");
    while (token) {
        // Resize libraries array
        libraries_size++;
        libraries = (void**) realloc(libraries, libraries_size * sizeof(void*));

        // Load library
        libraries[libraries_size - 1] = load_lib(token);

        // Next token
        token = strtok(NULL, ":");
    }
    free(libs);
}

void* load_lib(const char *path) { // Load shared library
    dbg_printf("Loading %s\n", path);
    void *lib = dlopen(path, RTLD_LAZY | RTLD_GLOBAL);
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
    puts(buff);

    if (strcmp(buff, "glxinfo") == 0) {
        setenv("DRI_PRIME", "1", 1);
        puts("DRI_PRIME is set");
    }
    fclose(f);
}

void unload(void) {
    dbg_printf("%s\n", "Unloading...");
    size_t i;

    // Unload libraries
    for (i = 0; i < libraries_size; i++) {
        dlclose(libraries[i]);
    }
    free(libraries);
    dbg_printf("%s\n", "Unloaded");
}

