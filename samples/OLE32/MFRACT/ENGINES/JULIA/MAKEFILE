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
#       builds JULIA.DLL: the Julia Set fractal engine.
#

OLEFLAGS = -I ..\..\idl -I ..\..\..\winhlprs
LINK = $(link)
LINKFLAGS = $(linkdebug) $(dlllflags)
RCFLAGS = -DWIN32

OBJS = julia.obj juliaob.obj
LIBS = $(olelibsdll) ..\..\idl\fguids.lib ..\..\..\winhlprs\winhlprs.lib

all: ..\..\bin\julia.dll

clean:
    -del *.obj
    -del *.map
    -del *.exp
    -del julia.res
    -del julia.dll
    -del julia.lib
    -del ..\..\bin\julia.dll

julia.obj: julia.cxx              \
        ..\..\idl\frcngn.h        \
        ..\..\idl\frhost.h        \
        ..\..\idl\juliacid.h      \
        ..\..\idl\qudcln.h        \
        ..\..\idl\qudngn.h        \
        ..\..\..\winhlprs\cdialog.h  \
        julia.h
    $(cc) $(cvars) $(cflags) $(UFLAGS) $(cdebug) $(OLEFLAGS) julia.cxx

juliaob.obj: juliaob.cxx          \
        ..\..\idl\frcngn.h        \
        ..\..\idl\frhost.h        \
        ..\..\idl\juliacid.h      \
        ..\..\idl\quadcid.h       \
        ..\..\idl\qudcln.h        \
        ..\..\idl\qudngn.h        \
        ..\..\..\winhlprs\cdialog.h  \
        ..\..\idl\palsize.h       \
        ..\..\..\winhlprs\strmhelp.h \
        julia.h
    $(cc) $(cvars) $(cflags) $(UFLAGS) $(cdebug) $(OLEFLAGS) juliaob.cxx

julia.res: julia.rc julia.h
    rc $(RCFLAGS) -r -fo$@ julia.rc

julia.dll: $(OBJS) julia.res
    $(LINK) @<<
        $(LINKFLAGS)
        -export:DllGetClassObject
        -export:DllCanUnloadNow
        -out:$@
        -map:$*.map
        $(OBJS)
        julia.res
        $(LIBS)
<<

..\..\bin\julia.dll: julia.dll
        copy julia.dll ..\..\bin


