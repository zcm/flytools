# This is a makefile written for Borland MAKE 3.6.
# Intended to be used with Turbo C++ 3.0.
# Written by Zachary Murray (dremelofdeath@gmail.com)
# Added to project on 01/30/09 20:27:16.

.autodepend

!ifndef CFLAGS
CFLAGS =
!endif
CC = tcc
RM = del

MAKEFILE = Makefile

OBJ = llnodes.obj dllist.obj sllist.obj llist.obj
LIB = llist.lib

all: $(LIB)

clean:
	$(RM) $(LIB)
	$(RM) $(LIB:.lib=.bak)
	$(RM) llnodes.obj
	$(RM) dllist.obj
	$(RM) sllist.obj
	$(RM) llist.obj

$(LIB): $(OBJ) $(MAKEFILE)

.c.obj:
	$(CC) $(CFLAGS) -c $<
	tlib $(LIB) -+$@
