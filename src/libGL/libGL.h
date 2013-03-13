#ifndef LIBGL_H_
#define LIBGL_H_

#include "common/config.h"
#include "dbus_bindings.h"

void __attribute__ ((constructor)) on_load(void);
void __attribute__ ((destructor)) on_unload(void);

// Load library handle via dlopen
void *load_lib(const char *path);

// Get DRI_PRIME from server
void dbus_onload();

// Original libGL handle
void *libgl;

// Dbus proxy
OrgDriPrimeCtl *dbus_proxy;

#endif /* LIBGL_H_ */
