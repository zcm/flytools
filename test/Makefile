TESTS = $(patsubst %.c,%,$(wildcard test_*.c))

CFLAGS += -I../include -lcmocka -Wall -O3 -DFLYAPIBUILD -std=gnu99

all: $(TESTS)

.PHONY: clean run

% : %.c ../build/libflytools.a
	$(CC) $(CFLAGS) -o $@ $^

../build/flytools.a:
	$(MAKE) -C ..

run: all
	$(foreach test,$(TESTS),./$(test);)

clean:
	rm -f *.o $(TESTS)
