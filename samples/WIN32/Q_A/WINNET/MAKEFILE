# Nmake macros for building Win32 applications
!Include <NTWin32.Mak>

# Inference Rules
.C.Obj:
    $(cc) $(cdebug) $(cflags) $(cvars) $*.C
.Obj.Exe:
    $(link) $(ldebug) $(conlflags) -out:$*.Exe $**\
      mpr.lib $(conlibs) User32.Lib

# Macro Defines
PROJ=WinNet

# Target List
All: $(PROJ).Exe

# Source Objects
$(PROJ).obj: $(PROJ).c

# Executables
$(PROJ).exe: $(PROJ).obj

# Clean Up Everything
CleanAll: clean
    @del *.Exe 2> nul

# Clean Up Everything Except .Exe Files
Clean:
    @del *.Obj 2> nul
