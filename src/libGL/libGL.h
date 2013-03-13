#ifndef LIBGL_H_
#define LIBGL_H_

#include "common/config.h"
#include "dbus_bindings.h"

void __attribute__ ((constructor)) on_load(void);
void __attribute__ ((destructor)) on_unload(void);

// dlopen library
void* load_lib(const char *path);
void poc();
void poc_dbus();

// Original libGL
void *libgl;

// Debug state
int debug_enabled;

// Dbus
OrgDriPrimeCtl *dbus_proxy;

#endif /* LIBGL_H_ */
