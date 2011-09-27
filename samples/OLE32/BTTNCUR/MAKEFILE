#
# Makefile
# Standard Button Images and Cursors DLL Version 1.1, March 1993
#
#
!include <$(MSTOOLS)\samples\ole\include\olesampl.mak>

all: bttncur.dll

OLE_FLAGS = -DWIN32S

!ifndef NO_DEBUG
OLE_FLAGS = $(OLE_FLAGS) /D_DEBUG /DDEBUG /D_DEBUGTRACE=0
!endif

.SUFFIXES: .h .c .obj .exe .res .rc

#
#


INCLS    = ..\include\bttncur.h bttncuri.h
OBJS     = bttncur.obj cursors.obj

RCFILES1 = bttncur.rcv res\harrows.cur res\help.cur res\larrows.cur
RCFILES2 = res\magnify.cur res\neswarrs.cur res\nodrop.cur res\nwsearrs.cur
RCFILES3 = res\rarrow.cur res\sarrows.cur res\sizebarh.cur res\sizebarv.cur
RCFILES4 = res\splith.cur res\splitv.cur res\tabletop.cur res\varrows.cur
RCFILES5 = res\stdim72.bmp res\stdim96.bmp res\stdim120.bmp
RCFILES  = $(RCFILES1) $(RCFILES2) $(RCFILES3) $(RCFILES4) $(RCFILES5)


#
# Tool Directives
#
.c.obj:
    $(cc) $(cflags) $(cvars) $(cdebug) $(OLE_FLAGS) $*.c

.rc.res:
    rc -r -DWIN32 -DDEBUG $*.rc

clean:
    -del *.obj
    -del *.dll
    -del *.res
    -del *.lib
    -del *.exp
    -del *.map

bttncur.dll: $(OBJS) bttncur.lib bttncur.def bttncur.res
    $(link) $(linkdebug) $(dlllflags) bttncur.exp $(OBJS) $*.res -out:$@ -map:$*.map $(guilibsdll) -base:0x76000000
    if not exist ..\bin mkdir ..\bin
    copy bttncur.dll ..\bin

bttncur.lib: $(OBJS) bttncur.def
    $(implib) $(OBJS) -out:bttncur.lib -def:bttncur.def -machine:$(CPU)
    if not exist ..\lib mkdir ..\lib
    copy bttncur.lib ..\lib

##### Dependencies #####
bttncur.obj  : bttncur.c    $(INCLS)
cursors.obj  : cursors.c    $(INCLS)
bttncur.res  : bttncur.rc   $(INCLS) $(RCFILES)
