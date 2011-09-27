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
#       builds MANDEL.DLL: the Mandelbrot Set fractal engine
#

OLEFLAGS = -I ..\..\idl -I ..\..\..\winhlprs
LINK = $(link)
LINKFLAGS = $(linkdebug) $(dlllflags)
RCFLAGS = -DWIN32

OBJS = mandel.obj mandelob.obj
LIBS = $(olelibsdll) ..\..\idl\fguids.lib ..\..\..\winhlprs\winhlprs.lib

all: ..\..\bin\mandel.dll

clean:
    -del *.obj
    -del *.map
    -del *.exp
    -del mandel.res
    -del mandel.dll
    -del mandel.lib
    -del ..\..\bin\mandel.dll

mandel.obj: mandel.cxx            \
        ..\..\idl\frcngn.h        \
        ..\..\idl\frhost.h        \
        ..\..\idl\mandcid.h       \
        ..\..\idl\qudcln.h        \
        ..\..\idl\qudngn.h        \
        ..\..\..\winhlprs\cdialog.h  \
        mandel.h
    $(cc) $(cvars) $(cflags) $(UFLAGS) $(cdebug) $(OLEFLAGS) mandel.cxx

mandelob.obj: mandelob.cxx        \
        ..\..\idl\frcngn.h        \
        ..\..\idl\frhost.h        \
        ..\..\idl\mandcid.h       \
        ..\..\idl\quadcid.h       \
        ..\..\idl\qudcln.h        \
        ..\..\idl\qudngn.h        \
        ..\..\..\winhlprs\cdialog.h  \
        ..\..\idl\palsize.h       \
        ..\..\..\winhlprs\strmhelp.h \
        mandel.h
    $(cc) $(cvars) $(cflags) $(UFLAGS) $(cdebug) $(OLEFLAGS) mandelob.cxx

mandel.res: mandel.rc mandel.h
    rc $(RCFLAGS) -r -fo$@ mandel.rc

mandel.dll: $(OBJS) mandel.res
    $(LINK) @<<
        $(LINKFLAGS)
        -export:DllGetClassObject
        -export:DllCanUnloadNow
        -out:$@
        -map:$*.map
        $(OBJS)
        mandel.res
        $(LIBS)
<<

..\..\bin\mandel.dll: mandel.dll
        copy mandel.dll ..\..\bin


