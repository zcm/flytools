OBJ = src/dict.o src/hash.o src/scanner.o src/dllist.o src/justify.o \
	  src/llnodes.o src/sllist.o
CFLAGS += -Iinclude -Wall -O3 -DFLYAPIBUILD
VPATH = src:include:.

all: $(OBJ) build/flytools.a($(OBJ))

.PHONY: clean

src/dict.o: hash.h dict.h common.h
src/hash.o: hash.h common.h
src/scanner.c: scanner.h
src/scanner.o: scanner.h scanner.c common.h
src/dllist.o: dllist.h llnodes.h common.h
src/justify.o: justify.h scanner.h common.h
src/llnodes.o: llnodes.h common.h
src/sllist.o: sllist.h llnodes.h common.h

flytools.a($(OBJ)): $(OBJ)

clean:
	rm -f build/*.a
	rm -f build/*.o
	rm -f src/*.o
	rm -f src/scanner.c
