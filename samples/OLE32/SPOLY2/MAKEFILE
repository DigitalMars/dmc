##############################################################################
#
#  (c) Copyright Microsoft Corp. 1992-1993 All Rights Reserved
#
#  File:
#
#    makefile - makefile for spoly2.exe
#
#  Purpose:
#
#    Builds the OLE 2.0 sample IDispatch server, spoly2.exe.
#
#
#  Usage:
#
#     NMAKE                     ; build with defaults
#     or: NMAKE option          ; build with the given option(s)
#     or: NMAKE clean           ; erase all compiled files
#
#     option:
#         dev = [win16 | win32] ; dev=win32 is the default
#         DEBUG=[0|1]           ; DEBUG=1 is the default
#
#  Notes:
#
#    This makefile assumes that the PATH, INCLUDE and LIB environment
#    variables are setup properly.
#
##############################################################################



##########################################################################
#
# Default Settings
#

!if "$(dev)" == ""
dev = win32
!endif

!if !("$(dev)" == "win16" || "$(dev)" == "win32")
!error Invalid dev option, choose from [win16 | win32]
!endif

!if "$(dev)" == "win16"
TARGET  = WIN16
!endif

!if "$(dev)" == "win32"
TARGET  = WIN32
!endif

!ifdef NODEBUG
DEBUG = 0
!endif

!if "$(DEBUG)" == "0"
NODEBUG = 1
!endif

!if "$(DEBUG)" == ""
DEBUG = 1
!endif


##########################################################################
#
# WIN16 Settings
#
!if "$(TARGET)" == "WIN16"

CC   = cl
LINK = link

RCFLAGS = -dWIN16
CFLAGS = -c -W3 -AM -GA -GEs -DWIN16
LINKFLAGS = /NOD /NOI /BATCH /ONERROR:NOEXE

LIBS = libw.lib mlibcew.lib

!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS) -Od -Zi -D_DEBUG $(CL)
LINKFLAGS = $(LINKFLAGS) /COD
!else
CFLAGS = $(CFLAGS) -Ox $(CL)
LINKFLAGS = $(LINKFLAGS) /FAR /PACKC
!endif
!endif


##########################################################################
#
# WIN32 Settings
#
!if "$(TARGET)" == "WIN32"

!include <$(MSTOOLS)\samples\ole\include\olesampl.mak>

CC = $(cc)
CFLAGS = $(cflags) $(cvarsmt) -DINC_OLE2 $(cdebug)

!ifndef NODEBUG
CFLAGS = $(CFLAGS) -D_DEBUG
!endif

LINK = $(link)
LINKFLAGS = $(linkdebug) $(guilflags)
RCFLAGS = -DWIN32

!endif


##########################################################################
#
# Application Settings
#

!if "$(TARGET)" == "WIN16"
LIBS = ole2.lib compobj.lib ole2disp.lib $(LIBS)
!else
!if "$(TARGET)" == "WIN32"
LIBS = $(olelibsmt)
!endif
!endif

OBJS = \
        winmain.obj     \
        cpoly.obj       \
        cpoint.obj      \
        cenumpt.obj     \
        statbar.obj     \
        clsid.obj       \
        misc.obj        \
        tdata.obj

goal : setflags spoly2.exe

setflags :
        set CL=$(CFLAGS)

clean :
    if exist *.obj      del *.obj
    if exist spoly2.exe del spoly2.exe
    if exist spoly2.map del spoly2.map
    if exist spoly2.res del spoly2.res
    if exist *.pdb       del *.pdb


##########################################################################
#
# Application Build (WIN16 Specific)
#

!if "$(TARGET)" == "WIN16"

spoly2.exe : $(OBJS) spoly2.def spoly2.res spoly2.ico
        link $(LINKFLAGS) @<<
$(OBJS),
$@,,
$(LIBS),
spoly2.def
<<
        rc -k -t spoly2.res $@
!endif


##########################################################################
#
# Application Build (WIN32 Specific)
#
!if "$(TARGET)" == "WIN32"

spoly2.exe : $(OBJS) spoly2.def spoly2.res spoly2.ico
      $(LINK) @<<
        $(LINKFLAGS)
        -out:$@
        -map:$*.map
        $(OBJS)
        spoly2.res
        $(LIBS)
<<
!endif


spoly2.res : spoly2.rc resource.h
        rc $(RCFLAGS) -r -fo$@ spoly2.rc

winmain.obj: winmain.cpp hostenv.h resource.h spoly.h statbar.h
        $(CC) winmain.cpp

cpoint.obj: cpoint.cpp cpoint.h hostenv.h spoly.h statbar.h
        $(CC) cpoint.cpp

cpoly.obj: cpoly.cpp cpoint.h cpoly.h hostenv.h spoly.h statbar.h
        $(CC) cpoly.cpp

clsid.obj: clsid.c clsid.h
        $(CC) clsid.c

cenumpt.obj: cenumpt.cpp cenumpt.h
        $(CC) cenumpt.cpp

statbar.obj: statbar.cpp statbar.h
        $(CC) statbar.cpp

misc.obj: misc.cpp hostenv.h spoly.h
        $(CC) misc.cpp

tdata.obj: tdata.cpp
        $(CC) tdata.cpp

