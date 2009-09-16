# This is a makefile written for Borland MAKE 3.6.
# Intended to be used with Turbo C++ 3.0.
# Written by Zachary Murray (dremelofdeath@gmail.com)
# Added to project on 01/30/09 18:10:26.

.autodepend

!ifndef CFLAGS
CFLAGS =
!endif
CC = tcc
RM = del

MAKEFILE = Makefile.mak

OBJ = hash.obj dict.obj
LIB = dict.lib

all: $(LIB)

clean:
	$(RM) $(LIB)
	$(RM) $(LIB:.lib=.bak)
	$(RM) hash.obj
	$(RM) dict.obj

$(LIB): $(OBJ) $(MAKEFILE)

.c.obj:
	$(CC) $(CFLAGS) -c $<
	tlib $(LIB) -+$@
