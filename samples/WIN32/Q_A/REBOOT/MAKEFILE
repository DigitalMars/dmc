#  ************************************************************************
#
#                       Microsoft Developer Support
#                Copyright (c) 1994-1995 Microsoft Corporation
#
#  ************************************************************************
#  MAKEFILE  : Reboot
#  PURPOSE   : A Reboot Win32 Console Application Make File
#  COMMENTS  :
#  ************************************************************************

# Nmake macros for building Win32 applications
!Include <NTWin32.Mak>

# Inference Rules
.C.Obj:
    $(cc) $(cdebug) $(cflags) $(cvars) $*.C
.Obj.Exe:
    $(link) $(ldebug) $(conlflags) -out:$*.Exe $**\
      advapi32.lib user32.lib $(conlibs)

# Macro Defines
PROJ=Reboot

# Target List
All: $(PROJ).Exe

# Source Objects
$(PROJ).Obj: $(PROJ).C

# Executables
$(PROJ).Exe: $(PROJ).Obj

# Clean Up Everything
CleanAll: clean
    del *.Exe

# Clean Up Everything Except .Exe Files
Clean:
    del *.Obj
