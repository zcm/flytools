CC := clang
TESTS = $(patsubst %.c,bin/%,$(wildcard test_*.c))

CFLAGS += -g -I../include -I../src -Wall -std=c2x \
	-fsanitize=address -fno-omit-frame-pointer -fno-common

LDFLAGS += -lm -lcmocka -Wl,--wrap=malloc

all: $(TESTS)

.PHONY: clean run

mockmem.o: mockmem.c mockmem.h

bin/test_% : test_%.c mockmem.o ../build/libflytools.a
	$(CC) $(CFLAGS) \
		$(LDFLAGS) \
		-o $@ $^

../build/flytools.a:
	$(MAKE) -C ..

run: all
	$(foreach test,$(TESTS),$(test) && ) true

clean:
	rm -f *.o $(TESTS)
