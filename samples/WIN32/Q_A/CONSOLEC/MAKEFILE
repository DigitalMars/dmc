#************************************************************************
#
#                      Microsoft Developer Support
#             Copyright (c) 1992-1995 Microsoft Corporation
#
#************************************************************************
#
# MAKEFILE  : Color
#
# COMMENTS  :
#
#************************************************************************

# Nmake macros for building Win32 applications
!Include <NTWin32.Mak>

# Inference Rules
.C.Obj:
    $(cc) $(cdebug) $(cflags) $(cvars) $*.C
.Obj.Exe:
    $(link) $(ldebug) $(conlflags) -out:$*.Exe $**\
      User32.Lib $(conlibs)

# Macro Defines
PROJ=Color

# Target List
All: $(PROJ).Exe

# Source Objects
$(PROJ).obj: $(PROJ).c $(PROJ).H

# Executables
$(PROJ).exe: $(PROJ).obj

# Clean Up Everything
CleanAll: clean
    @del *.Exe 2> nul

# Clean Up Everything Except .Exe Files
Clean:
    @del *.Obj 2> nul
