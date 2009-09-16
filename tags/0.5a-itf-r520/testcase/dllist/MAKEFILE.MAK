# This is a makefile written for Borland MAKE 3.6.
# Intended to be used with Turbo C++ 3.0.
# Written by Zachary Murray (dremelofdeath@gmail.com)
# Added to project on 02/13/2009 12:46:50 EST.

.autodepend

CFLAGS =
CC = tcc
INCLUDE = -I..\..\ -I..\..\LLIST
RM = del

MAKEFILE = Makefile.mak

OBJ = testdl.obj
DEP = ..\..\llist\llist.lib

all: $(OBJ)

test: $(OBJ:.obj=.exe)

.obj.exe:
	$(CC) $(CFLAGS) $(INCLUDE) $< $(DEP)
	$@

clean:
	$(RM) *.obj
	$(RM) *.exe

.c.obj:
	$(CC) $(CFLAGS) $(INCLUDE) -c $<
