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
#       builds PLASMA.DLL: the Plasma Cloud Fractal engine
#

OLEFLAGS = -I ..\..\idl -I ..\..\..\winhlprs
LINK = $(link)
LINKFLAGS = $(linkdebug) $(dlllflags)
RCFLAGS = -DWIN32

OBJS = plasma.obj plasmaob.obj
LIBS = $(olelibsdll) ..\..\idl\fguids.lib ..\..\..\winhlprs\winhlprs.lib

all: ..\..\bin\plasma.dll

clean:
    -del *.obj
    -del *.map
    -del *.exp
    -del plasma.res
    -del plasma.dll
    -del plasma.lib
    -del ..\..\bin\plasma.dll

plasma.obj: plasma.cxx            \
        ..\..\idl\frcngn.h        \
        ..\..\idl\frhost.h        \
        ..\..\idl\plasmcid.h      \
        ..\..\..\winhlprs\cdialog.h  \
        plasma.h
    $(cc) $(cvars) $(cflags) $(UFLAGS) $(cdebug) $(OLEFLAGS) plasma.cxx

plasmaob.obj: plasmaob.cxx       \
        ..\..\idl\frcngn.h       \
        ..\..\idl\frhost.h       \
        ..\..\idl\palsize.h      \
        ..\..\idl\plasmcid.h     \
        ..\..\..\winhlprs\cdialog.h \
        plasma.h
    $(cc) $(cvars) $(cflags) $(UFLAGS) $(cdebug) $(OLEFLAGS) plasmaob.cxx

plasma.res: plasma.rc plasma.dlg plasma.h
    rc $(RCFLAGS) -r -fo$@ plasma.rc

plasma.dll: $(OBJS) plasma.res
    $(LINK) @<<
        $(LINKFLAGS)
        -export:DllGetClassObject
        -export:DllCanUnloadNow
        -out:$@
        -map:$*.map
        $(OBJS)
        plasma.res
        $(LIBS)
<<

..\..\bin\plasma.dll: plasma.dll
        copy plasma.dll ..\..\bin


