
PROGRAM  = wx

CSOURCES = main.c output.c opt.c units.c wxcurl.c geo.c noaa.c
CDEF     = 
CINC     = -I./json -I./strings

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
units.o : units.c  wx.h

run: $(PROGRAM)
	$(DBG) ./$< -g -c New Haven, CT

dbg:
	@$(MAKE) run DBG="gdb --args"
