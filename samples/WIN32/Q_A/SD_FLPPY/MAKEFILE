# Nmake macros for building Win32 applications
!Include <NTWin32.Mak>

# Inference Rules
.C.Obj:
    $(cc) $(cdebug) $(cflags) $(cvarsmt) $*.C
.Obj.Exe:
    $(link) $(ldebug) $(conlflags) -out:$*.Exe $**\
      advapi32.lib $(conlibsmt) User32.Lib

# Macro Defines
PROJ1=SD_Flppy
PROJ2=FlopLock
PROJ3=InstSrv
PROJ4=ChgFlpSD

# Target List
All: $(PROJ1).Exe $(PROJ2).Exe $(PROJ3).Exe $(PROJ4).Exe

# Source Objects
$(PROJ1).obj: $(PROJ1).c
$(PROJ2).obj: $(PROJ2).c
$(PROJ3).obj: $(PROJ3).c
$(PROJ4).obj: $(PROJ4).c

# Executables
$(PROJ1).exe: $(PROJ1).obj
$(PROJ2).exe: $(PROJ2).obj
$(PROJ3).exe: $(PROJ3).obj
$(PROJ4).exe: $(PROJ4).obj

# Clean Up Everything
CleanAll: clean
    del *.Exe

# Clean Up Everything Except .Exe Files
Clean:
    del *.Obj
