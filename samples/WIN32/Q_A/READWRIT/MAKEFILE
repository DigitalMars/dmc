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
.Obj.Dll:
    $(link) $(ldebug) $(dlllflags) -out:$*.Dll $** $(conlibs)
.Obj.Exe:
    $(link) $(ldebug) $(conlflags) -out:$*.Exe $** $(conlibs) user32.lib

# Macro Defines
PROJ=ReadWrit
DLL=DataBase

# Target List
All: $(PROJ).Exe $(DLL).Dll

# Source Objects
$(PROJ).Obj: $(PROJ).C $(DLL).H
$(DLL).Obj: $(DLL).C $(DLL).H

# Import/Export Libraries
$(DLL).Lib $(DLL).Exp: $(DLL).Def $(DLL).Obj

# Dynamic Link Libraries
$(DLL).Dll: $(DLL).Obj $(DLL).Exp

# Executables
$(PROJ).Exe: $(PROJ).Obj $(DLL).Lib

# Clean Up Everything
CleanAll: clean
    @del *.Exe 2> nul
    @del *.Dll 2> nul

# Clean Up Everything Except .Exe And .Dll Files
Clean:
    @del *.Obj 2> nul
    @del *.Lib 2> nul
    @del *.Exp 2> nul
