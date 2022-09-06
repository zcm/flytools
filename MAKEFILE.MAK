# This is a makefile written for Borland MAKE 3.6.
# Intended to be used with Turbo C++ 3.0.
# Written by Zachary Murray (dremelofdeath@gmail.com)
# Added to project on 01/30/09 23:03:24.

.autodepend

!ifndef CFLAGS
CFLAGS = -DFLYAPIBUILD
!endif

INC = -Iinclude -Isrc -Icompat\turboc

CC = tcc
MAKE = make
RM = del

MAKEFILE = Makefile.mak

DEP = build\dict.obj build\hash.obj build\list.obj \
			build\common.obj build\generics.obj
LIB = build\flytools.lib

all: $(LIB)

clean:
	$(RM) $(LIB)
	$(RM) build\*.obj
	$(RM) $(LIB:.lib=.bak)

$(LIB): $(DEP) compat\turboc\stdint.h $(MAKEFILE)

build\dict.obj: src\dict.c include\dict.h include\hash.h include\common.h src\internal\dict.h
	$(CC) $(CFLAGS) $(INC) -o$@ -c $(*:build=src).c
	tlib $(LIB) -+$@

build\hash.obj: src\hash.c include\hash.h include\common.h
	$(CC) $(CFLAGS) $(INC) -o$@ -c $(*:build=src).c
	tlib $(LIB) -+$@

build\list.obj: src\list.c include\list.h include\common.h include\generics.h
	$(CC) $(CFLAGS) $(INC) -o$@ -c $(*:build=src).c
	tlib $(LIB) -+$@

build\common.obj: src\common.c include\common.h
	$(CC) $(CFLAGS) $(INC) -o$@ -c $(*:build=src).c
	tlib $(LIB) -+$@

build\generics.obj: src\generics.c include\generics.h include\common.h
	$(CC) $(CFLAGS) $(INC) -o$@ -c $(*:build=src).c
	tlib $(LIB) -+$@
