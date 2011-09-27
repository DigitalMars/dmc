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
PROJ=MaskBlt
OBJ1=Track
OBJ2=Bitmap

# Target List
all: $(PROJ).exe

# Source Objects
$(PROJ).obj: $(PROJ).c $(PROJ).h $(OBJ1).h  $(OBJ2).h
$(OBJ1).obj: $(OBJ1).c $(OBJ1).h
$(OBJ2).obj: $(OBJ2).c $(OBJ2).h

# Resources
$(PROJ).res: $(PROJ).rc $(PROJ).h

# Executables
$(PROJ).exe: $(PROJ).obj $(OBJ1).obj $(OBJ2).obj $(PROJ).Res

# Clean Up Everything
CleanAll: clean
    del *.Exe

# Clean Up Everything Except .Exe And .Dll Files
Clean:
    del *.Obj
    del *.Res
