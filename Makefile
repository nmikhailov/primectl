CC=gcc
CFLAGS=-Wall -shared -fPIC -Wl,-soname,GL
LDFLAGS=
SOURCES=main.c
EXECUTABLE=libGL.so.1

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

clean:
	rm $(EXECUTABLE)
