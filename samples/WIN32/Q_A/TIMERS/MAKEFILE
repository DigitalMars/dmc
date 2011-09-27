# Nmake macros for building Win32 applications
!Include <NTWin32.Mak>

# Inference Rules
.C.Obj:
    $(cc) $(cdebug) $(cflags) $(cvars) $*.C
.Obj.Exe:
    $(link) $(ldebug) $(guilflags) -out:$*.Exe $** $(guilibs)

# Macro Defines
PROJ=Timers

# Target List
all: $(PROJ).exe

# Source Objects
$(PROJ).obj: $(PROJ).c $(PROJ).h

# Executables
$(PROJ).exe: $(PROJ).obj

# Clean Up Everything
CleanAll: clean
    @del *.Exe 2> nul

# Clean Up Everything Except .Exe And .Dll Files
Clean:
    @del *.Obj 2> nul
