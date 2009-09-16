# This is a makefile written for Borland MAKE 3.6.
# Intended to be used with Turbo C++ 3.0.
# Written by Zachary Murray (dremelofdeath@gmail.com)
# Added to project on 01/30/09 23:03:24.

.autodepend

!ifndef CFLAGS
CFLAGS =
!endif
CC = tcc
MAKE = make
RM = del

MAKEFILE = Makefile.mak

DEP1 = dict\dict.lib justify\justify.lib
DEP2 = llist\llist.lib memcheck\memcheck.lib
DEP = $(DEP1) $(DEP2)
LIB = flytools.lib

all: $(LIB)

clean:
	cd dict
	$(MAKE) -fmakefile.mak clean
	cd ../justify
	$(MAKE) -fmakefile.mak clean
	cd ../llist
	$(MAKE) -fmakefile.mak clean
	cd ../memcheck
	$(MAKE) -fmakefile.mak clean
	cd ..
	$(RM) $(LIB)
	$(RM) $(LIB:.lib=.bak)

$(LIB): $(DEP) $(MAKEFILE)

dict\dict.lib: dict\dict.c dict\dict.h dict\hash.h
	cd dict
	$(MAKE) -fmakefile.mak $(MFLAGS) -DCFLAGS="$(CFLAGS)"
	cd ..
	tlib $(LIB) -+$@

justify\justify.lib: justify\justify.c justify\justify.h
	cd justify
	$(MAKE) -fmakefile.mak $(MFLAGS) -DCFLAGS="$(CFLAGS)"
	cd ..
	tlib $(LIB) -+$@

llist\llist.lib: llist\llist.c llist\llist.h llist\dllist.c llist\dllist.h llist\sllist.c llist\sllist.h llist\llnodes.c llist\llnodes.h
	cd llist
	$(MAKE) -fmakefile.mak $(MFLAGS) -DCFLAGS="$(CFLAGS)"
	cd ..
	tlib $(LIB) -+$@

memcheck\memcheck.lib: memcheck\memcheck.c memcheck\memcheck.h
	cd memcheck
	$(MAKE) -fmakefile.mak $(MFLAGS) -DCFLAGS="$(CFLAGS)"
	cd ..
	tlib $(LIB) -+$@
