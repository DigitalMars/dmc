
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
#       builds the WINHLPRS.LIB library
#
#       used by MFract and DFView
#

OLEFLAGS = -I ..\idl

all: winhlprs.lib

clean:
    -del *.lib
    -del *.obj

cwindow.obj: cwindow.cxx cwindow.h
    $(cc) $(cvars) $(cflags) $(UFLAGS) $(cdebug) $(OLEFLAGS) cwindow.cxx

cdialog.obj: cdialog.cxx cdialog.h
    $(cc) $(cvars) $(cflags) $(UFLAGS) $(cdebug) $(OLEFLAGS) cdialog.cxx

strmhelp.obj: strmhelp.cxx strmhelp.h
    $(cc) $(cvars) $(cflags) $(UFLAGS) $(cdebug) $(OLEFLAGS) strmhelp.cxx

winhlprs.lib: cwindow.obj cdialog.obj strmhelp.obj
    lib -out:winhlprs.lib cwindow.obj cdialog.obj strmhelp.obj

