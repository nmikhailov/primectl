CC=gcc
CFLAGS=-Wall -shared -fPIC -lpthread -ldl
LDFLAGS=
SOURCES=libgl.c utils.c defs.c
EXECUTABLE=libGL.so.1

CFLAGS+=-DLIBGL_DEFAULT="\"/usr/lib/libGL.so\""

all: libgl

libgl: defs
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

defs:
	tools/gen.py /usr/include/GL/glx.h > defs.h

clean:
	rm -f $(EXECUTABLE)
	rm -f defs.h
