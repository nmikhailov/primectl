CC=gcc
CFLAGS=-W -Wall -shared -fPIC -lpthread -ldl -pedantic -std=c99
LDFLAGS=
SOURCES=libgl.c utils.c defs.c
EXECUTABLE=libGL.so.1

CFLAGS+=-DLIBGL_DEFAULT="\"/usr/lib/libGL.so\""
CFLAGS+=-D_GNU_SOURCE


all: libgl

libgl: defs.h
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

defs.h:
	tools/gen.py /usr/include/GL/glx.h > defs.h

clean:
	rm -f $(EXECUTABLE)
	rm -f defs.h
