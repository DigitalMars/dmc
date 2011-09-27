# THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
# ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
# PARTICULAR PURPOSE.
#
# Copyright (C) 1992-1995  Microsoft Corporation.  All Rights Reserved.


#
# To execute a build, refer to the following table:
#
#     Build Type                Command line
#     -----------------         ----------------------------
#     32-bit, debug             nmake
#     32-bit, non-debug 	    nmake "nodebug=1"
#
!include <ntwin32.mak>


PROJ = nddechat

all: $(PROJ).exe


#--------------------------------------------------------------------------
# Files in project

PROJ_OBJS  = nddechat.obj nddeutil.obj ddecback.obj connect.obj mem.obj 
BASE_OBJS  = winmain.obj init.obj misc.obj about.obj dispatch.obj
GLOBAL_DEP = globals.h win16ext.h
RC_DEP     = globals.h about.dlg

LIBS32     = user32.lib gdi32.lib kernel32.lib version.lib advapi32.lib
MAPFILE    =


#--------------------------------------------------------------------------
# Dependencies

winmain.obj:  winmain.c  $(GLOBAL_DEP)
init.obj:     init.c     $(GLOBAL_DEP)
misc.obj:     misc.c     $(GLOBAL_DEP)
about.obj:    about.c    $(GLOBAL_DEP)
dispatch.obj: dispatch.c $(GLOBAL_DEP)
nddeutil.obj: nddeutil.c $(GLOBAL_DEP)
connect.obj:  connect.c  $(GLOBAL_DEP)
mem.obj:      mem.c      $(GLOBAL_DEP)
ddecback.obj: ddecback.c $(GLOBAL_DEP)


#--------------------------------------------------------------------------
# Note:  You should not need to change anthing below this line unless you
#        want to modify the compiler options, etc.  For normal project
#        maintenance, all changes are done above.

# 32-build specific build information

#--------------------------------------------------------------------------
# 32-bit Build Rules

$(PROJ).EXE: $(BASE_OBJS) $(PROJ_OBJS) $(PROJ).res
    $(link) @<<
    $(lflags) $(ldebug)
    $(PROJ_OBJS) $(BASE_OBJS)
    /out:$(PROJ).exe
    $(MAPFILE)
    $(PROJ).res
    $(LIBS32)
    $(guilflags)
    $(guilibsdll)
<<


#--------------------------------------------------------------------------
# Inference rules common to 32-bit builds

.c.obj:
    $(cc) @<<
    $(cvarsdll) $(cdebug) $(cflags) $(CDEFINES) $<
<<


#--------------------------------------------------------------------------
# Build rules common 32-bit builds

$(PROJ).res: $(PROJ).rc $(RC_DEP)
    rc $(rcvars) /fo$(PROJ).res $(PROJ).rc


#--------------------------------------------------------------------------
# Rules for cleaning out those old files

clean:
    del *.bak
    del *.pdb
    del *.obj
    del *.res
    del *.exp
    del *.map

cleaner: clean
    del *.exe
    del *.lib
    del *.dll

# End of file -------------------------------------------------------------
