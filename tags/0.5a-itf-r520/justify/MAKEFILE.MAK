# This is a makefile written for Borland MAKE 3.6.
# Intended to be used with Turbo C++ 3.0.
# Written by Zachary Murray (dremelofdeath@gmail.com)
# Added to project on 01/30/09 23:00:15.

.autodepend

!ifndef CFLAGS
CFLAGS =
!endif
CC = tcc
MAKE = make
RM = del

MAKEFILE = Makefile.mak

OBJ = justify.obj tcscan.obj
LIB = justify.lib

all: $(DEP) $(LIB)

clean:
	$(RM) $(LIB)
	$(RM) $(LIB:.lib=.bak)
	$(RM) *.obj

$(LIB): $(OBJ) $(MAKEFILE)

.c.obj:
	$(CC) $(CFLAGS) -c $<
	tlib $(LIB) -+$@
