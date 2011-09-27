# Nmake macros for building Win32 applications
!Include <NTWin32.Mak>

# Inference Rules
.C.Obj:
    $(cc) $(cdebug) $(cflags) $(cvars) $*.C
.Obj.Exe:
    $(link) $(ldebug) $(conlflags) -out:$*.Exe $**\
      user32.lib $(conlibs)

# Macro Defines
PROJ=Inherit
PROJ2=Child

# Target List
All: $(PROJ).Exe $(PROJ2).Exe

# Source Objects
$(PROJ).obj: $(PROJ).c
$(PROJ2).obj: $(PROJ2).c

# Executables
$(PROJ).exe: $(PROJ).obj
$(PROJ2).exe: $(PROJ2).obj

# Clean Up Everything
CleanAll: clean
    @del *.Exe 2> nul

# Clean Up Everything Except .Exe Files
Clean:
    @del *.Obj 2> nul
