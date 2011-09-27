# Nmake macros for building Win32 applications
!Include <NTWin32.Mak>

# Suffixes
.SUFFIXES: .Def

# Inference Rules
.C.Obj:
    $(cc) $(cdebug) $(cflags) $(cvars) $*.C
.Def.Lib:
    $(implib) -machine:$(CPU) -def:$*.Def $*.Obj -out:$*.Lib
.Def.Exp:
    $(implib) -machine:$(CPU) -def:$*.Def $*.Obj -out:$*.Lib
.RC.Res:
    $(rc) $(rcvars) -r -fo $*.Res $*.RC
.Obj.Dll:
    $(link) $(ldebug) $(dlllflags) -out:$*.Dll $** $(guilibs)
.Obj.Exe:
    $(link) $(ldebug) $(guilflags) -out:$*.Exe $** $(guilibs)

# Macro Defines
PROJ1=LoadTest
PROJ2=LinkTest
DLL=The_Dll

# Target List
All: $(PROJ1).Exe $(PROJ2).Exe $(DLL).Dll

# Source Objects
$(PROJ1).Obj: $(PROJ1).C $(PROJ1).H $(DLL).H
$(PROJ2).Obj: $(PROJ2).C $(PROJ2).H  $(DLL).H
$(DLL).Obj: $(DLL).C $(DLL).H

# Import/Export Libraries
$(DLL).Lib $(DLL).Exp: $(DLL).Def $(DLL).Obj

# Resources
$(PROJ1).Res: $(PROJ1).RC $(PROJ1).H
$(PROJ2).Res: $(PROJ2).RC $(PROJ2).H
$(DLL).Res: $(DLL).RC

# Dynamic Link Libraries
$(DLL).Dll: $(DLL).Obj $(DLL).Exp $(DLL).Res

# Executables
$(PROJ1).Exe: $(PROJ1).Obj $(DLL).Lib $(PROJ1).Res
$(PROJ2).Exe: $(PROJ2).Obj $(DLL).Lib $(PROJ2).Res

# Clean Up Everything
CleanAll: clean
    @del *.Exe 2> nul
    @del *.Dll 2> nul

# Clean Up Everything Except .Exe And .Dll Files
Clean:
    @del *.Obj 2> nul
    @del *.Lib 2> nul
    @del *.Exp 2> nul
    @del *.Res 2> nul
