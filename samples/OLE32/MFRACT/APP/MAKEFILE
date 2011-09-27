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
#       builds MFract.EXE
#

OLEFLAGS = -I ..\idl -I ..\..\winhlprs
LINK = $(link)
LINKFLAGS = $(linkdebug) $(guilflags)
RCFLAGS = -DWIN32

OBJS = fractal.obj fclass.obj engnlist.obj about.obj chngdim.obj dib.obj
LIBS = $(olelibsmt) ..\idl\fguids.lib ..\..\winhlprs\winhlprs.lib
#LIBS = $(olelibsmt) kernel32.lib user32.lib advapi32.lib ..\idl\fguids.lib ..\..\winhlprs\winhlprs.lib

all: ..\bin\MFract.exe

clean:
    -del *.obj
    -del fractal.res
    -del MFract.map
    -del MFract.exe
    -del ..\bin\MFract.exe

fractal.obj: fractal.cxx        \
        fractal.h               \
        ..\idl\frcngn.h         \
        ..\idl\frhost.h         \
        ..\idl\palsize.h        \
        ..\..\winhlprs\cwindow.h
    $(cc) $(cvars) $(cflags) $(UFLAGS) $(cdebug) $(OLEFLAGS) fractal.cxx

fclass.obj: fclass.cxx          \
        fclass.h                \
        fractal.h               \
        ..\idl\frcngn.h         \
        ..\idl\frhost.h         \
        ..\idl\palsize.h        \
        ..\..\winhlprs\cwindow.h   \
        ..\..\winhlprs\cdialog.h   \
        about.h                 \
        chngdim.h               \
        dib.h
    $(cc) $(cvars) $(cflags) $(UFLAGS) $(cdebug) $(OLEFLAGS) fclass.cxx

engnlist.obj: engnlist.cxx      \
        engnlist.h              \
        fractal.h               \
        ..\idl\plasmcid.h       \
        ..\idl\juliacid.h       \
        ..\idl\mandcid.h
    $(cc) $(cvars) $(cflags) $(UFLAGS) $(cdebug) $(OLEFLAGS) engnlist.cxx

about.obj: about.cxx about.h ..\..\winhlprs\cdialog.h
    $(cc) $(cvars) $(cflags) $(UFLAGS) $(cdebug) $(OLEFLAGS) about.cxx

chngdim.obj: chngdim.cxx        \
        chngdim.h               \
        ..\idl\frcngn.h         \
        ..\idl\frhost.h         \
        ..\idl\palsize.h        \
        ..\..\winhlprs\cdialog.h   \
        ..\..\winhlprs\cwindow.h
    $(cc) $(cvars) $(cflags) $(UFLAGS) $(cdebug) $(OLEFLAGS) chngdim.cxx

dib.obj: dib.c dib.h
    $(cc) $(cvars) $(cflags) $(UFLAGS) $(cdebug) $(OLEFLAGS) dib.c

fractal.res: fractal.rc fractal.ico fractal.h
    rc $(RCFLAGS) -r -fo$@ fractal.rc

MFract.exe: $(OBJS) fractal.res
    $(LINK) @<<
        $(LINKFLAGS)
        -out:$@
        -map:$*.map
        $(OBJS)
        fractal.res
        $(LIBS)
<<

..\bin\MFract.exe: MFract.exe
        copy MFract.exe ..\bin


