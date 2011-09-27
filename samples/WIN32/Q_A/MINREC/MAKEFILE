#  ************************************************************************
#
#                       Microsoft Developer Support
#                Copyright (c) 1992-1995 Microsoft Corporation
#
#  ************************************************************************
#  MAKEFILE  : MinRec
#  PURPOSE   : A Small Win32 Recorder-Like Sample Application
#  COMMENTS  :
#  ************************************************************************

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
PROJ=MinRec
DLL=RecHook

# Target List
All: $(PROJ).Exe $(DLL).Dll

# Source Objects
$(PROJ).Obj: $(PROJ).C $(PROJ).H
$(DLL).Obj: $(DLL).C $(DLL).H $(PROJ).H

# Import/Export Libraries
$(DLL).Lib $(DLL).Exp: $(DLL).Def $(DLL).Obj

# Resources
$(PROJ).Res: $(PROJ).RC $(PROJ).Dlg $(PROJ).Ico $(PROJ).H

# Dynamic Link Libraries
$(DLL).Dll: $(DLL).Obj $(DLL).Exp

# Executables
$(PROJ).Exe: $(PROJ).Obj $(DLL).Lib $(PROJ).Res

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
