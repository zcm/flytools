CC := clang
TESTS = $(patsubst %.c,bin/%,$(wildcard test_*.c))

CFLAGS += -g -I../include -I../src -lm -lcmocka -Wall -std=c2x \
	-fsanitize=address -fno-omit-frame-pointer -fno-common

all: $(TESTS)

.PHONY: clean run

bin/% : %.c ../build/libflytools.a
	$(CC) $(CFLAGS) -o $@ $^

../build/flytools.a:
	$(MAKE) -C ..

run: all
	$(foreach test,$(TESTS),$(test) && ) true

clean:
	rm -f *.o $(TESTS)
