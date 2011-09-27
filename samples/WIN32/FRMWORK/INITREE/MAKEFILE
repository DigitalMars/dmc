# THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
# ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
# PARTICULAR PURPOSE.
#
# Copyright (C) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.
#
#
# Processor independent makefile

# Nmake macros for building Windows 32-Bit apps
!include <ntwin32.mak>

PROJ = INITREE

all: $(PROJ).exe

# Define project specific macros
PROJ_OBJS  = ini.obj initree.obj
BASE_OBJS  = winmain.obj init.obj misc.obj about.obj dispatch.obj
EXTRA_LIBS = version.lib comctl32.lib
GLOBAL_DEP = globals.h resource.h
RC_DEP     = resource.h

# Dependencies
winmain.obj:  winmain.c  $(GLOBAL_DEP)
init.obj:     init.c     $(GLOBAL_DEP)
misc.obj:     misc.c     $(GLOBAL_DEP)
about.obj:    about.c    $(GLOBAL_DEP)
dispatch.obj: dispatch.c $(GLOBAL_DEP)
ini.obj:      ini.c      $(GLOBAL_DEP)
initree.obj:  initree.c  $(GLOBAL_DEP)



# Inference rule for updating the object files
.c.obj:
  $(cc) $(cdebug) $(cflags4) $(cvars) $*.c

# Build rule for resource file
$(PROJ).res: $(PROJ).rc $(RC_DEP)
    $(rc) $(rcflags) $(rcvars) /fo $(PROJ).res $(PROJ).rc

# Build rule for EXE
$(PROJ).EXE: $(BASE_OBJS) $(PROJ_OBJS) $(PROJ).res
    $(link) $(linkdebug) $(guilflags4) \
    $(BASE_OBJS) $(PROJ_OBJS) $(PROJ).res $(guilibs) $(EXTRA_LIBS) \
    -out:$(PROJ).exe $(MAPFILE)



# Rules for cleaning out those old files
clean:
    del *.bak *.pdb *.obj *.res *.exp *.map *.sbr *.bsc
