# Nmake macros for building Win32 applications
!Include <NTWin32.Mak>

# Suffixes
.SUFFIXES: .Def .RC .MC

# Inference Rules
.C.Obj:
    $(cc) $(cdebug) $(cflags) $(cvars) $*.C
.Def.Lib:
    $(implib) -machine:$(CPU) -def:$*.Def $*.Obj -out:$*.Lib
.Def.Exp:
    $(implib) -machine:$(CPU) -def:$*.Def $*.Obj -out:$*.Lib
.RC.Res:
    $(rc) $(rcvars) -r -fo $*.Res $*.RC
.MC.RC:
    mc $*.mc
.MC.H:
    mc $*.mc
.Obj.Dll:
    $(link) $(ldebug) -NODEFAULTLIB -dll -base:0x60000000 -out:$*.Dll $** $(conlibs)
.Obj.Exe:
    $(link) $(ldebug) $(conlflags) -out:$*.Exe $**\
      user32.lib $(conlibs)

# Macro Defines
PROJ=MsgTest
DLL=Messages

# Target List
All: $(PROJ).Exe $(DLL).Dll

# Source Objects
$(PROJ).Obj: $(PROJ).C $(DLL).H
$(DLL).Obj: $(DLL).C

# Resources
$(DLL).RC $(DLL).H: $(DLL).MC
$(DLL).Res: $(DLL).RC $(DLL).H

# Dynamic Link Libraries
$(DLL).Dll: $(DLL).Obj $(DLL).Res

# Executables
$(PROJ).Exe: $(PROJ).Obj

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
    @del *.RC  2> nul
    @del *.H   2> nul
    @del *.Bin 2> nul
