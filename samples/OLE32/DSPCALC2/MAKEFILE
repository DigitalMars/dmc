####
#makefile - makefile for dspcalc2.exe
#
#       Copyright (C) 1992-1995, Microsoft Corporation
#
#Purpose:
#  Builds the OLE 2.0 sample IDispatch server, dspcalc2.exe.
#
#
#  Usage: NMAKE                 ; build with defaults
#     or: NMAKE option          ; build with the given option(s)
#     or: NMAKE clean           ; erase all compiled files
#
#     option: dev = [win16 | win32]    ; dev=win32 is the default
#             DEBUG=[0|1]              ; DEBUG=1 is the default
#             HOST=[DOS | NT]          ; HOST=DOS (for win16)
#                                      ; HOST=NT  (for win32)
#
#Notes:
#  This makefile assumes that the PATH, INCLUDE and LIB environment
#  variables are setup properly.
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
!if "$(HOST)" == ""
HOST  = DOS
!endif
!endif

!if "$(dev)" == "win32"
TARGET  = WIN32
HOST  = NT
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

!if "$(HOST)" == "DOS"
WX   = wx /w
!else
WX   =
!endif

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
CFLAGS = $(cflags) $(cvarsmt) $(cdebug)

!ifndef NODEBUG
CFLAGS = $(CFLAGS) -D_DEBUG
!endif

LINK = $(link)
LINKFLAGS = $(linkdebug) $(guilflags)
RCFLAGS = -DWIN32

WX =

!endif


##########################################################################
#
# Build rules
#

.cpp.obj:
    @echo Compiling $<...
    $(CC) $<

.c.obj:
    @echo Compiling $<...
    $(CC) $<


##########################################################################
#
# Application Settings
#

APPS = dspcalc2


!if "$(TARGET)" == "WIN16"
LIBS = ole2.lib compobj.lib ole2disp.lib typelib.lib $(LIBS)
!endif
!if "$(TARGET)" == "WIN32"
LIBS = $(olelibsmt)
!endif

OBJS = \
        main.obj        \
        dspcalc2.obj    \
        clsid.obj


##########################################################################
#
# Default Goal
#

goal : setflags $(APPS).exe

setflags :
        set CL=$(CFLAGS)


##########################################################################
#
# Clean (erase) generated files
#
clean :
    if exist *.obj       del *.obj
    if exist $(APPS).exe del $(APPS).exe
    if exist $(APPS).tlb del $(APPS).tlb
    if exist $(APPS).map del $(APPS).map
    if exist $(APPS).res del $(APPS).res
    if exist calctype.h  del calctype.h
    if exist *.log       del *.log
    if exist *.pdb       del *.pdb


##########################################################################
#
# Application Build (WIN16 Specific)
#

!if "$(TARGET)" == "WIN16"
$(APPS).exe : $(OBJS) $(APPS).def $(APPS).res $(APPS).ico
        link $(LINKFLAGS) @<<
$(OBJS),
$@,,
$(LIBS),
$(APPS).def
<<
        rc -k -t $(APPS).res $@
!endif


##########################################################################
#
# Application Build (WIN32 Specific)
#
!if "$(TARGET)" == "WIN32"
$(APPS).exe : $(OBJS) $(APPS).def $(APPS).res $(APPS).ico
      $(LINK) @<<
        $(LINKFLAGS)
        -out:$@
        -map:$*.map
        $(OBJS)
        $(APPS).res
        $(LIBS)
<<
!endif


##########################################################################
#
# Application Build (Common)
#

$(APPS).res : $(APPS).rc
        rc $(RCFLAGS) -r -fo$@ $?


##########################################################################
#
# Dependencies
#

calctype.h : calctype.odl
        if exist calctype.h  del calctype.h
        if exist dspcalc2.tlb  del dspcalc2.tlb
        $(WX) mktyplib /D$(TARGET) /h calctype.h /o calctype.log /tlb dspcalc2.tlb calctype.odl

main.obj : main.cpp dspcalc2.h calctype.h
        $(CC) main.cpp

dspcalc2.obj : dspcalc2.cpp dspcalc2.h calctype.h
        $(CC) dspcalc2.cpp

clsid.obj : clsid.c clsid.h
        $(CC) clsid.c
