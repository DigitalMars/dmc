# Nmake macros for building Win32 applications
!Include <NTWin32.Mak>

# Inference Rules
.C.Obj:
    $(cc) $(cdebug) $(cflags) $(cvars) $*.C
.RC.Res:
    $(rc) $(rcvars) -r -fo $*.Res $*.RC
.Obj.Exe:
    $(link) $(ldebug) $(guilflags) -out:$*.Exe $** $(guilibs)

# Macro Defines
PROJ=Platform

# Target List
all: $(PROJ).exe

# Source Objects
$(PROJ).obj: $(PROJ).c $(PROJ).h

# Resources
$(PROJ).res: $(PROJ).rc $(PROJ).h

# Executables
$(PROJ).exe: $(PROJ).obj $(PROJ).Res

# Clean Up Everything
CleanAll: clean
    del *.Exe

# Clean Up Everything Except .Exe And .Dll Files
Clean:
    del *.Obj
    del *.Res
