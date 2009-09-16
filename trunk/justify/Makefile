CFLAGS = -Wall -O3 -fomit-frame-pointer
CC = gcc
LD = ld
LDFLAGS =
LFLAGS =
LEX = flex
RM = rm -f

MAKEFILE = Makefile

GEN = scanner.c
OBJ = justify.o scanner.o
BIN = justifier.o

.PHONY: all clean

all: $(BIN)

clean:
	$(RM) $(OBJ) $(BIN) $(GEN)

$(BIN): $(OBJ) $(MAKEFILE)
	$(LD) -r $(LDFLAGS) $(OBJLIBS) $(OBJ) -o $@

scanner.c: scanner.l scanner.h $(MAKEFILE)
	$(LEX) -o $@ scanner.l $(LFLAGS)

scanner.o: scanner.c $(MAKEFILE)
	$(CC) -c $(CFLAGS) scanner.c -o $@

justify.o: justify.c justify.h $(MAKEFILE)
	$(CC) -c justify.c -o $@ $(CFLAGS)
