AR = ar
CFLAGS = -Wall -O3 -fomit-frame-pointer
CC = gcc
LD = ld
LDFLAGS =
RM = rm -f

LIBS = -lc

MAKEFILE = Makefile

OBJ = llnodes.o dllist.o sllist.o llist_c.o
BIN = llist.o
LIB = llist.a

.PHONY: all clean lib bin

all: bin

bin: $(BIN)

lib: $(LIB)

clean:
	$(RM) $(OBJ) $(BIN) $(LIB)

$(LIB): $(OBJ)
	$(AR) rcs $@ $?

llnodes.o: llnodes.c llnodes.h $(MAKEFILE)
	$(CC) -c llnodes.c -o $@ $(CFLAGS)
dllist.o: dllist.c dllist.h llnodes.h $(MAKEFILE)
	$(CC) -c dllist.c -o $@ $(CFLAGS)
sllist.o: sllist.c sllist.h llnodes.h $(MAKEFILE)
	$(CC) -c sllist.c -o $@ $(CFLAGS)
llist_c.o: llist.c llist.h llnodes.h dllist.h sllist.h $(MAKEFILE)
	$(CC) -c llist.c -o $@ $(CFLAGS)

llist.o: $(OBJ) $(MAKEFILE)
	$(LD) -r $(OBJ) -o $@
