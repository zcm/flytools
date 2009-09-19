OBJ = src/dict.o src/hash.o src/llist.o src/scanner.o src/dllist.o \
      src/justify.o src/llnodes.o src/sllist.o
CFLAGS += -Iinclude -Wall
VPATH = src:include:.

all: $(OBJ) builddir build/flytools.a($(OBJ))

.PHONY: clean builddir

builddir:
	mkdir build

src/dict.o: hash.h dict.h
src/hash.o: hash.h
src/llist.o: llnodes.h dllist.h sllist.h
src/scanner.c: scanner.h
src/scanner.o: scanner.h scanner.c
src/dllist.o: dllist.h llnodes.h
src/justify.o: justify.h scanner.h
src/llnodes.o: llnodes.h
src/sllist.o: sllist.h llnodes.h

flytools.a($(OBJ)): $(OBJ)

clean:
	rm -rf build/
	rm -f src/*.o
	rm -f src/scanner.c
