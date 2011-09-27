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
#       Makefile for OLE Sample MFRACT
#
#       builds the FGUIDS.LIB library.
#

MFLAGS  = /ms_ext /c_ext /cpp_cmd $(cc) /Zp4
OLEFLAGS =

all: fguids.lib

clean:
    -del frhost.h
    -del frhost_i.c
    -del frcngn.h
    -del frcngn_i.c
    -del qudngn.h
    -del qudngn_i.c
    -del qudcln.h
    -del qudcln_i.c
    -del *.lib
    -del *.obj

frhost.h frhost_i.c:frhost.idl
    midl $(MFLAGS) $**

frcngn.h frcngn_i.c:frcngn.idl
    midl $(MFLAGS) $**

qudngn.h qudngn_i.c:qudngn.idl
    midl $(MFLAGS) $**

qudcln.h qudcln_i.c:qudcln.idl
    midl $(MFLAGS) $**

frhost_i.obj: frhost_i.c frhost.h
    $(cc) $(cvars) $(cflags) $(UFLAGS) $(cdebug) $(OLEFLAGS) frhost_i.c

frcngn_i.obj: frcngn_i.c frcngn.h
    $(cc) $(cvars) $(cflags) $(UFLAGS) $(cdebug) $(OLEFLAGS) frcngn_i.c

qudngn_i.obj: qudngn_i.c qudngn.h
    $(cc) $(cvars) $(cflags) $(UFLAGS) $(cdebug) $(OLEFLAGS) qudngn_i.c

qudcln_i.obj: qudcln_i.c qudcln.h
    $(cc) $(cvars) $(cflags) $(UFLAGS) $(cdebug) $(OLEFLAGS) qudcln_i.c

mandcid.obj: mandcid.cxx mandcid.h
    $(cc) $(cvars) $(cflags) $(UFLAGS) $(cdebug) $(OLEFLAGS) mandcid.cxx

juliacid.obj: juliacid.cxx juliacid.h
    $(cc) $(cvars) $(cflags) $(UFLAGS) $(cdebug) $(OLEFLAGS) juliacid.cxx

plasmcid.obj: plasmcid.cxx plasmcid.h
    $(cc) $(cvars) $(cflags) $(UFLAGS) $(cdebug) $(OLEFLAGS) plasmcid.cxx

rgencid.obj: rgencid.cxx rgencid.h
    $(cc) $(cvars) $(cflags) $(UFLAGS) $(cdebug) $(OLEFLAGS) rgencid.cxx

quadcid.obj: quadcid.cxx quadcid.h
    $(cc) $(cvars) $(cflags) $(UFLAGS) $(cdebug) $(OLEFLAGS) quadcid.cxx

fguids.lib: frcngn_i.obj qudcln_i.obj qudngn_i.obj frhost_i.obj \
            mandcid.obj juliacid.obj plasmcid.obj quadcid.obj \
            rgencid.obj
    lib -out:fguids.lib frcngn_i.obj qudcln_i.obj qudngn_i.obj frhost_i.obj \
                        mandcid.obj juliacid.obj plasmcid.obj quadcid.obj\
                        rgencid.obj
