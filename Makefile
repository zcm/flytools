OBJ = src/common.o src/generics.o src/dict.o src/hash.o src/scanner.o src/list.o src/llnodes.o
CFLAGS += -Iinclude -Wall -O3 -DFLYAPIBUILD -std=gnu99
VPATH = src:include:.

# uncomment to enable compilation of justification code
#OBJ += src/justify.o

all: $(OBJ) build/flytools.a($(OBJ))

.PHONY: clean

src/dict.o: hash.h dict.h common.h
src/hash.o: hash.h common.h
src/scanner.c: scanner.h
src/scanner.o: scanner.h scanner.c common.h
src/list.o: list.h llnodes.h common.h
src/llnodes.o: llnodes.h common.h

# uncomment to enable compilation of justification code
#src/justify.o: justify.h scanner.h common.h

flytools.a($(OBJ)): $(OBJ)

clean:
	rm -f build/*.a
	rm -f build/*.o
	rm -f src/*.o
	rm -f src/scanner.c
