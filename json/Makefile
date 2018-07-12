
PROGRAM = dprjson
LIBRARY = libdprjson.a

CSOURCES = testmain.c
CFLAGS   = $(COPT) $(CDEF) $(CINC)
COPT     = -g
CDEF     = 
CINC     = -I../strings

LDOPT    = 
LDDIR    = -L. -L../strings
LDLIB    = -ldprjson -ldprstrings

CLIBSOURCES = dprjson.c jsondata.c $(LCSOURCES) $(YCSOURCES)

CDEFINES_extra = -DDEBUG_PRINT_STATEMENTS
CINCLUDE_extra = -I../strings

LSOURCES = json.l
YSOURCES = json.y

LDDIRS_extra = -L. -L../strings
LDLIBS_extra = -ldprjson -ldprstrings

INSTALL_LOCAL = install_local

include ../mk.common

dprjson.o: json.tab.h jsondata.h
json.l.c: json.tab.h jsondata.h
json.tab.h json.tab.c: jsondata.h
jsondata.o: debug.h

jsondata.o: jsondata.h

run: $(PROGRAM)
	@printf "********************\n"
	./$< tests/l12.json current_observation.estimated.misc1
	@printf "********************\n"
	./$< tests/l12.json "current_observation.estimated.misc2"
	@printf "********************\n"
	./$< tests/l12.json "current_observation.estimated.misc2[2]"
	@printf "********************\n"
	./$< tests/l12.json "current_observation.estimated.misc2[2].2b"
	@printf "********************\n"
	./$< tests/l12.json "current_observation.estimated.misc2[2].2b[0]"
	@printf "********************\n"

run1: $(PROGRAM)
	./$< tests/001.json

run2: $(PROGRAM)
	./$< tests/002.json

run3: $(PROGRAM)
	./$< tests/003.json

runs: $(PROGRAM)
	./$< tests/sample.json

joex: $(PROGRAM)
	./$< tests/json.org/ex01.json
	./$< tests/json.org/ex02.json
	./$< tests/json.org/ex03.json
	./$< tests/json.org/ex04.json
	./$< tests/json.org/ex05.json

lax: $(PROGRAM)
	./$< tests/lax.json

install_local: $(LIBRARY) $(PROGRAM)
#	@echo " + CP $(PROGRAM) >> $(INSTALLBINDIR)/$(PROGRAM)"
#	cp "$(PROGRAM)" "$(INSTALLBINDIR)/$(PROGRAM)"
