#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "common/config.h"
#include "libGL.h"

void dummy_log_handler(const gchar *a, GLogLevelFlags b, const gchar *c, gpointer d) {}

void on_load(void) {
    GError *error = NULL;
    dbus_proxy = NULL;

    // Set log level
    GLogLevelFlags log_level = G_LOG_LEVEL_ERROR | G_LOG_LEVEL_CRITICAL;
    g_log_set_handler(G_LOG_DOMAIN, G_LOG_LEVEL_MASK, dummy_log_handler, NULL);

    if (getenv("PLGL_DEBUG") != NULL) {
        log_level |= G_LOG_LEVEL_MESSAGE;
    }
    g_log_set_handler(G_LOG_DOMAIN, log_level, g_log_default_handler, NULL);


    // Connect to dbus
    g_type_init();

    dbus_proxy = org_dri_prime_ctl_proxy_new_for_bus_sync(G_BUS_TYPE_SESSION,
            0, DBUS_SERVER_NAME, DBUS_SERVER_PATH, NULL, &error);

    if (dbus_proxy == NULL) {
        g_critical("Failed to create dbus proxy: %s", error->message);
    } else {
        // Set timeout
        g_dbus_proxy_set_default_timeout(G_DBUS_PROXY(dbus_proxy), 3000);
    }

    // Connect to dbus server
    dbus_onload();

    // Load original libGL
    libgl = load_lib(LIBGL_DEFAULT);

    g_message("Loaded");
}


void* load_lib(const char *path) { // Load shared library
    g_message("Loading %s", path);

    // libGL requires RTLD_GLOBAL: http://dri.sourceforge.net/doc/DRIuserguide.html
    void *lib = dlopen(path, RTLD_NOW | RTLD_GLOBAL);
    if (!lib) {
        g_error("Can't load %s, error: %s. Abort.", path, dlerror());
        exit(1);
    }
    return lib;
}

void dbus_onload() {
    GError *error = NULL;

    // New DRI_PRIME value
    guint prime_new = -1;

    // Old(Current) DRI_PRIME value
    gchar *prime_old = getenv("DRI_PRIME");
    if (prime_old == NULL) {
        prime_old = "";
    }

    // Send dbus message
    if (!org_dri_prime_ctl_call_hook_libgl_load_sync(dbus_proxy, getpid(), prime_old, &prime_new, NULL, &error)) {
        g_critical("Can't send 'hook_load': %s", error->message);
        g_error_free(error);

        prime_new = -1;
    }

    if (prime_new != -1) {
        // New value
        char buff[64];
        snprintf(buff, 64, "%d", prime_new);
        setenv("DRI_PRIME", buff, 1);

        g_message("DRI_PRIME is set to: %s", buff);
    } else {
        // Connection failed or no change required
        g_message("DRI_PRIME unchanged: %s", getenv("DRI_PRIME"));
    }
}

void on_unload(void) {
    g_message("%s", "Unloading...");

    // Send dbus message
    org_dri_prime_ctl_call_hook_libgl_unload(dbus_proxy, getpid(), NULL, NULL, NULL);
    g_object_unref(dbus_proxy);

    // Unload libraries
    dlclose(libgl);

    g_message("%s", "Unloaded");
}

