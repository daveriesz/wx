
PROGRAM = stringtest
LIBRARY = libdprstrings.a

CSOURCES    = stringtest.c
CLIBSOURCES = dprstrings.c

LDOPT    = 
LDDIR    = -L.
LDLIB    = -ldprstrings

INSTALL_LOCAL = install_local

include ../mk.common

dprstrings.o: dprstrings.h


run: $(PROGRAM)
	@$(DBG) ./$<

dbg:
	@$(MAKE) run DBG="gdb --args"
