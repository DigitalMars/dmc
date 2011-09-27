#+---------------------------------------------------------------------------
#
#  Microsoft Windows
#  Copyright (C) Microsoft Corporation, 1994-1995.
#
#  File:        makefile
#
#----------------------------------------------------------------------------

!include <$(MSTOOLS)\samples\ole\include\olesampl.mak>

#
#       Makefile for OLE Sample MFract
#
#       builds QUAD.DLL: the Quadrant Fractal Engine Helper
#

OLEFLAGS = -I ..\..\idl -I ..\..\..\winhlprs
LINK = $(link)
LINKFLAGS = $(linkdebug) $(dlllflags)
RCFLAGS = -DWIN32

OBJS = quad.obj quadobj.obj
LIBS = $(olelibsdll) ..\..\idl\fguids.lib ..\..\..\winhlprs\winhlprs.lib

all: ..\..\bin\quad.dll

clean:
    -del *.obj
    -del *.map
    -del *.exp
    -del quad.lib
    -del quad.dll
    -del ..\..\bin\quad.dll

quad.obj: quad.cxx            \
        ..\..\idl\frcngn.h    \
        ..\..\idl\frhost.h    \
        ..\..\idl\quadcid.h   \
        ..\..\idl\qudcln.h    \
        ..\..\idl\qudngn.h    \
        quad.h
    $(cc) $(cvars) $(cflags) $(cdebug) $(OLEFLAGS) quad.cxx

quadobj.obj: quadobj.cxx      \
        ..\..\idl\frcngn.h    \
        ..\..\idl\frhost.h    \
        ..\..\idl\qudcln.h    \
        ..\..\idl\qudngn.h    \
        quad.h
    $(cc) $(cvars) $(cflags) $(cdebug) $(OLEFLAGS) quadobj.cxx

quad.dll: $(OBJS)
    $(LINK) @<<
        $(LINKFLAGS)
        -export:DllGetClassObject
        -export:DllCanUnloadNow
        -out:$@
        -map:$*.map
        $(OBJS)
        $(LIBS)
<<

..\..\bin\quad.dll: quad.dll
        copy quad.dll ..\..\bin


