OBJ = src/dict.o src/fugue.o src/hash.o src/scanner.o src/dllist.o \
	  src/justify.o src/llnodes.o src/sllist.o
CFLAGS += -Iinclude -Wall -O3
VPATH = src:include:.

all: $(OBJ) build/flytools.a($(OBJ))

.PHONY: clean

src/dict.o: hash.h dict.h
src/fugue.o: fugue.h
src/hash.o: hash.h fugue.h
src/scanner.c: scanner.h
src/scanner.o: scanner.h scanner.c
src/dllist.o: dllist.h llnodes.h
src/justify.o: justify.h scanner.h
src/llnodes.o: llnodes.h
src/sllist.o: sllist.h llnodes.h

flytools.a($(OBJ)): $(OBJ)

clean:
	rm -f build/*.a
	rm -f build/*.o
	rm -f src/*.o
	rm -f src/scanner.c
