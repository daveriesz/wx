
PROGRAM  = wx

CSOURCES = main.c geo.c noaa.c
CFLAGS   = $(COPT) $(CDEF) $(CINC)
COPT     = -g
CDEF     = 
CINC     = -I./json

LDFLAGS  = $(LDOPT) $(LDDIR) $(LDLIB)
LDOPT    = 
LDDIR    = 
LDLIB    = 

SUBDIRS  = strings

include mk.common

strings:
	@$(MAKE) -C strings
json:
	@$(MAKE) -C json


	

STRLONG  = 15
STRADV   = 24
ldexec   = printf " + LINK %$(STRLONG)s\r" "$(1)" && printf "\033[$(STRADV)C<== %$(STRLONG)s\n" $(2) && $(CC) -o $(1) $(2) $(3)

.PHONY: strings
