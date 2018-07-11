
PROGRAM  = wx

CSOURCES = main.c output.c opt.c wxcurl.c geo.c noaa.c
CFLAGS   = $(COPT) $(CDEF) $(CINC)
COPT     = -g
CDEF     = 
CINC     = -I./json -I./strings

LDFLAGS  = $(LDOPT) $(LDDIR) $(LDLIB)
LDOPT    = 
LDDIR    = -L./json -L./strings
LDLIB    = -ldprjson -ldprstrings -lcurl

SUBDIRS  = strings json

include mk.common

main.o  : main.c   wx.h
output.o: output.c wx.h
opt.o   : opt.c    wx.h
geo.o   : geo.c    wx.h
noaa.o  : noaa.c   wx.h
wxcurl.o: wxcurl.c wx.h

run: $(PROGRAM)
	$(DBG) ./$< -g -f krei

dbg:
	@$(MAKE) run DBG="gdb --args"
