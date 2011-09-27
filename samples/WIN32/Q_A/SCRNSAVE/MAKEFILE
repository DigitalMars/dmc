# Nmake macros for building Win32 applications
!Include <NTWin32.Mak>

# Inference Rules
.C.Obj:
    $(cc) $(cdebug) $(cflags) $(cvars) $*.C
.RC.Res:
    $(rc) $(rcvars) -r -fo $*.Res $*.RC
.Obj.Scr:
    $(link) $(ldebug) $(savlflags) \
      -out:$*.Scr $** scrnsave.lib $(guilibs)

# Macro Defines
PROJ=Fractal

# Target List
all: $(PROJ).Scr

# Source Objects
$(PROJ).obj: $(PROJ).c $(PROJ).h

# Resources
$(PROJ).res: $(PROJ).rc $(PROJ).h

# Executables
$(PROJ).Scr: $(PROJ).obj $(PROJ).Res

# Clean Up Everything
CleanAll: clean
    del *.Scr

# Clean Up Everything Except .Exe And .Dll Files
Clean:
    del *.Obj
    del *.Res
