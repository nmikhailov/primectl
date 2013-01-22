// My LibGL
#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void __attribute__ ((constructor)) load(void);
void __attribute__ ((destructor)) unload(void);


void* original;

void load(void) {
    puts("hello");
//    pid_t pid = getpid();
    FILE *f = fopen("/proc/self/cmdline", "r");
    char buff[1024];
    fread(buff, 1024, 1, f);
    puts(buff);

    if (strcmp(buff, "glxinfo") == 0) {
        setenv("DRI_PRIME", "1", 1);
    }
    fclose(f);

//    original = dlopen("/usr/lib/libGL.so.1.2.0", RTLD_LAZY);
    original = dlopen("/usr/lib/catalystpxp/fglrx/fglrx-libGL.so.1.2", RTLD_LAZY);
    if (!original) {
        puts("Error");
        puts(dlerror());
    }
}

void * glXChooseVisual(void* a, int b, void* c) {
    puts("glXChooseVisual");
    void * fn = dlsym(original, "glXChooseVisual");
    char* err = dlerror();
    if (err) {
        puts(err);
    }

    typedef void* (*FN)(void *, int, void*);
    return ((FN)fn)(a, b, c);
}

void unload(void) {
    dlclose(original);
    puts("Bye bye");
}

