AR = ar
CFLAGS = -Wall -O3 -fomit-frame-pointer -std=c99
CC = gcc
LD = ld
LDFLAGS =
RM = rm -f

LIBS = -lc

MAKEFILE = Makefile

OBJ = hash.o dict_c.o
BIN = dict.o
LIB = dict.a

.PHONY: all clean lib bin
.SILENT: force_look

all: bin

bin: $(BIN)

lib: $(LIB)

clean:
	$(RM) $(OBJ) $(BIN) $(LIB)

$(LIB): $(EXTOBJ) $(OBJ)
	$(AR) rcs $@ $?

hash.o: hash.c hash.h $(MAKEFILE)
	$(CC) -c hash.c -o $@ $(CFLAGS)
dict_c.o: hash.o dict.c dict.h $(MAKEFILE)
	$(CC) -c dict.c -o $@ $(CFLAGS)

dict.o: $(OBJ) $(MAKEFILE)
	$(LD) -r $(OBJ) -o $@
