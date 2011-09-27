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
#     32-bit, non-debug 	nmake "nodebug=1"
#
!include <ntwin32.mak>


PROJ_DLL = servenum

all: $(PROJ_DLL).dll $(PROJ_DLL).lib


#--------------------------------------------------------------------------
# Files in PROJ_DLL project

PROJ_DLL_OBJS  = enumdlg.obj enumthrd.obj enumutil.obj thrdutil.obj \
		 mem.obj servenum.obj miscutil.obj dlldisp.obj
RC_DEP	       = enumdlg.dlg

LIBS32	       = mpr.lib netapi32.lib

CDEFINES       = /D"UNICODE" /D"_UNICODE"
MAPFILE        = /map:$(PROJ_DLL).map


#--------------------------------------------------------------------------
# Dependencies

dlldisp.obj:   dlldisp.c dlldisp.h
enumdlg.obj:   enumdlg.c  enumdlg.h enumutil.h
servenum.obj:  servenum.c servenum.h enumutil.h
enumthrd.obj:  enumthrd.c enumthrd.h enumutil.h
enumutil.obj:  enumutil.c enumutil.h mem.h
thrdutil.obj:  thrdutil.c thrdutil.h mem.h
miscutil.obj:  miscutil.c miscutil.h mem.h
mem.obj:       mem.c mem.h

#--------------------------------------------------------------------------
# Note:  You should not need to change anthing below this line unless you
#        want to modify the compiler options, etc.  For normal project
#        maintenance, all changes are done above.


#--------------------------------------------------------------------------
# 32-bit Build Rules

$(PROJ_DLL).DLL: $(PROJ_DLL_OBJS) $(PROJ_DLL).res $(PROJ_DLL).exp
    $(link) @<<
    $(lflags) $(ldebug)
    /out:$(PROJ_DLL).dll
    $(PROJ_DLL_OBJS)
    $(MAPFILE)
    $(PROJ_DLL).res $(PROJ_DLL).exp
    $(LIBS32)
    $(dlllflags) $(guilibsdll)
<<


#--------------------------------------------------------------------------
# Inference rules

.c.obj:
    $(cc) @<<
    $(cvarsdll) $(cflags) $(cdebug) $(CDEFINES) $<
<<


#--------------------------------------------------------------------------
# Build rules common


$(PROJ_DLL).res: $(PROJ_DLL).rc $(RC_DEP)
    rc $(rcvars) /fo$(PROJ_DLL).res $(PROJ_DLL).rc

$(PROJ_DLL).exp $(PROJ_DLL).lib: $(PROJ_DLL).obj $(PROJ_DLL).def
    lib $(PROJ_DLL).obj -machine:$(CPU) -def:$(PROJ_DLL).def -out:$(PROJ_DLL).lib


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
