#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "libGL.h"
#include "utils.h"
#include "common/config.h"

void on_load(void) {
    GError *error = NULL;

    // Set debug level
    debug_enabled = getenv("PLGL_DEBUG") != NULL;

    // Connect to dbus
    g_type_init();

    dbus_proxy = org_dri_prime_ctl_proxy_new_for_bus_sync(G_BUS_TYPE_SESSION,
            0, DBUS_SERVER_NAME, DBUS_SERVER_PATH, NULL, &error);

    if (dbus_proxy == NULL) {
        err_printf("%s\n", "Failed to create dbus manager");
        exit(1);
    }

    // Connect to server
    dbg_printf("%s\n", "Loading libraries...");
    /*poc();*/
    poc_dbus();

    // Load original libGL
    libgl = load_lib(LIBGL_DEFAULT);

    dbg_printf("%s\n", "Loaded");
}


void* load_lib(const char *path) { // Load shared library
    dbg_printf("Loading %s\n", path);

    // libGL requires RTLD_GLOBAL: http://dri.sourceforge.net/doc/DRIuserguide.html
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
    dbg_printf("current process %s\n", buff);

    if (strcmp(buff, "glxinfo") == 0) {
        setenv("DRI_PRIME", "1", 1);
        dbg_printf("%s\n", "DRI_PRIME is set");
    }
    fclose(f);
}

void poc_dbus() {
    GError *error = NULL;
    guint prime_val = -1;

    gchar *prime_old = getenv("DRI_PRIME");
    if (prime_old == NULL) {
        prime_old = "";
    }

    if (!org_dri_prime_ctl_call_hook_libgl_load_sync(dbus_proxy, getpid(), prime_old, &prime_val, NULL, &error)) {
        err_printf("%s\n", "Can't send hook_load");
        g_printerr("Error : %s", error->message);
        g_error_free(error);
        exit(1);
    }

    if (prime_val != -1) {
        char buff[256];
        snprintf(buff, 256, "%d", prime_val);
        setenv("DRI_PRIME", buff, 1);

        dbg_printf("DRI_PRIME changed to: %s\n", buff);
    } else {
        unsetenv("DRI_PRIME");

        dbg_printf("%s\n", "DRI_PRIME unset");
    }
}

void on_unload(void) {
    dbg_printf("%s\n", "Unloading...");

    // Send dbus message
    org_dri_prime_ctl_call_hook_libgl_unload(dbus_proxy, getpid(), NULL, NULL, NULL);
    /*g_object_unref(dbus_proxy);*/

    // Unload libraries
    dlclose(libgl);

    dbg_printf("%s\n", "Unloaded");
}

