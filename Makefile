CC=gcc
CFLAGS=-Wall -shared -fPIC
LDFLAGS=
SOURCES=libgl.c utils.c
EXECUTABLE=libGL.so.1

CFLAGS+=-DLIBS_DEFAULT="\"/usr/lib/libGL.so:/usr/lib/libglapi.so\""

all: libgl

libgl:
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)
