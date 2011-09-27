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
#       builds rgen.DLL: the rgen Cloud Fractal engine
#

OLEFLAGS = -I ..\..\idl -I ..\..\..\winhlprs
LINK = $(link)
LINKFLAGS = $(linkdebug) $(dlllflags)
RCFLAGS = -DWIN32

OBJS = rgen.obj rgenob.obj polar.obj genwin.obj
LIBS = $(olelibsdll) ..\..\idl\fguids.lib ..\..\..\winhlprs\winhlprs.lib

all: ..\..\bin\rgen.dll

clean:
    -del *.obj
    -del *.map
    -del *.exp
    -del rgen.res
    -del rgen.dll
    -del rgen.lib
    -del ..\..\bin\rgen.dll

rgen.obj: rgen.cxx                \
        ..\..\idl\frcngn.h        \
        ..\..\idl\frhost.h        \
        ..\..\idl\rgencid.h       \
        ..\..\..\winhlprs\cdialog.h  \
        ..\..\..\winhlprs\cwindow.h  \
        rgen.h
    $(cc) $(cvars) $(cflags) $(UFLAGS) $(cdebug) $(OLEFLAGS) rgen.cxx

rgenob.obj: rgenob.cxx           \
        ..\..\idl\frcngn.h       \
        ..\..\idl\frhost.h       \
        ..\..\idl\palsize.h      \
        ..\..\idl\rgencid.h      \
        ..\..\..\winhlprs\cdialog.h \
        ..\..\..\winhlprs\cwindow.h \
        polar.h                  \
        genwin.h                 \
        rgen.h
    $(cc) $(cvars) $(cflags) $(UFLAGS) $(cdebug) $(OLEFLAGS) rgenob.cxx

polar.obj: polar.cxx             \
        polar.h
    $(cc) $(cvars) $(cflags) $(UFLAGS) $(cdebug) $(OLEFLAGS) polar.cxx

genwin.obj: genwin.cxx           \
        ..\..\..\winhlprs\cwindow.h \
        polar.h                  \
        genwin.h                 \
        rgen.h                   \
        polar.h
    $(cc) $(cvars) $(cflags) $(UFLAGS) $(cdebug) $(OLEFLAGS) genwin.cxx

rgen.res: rgen.rc rgen.dlg rgen.h choose.cur
    rc $(RCFLAGS) -r -fo$@ rgen.rc

rgen.dll: $(OBJS) rgen.res
    $(LINK) @<<
        $(LINKFLAGS)
        -export:DllGetClassObject
        -export:DllCanUnloadNow
        -out:$@
        -map:$*.map
        $(OBJS)
        rgen.res
        $(LIBS)
<<

..\..\bin\rgen.dll: rgen.dll
        copy rgen.dll ..\..\bin


