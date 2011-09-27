# *********************************************************
#  makefile: for WORLD sample
#
#
# *********************************************************

# Nmake macros for building Win32 applications
!Include <NTWin32.Mak>

# Suffixes
.SUFFIXES: .Def

# Inference Rules
.C.Obj:
    $(cc) $(cdebug) $(cflags) $(cvars) $*.C
.RC.Res:
    $(rc) $(rcvars) -r -fo $*.Res $*.RC
.Obj.Exe:
    $(link) $(ldebug) $(guilflags) -out:$*.Exe $** $(guilibs)

# Macro Defines
PROJ=World

# Target List
All: $(PROJ).Exe

# Source Objects
$(PROJ).Obj: $(PROJ).C $(PROJ).H

# Resources
$(PROJ).Res: $(PROJ).RC $(PROJ).Dlg $(PROJ).Ico $(PROJ).H

# Executables
$(PROJ).Exe: $(PROJ).Obj $(PROJ).Res

# Clean Up Everything
CleanAll: clean
    @del *.Exe 2> nul

# Clean Up Everything Except .Exe And .Dll Files
Clean:
    @del *.Obj 2> nul
    @del *.Res 2> nul
