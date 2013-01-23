CC=gcc
CFLAGS=-Wall -shared -fPIC -Wl,-soname,GL
LDFLAGS=
SOURCES=main.c utils.c functions.c
EXECUTABLE=libGL.so.1

all: libgl

libgl:
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)
