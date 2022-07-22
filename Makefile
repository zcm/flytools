CC := clang
#OBJ = $(patsubst %.c,%.o,$(wildcard src/*.c))
OBJ = src/common.o src/generics.o src/dict.o src/hash.o src/list.o
CFLAGS += -Iinclude -Wall -DFLYAPIBUILD -D_GNU_SOURCE -std=c2x
VPATH = src:include:.

# uncomment to enable compilation of scanner code
#OBJ += src/scanner.o 

# uncomment to enable compilation of justification code
#OBJ += src/justify.o

CTEST = -g -fsanitize=address -fno-omit-frame-pointer -fno-common

all: $(OBJ) build/libflytools.a($(OBJ))

.PHONY: clean test test_clean

src/dict.o: hash.h dict.h common.h list.h
src/hash.o: hash.h common.h
src/list.o: list.h common.h

# uncomment to enable compilation of scanner code
#src/scanner.c: scanner.h
#src/scanner.o: scanner.h scanner.c common.h

# uncomment to enable compilation of justification code
#src/justify.o: justify.h scanner.h common.h

libflytools.a($(OBJ)): $(OBJ)

test: clean
	CFLAGS="$(CFLAGS) $(CTEST)" $(MAKE) -eC .
	$(MAKE) -C test run

test_clean:
	$(MAKE) -C test clean

clean: test_clean
	rm -f build/*.a build/*.o */*.tmp src/*.o src/scanner.c
