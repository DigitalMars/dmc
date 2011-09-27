# Nmake macros for building Win32 applications
!Include <NTWin32.Mak>

# Inference Rules
.C.Obj:
    $(cc) $(cdebug) $(cflags) $(cvars) $*.C
.RC.Res:
    $(rc) $(rcvars) -r -fo $*.Res $*.RC
.Res.Dll:
    $(link) /NODEFAULTLIB -dll /NOENTRY -out:$*.Dll $**
.Obj.Exe:
    $(link) $(ldebug) $(guilflags) -out:$*.Exe $** $(guilibs)

# Macro Defines
PROJ=Main
DLL=The_Dll

# Target List
All: $(PROJ).Exe $(DLL).Dll

# Source Objects
$(PROJ).Obj: $(PROJ).C $(PROJ).H

# Resources
$(DLL).Res: $(DLL).RC $(DLL).Cur $(DLL).Ico $(DLL).Bmp

# Dynamic Link Libraries
$(DLL).Dll: $(DLL).Res

# Executables
$(PROJ).Exe: $(PROJ).Obj

# Clean Up Everything
CleanAll: clean
    @del *.Exe 2> nul
    @del *.Dll 2> nul

# Clean Up Everything Except .Exe And .Dll Files
Clean:
    @del *.Obj 2> nul
    @del *.Res 2> nul
