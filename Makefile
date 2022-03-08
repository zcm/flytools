OBJ = src/common.o src/generics.o src/dict.o src/hash.o src/list.o src/llnodes.o
CFLAGS += -Iinclude -Wall -O3 -DFLYAPIBUILD -std=gnu99
VPATH = src:include:.

# uncomment to enable compilation of scanner code
#OBJ += src/scanner.o 

# uncomment to enable compilation of justification code
#OBJ += src/justify.o

all: $(OBJ) build/libflytools.a($(OBJ))

.PHONY: clean test test_clean

src/dict.o: hash.h dict.h common.h list.h
src/hash.o: hash.h common.h
src/list.o: list.h llnodes.h common.h
src/llnodes.o: llnodes.h common.h

# uncomment to enable compilation of scanner code
#src/scanner.c: scanner.h
#src/scanner.o: scanner.h scanner.c common.h

# uncomment to enable compilation of justification code
#src/justify.o: justify.h scanner.h common.h

libflytools.a($(OBJ)): $(OBJ)

test:
	$(MAKE) -C test run

test_clean:
	$(MAKE) -C test clean

clean: test_clean
	rm -f build/*.a
	rm -f build/*.o
	rm -f src/*.o
	rm -f src/scanner.c
