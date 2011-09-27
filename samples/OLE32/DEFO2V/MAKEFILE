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
#       Makefile for OLE Sample DEFO2V32.DLL
#
#       builds DEFO2V32.DLL: viewer DLL used by the OLE2VIEW tool.
#

OLEFLAGS = -I ..\..\idl -I ..\..\winhlprs
LINK = $(link)
LINKFLAGS = $(linkdebug)
RCFLAGS = -DWIN32

OBJS = iadvsink.obj     \
       idataobj.obj     \
       idisp.obj        \
       tofile.obj       \
       util.obj

LIBS = $(olelibsdll)
all: defo2v32.dll

clean:
    -del *.obj
    -del *.exp
    -del *.res
    -del *.dll
    -del *.lib

iadvsink.obj: iadvsink.cpp      \
              precomp.h         \
              defo2v.h          \
              resource.h        \
              util.h            \
              idataobj.h        \
              iadvsink.h
    $(cc) $(cvarsdll) $(cflags) $(cdebug) $(OLEFLAGS) iadvsink.cpp

idataobj.obj: idataobj.cpp       \
              precomp.h          \
              defo2v.h           \
              resource.h         \
              util.h             \
              idataobj.h         \
              iadvsink.h
    $(cc) $(cvarsdll) $(cflags) $(cdebug) $(OLEFLAGS) idataobj.cpp

idisp.obj:   idisp.cpp          \
             precomp.h          \
             defo2v.h           \
             resource.h         \
             util.h             \
             idisp.h            \
             tofile.h
    $(cc) $(cvarsdll) $(cflags) $(cdebug) $(OLEFLAGS) idisp.cpp

tofile.obj:   tofile.cpp        \
              precomp.h         \
              defo2v.h          \
              resource.h        \
              util.h            \
              idisp.h           \
              tofile.h
    $(cc) $(cvarsdll) $(cflags) $(cdebug) $(OLEFLAGS) tofile.cpp

util.obj:   util.cpp    \
            precomp.h   \
            defo2v.h    \
            resource.h  \
            util.h
    $(cc) $(cvarsdll) $(cflags) $(cdebug) $(OLEFLAGS) util.cpp

defo2v.res: defo2v.rc defo2v.h
    rc $(RCFLAGS) -r -fo$@ defo2v.rc

defo2v32.dll: $(OBJS) defo2v.res
    $(LINK) @<<
        $(LINKFLAGS)
        -dll
        -out:$@
        -def:defo2v32.def
        -noentry
        -MERGE:.CRT=.data
        -MERGE:_PAGE=PAGE
        -MERGE:_TEXT=.text
        -SECTION:INIT,d
        -OPT:REF
        -INCREMENTAL:NO
        -IGNORE:4037,4065
        -PDB:NONE
        -NODEFAULTLIB
        -subsystem:native
        $(OBJS)
        defo2v.res
        $(LIBS)
<<

