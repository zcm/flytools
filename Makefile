CC := clang

#OBJ = $(patsubst %.c,%.o,$(wildcard src/*.c))

OBJ = \
	src/common.o src/generics.o src/dict.o src/hash.o src/list.o \
	src/random.o src/entropy.o

CFLAGS += \
	-Iinclude -Isrc/pcg-c -Isrc/pcg-c/include -I src/fastrange \
	-Wall -DFLYAPIBUILD -D_GNU_SOURCE -std=c2x

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

src/random.o: random.h common.h fastrange.h \
	src/pcg-c/extras/entropy.h src/pcg-c/include/pcg_variants.h

src/entropy.o: src/pcg-c/extras/entropy.c src/pcg-c/extras/entropy.h \
	src/pcg-c/extras/pcg_spinlock.h src/pcg-c/include/pcg_variants.h

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
