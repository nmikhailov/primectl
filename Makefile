CC=gcc
CFLAGS=-W -Wall -fPIC -pedantic -std=c99 -O2
LDFLAGS=-ldl -lpthread -shared
SOURCES=libgl.c utils.c defs.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=libGL.so.1

CFLAGS+=-DLIBGL_DEFAULT="\"/usr/lib/libGL.so\""
CFLAGS+=-D_GNU_SOURCE

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(EXECUTABLE)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

defs.c: defs.h

defs.h: tools/gen.py
	tools/gen.py /usr/include/GL/glx.h > defs.h

clean:
	rm -f $(EXECUTABLE)
	rm -f defs.h
	rm -f *.o
	rm -f .depends

depend: .depends

.depends: $(SOURCES)
	$(CC) $(CFLAGS) -MM $^ > .depends

-include .depends
