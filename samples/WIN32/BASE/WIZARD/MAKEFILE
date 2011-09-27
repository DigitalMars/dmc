# Nmake macros for building Windows 32-Bit apps

TARGETOS=WIN95
APPVER=4.0

!include <win32.mak>

all: wizard.exe

# Update the resource if necessary

wizard.res: wizard.rc wizard.h resource.h
    $(rc) $(rcflags) $(rcvars) wizard.rc

# Update the object file if necessary

wizard.obj: wizard.c wizard.h
    $(cc) $(cflags) $(cvars) $(cdebug) wizard.c

# Update the executable file if necessary, and if so, add the resource back in.

wizard.exe: wizard.obj wizard.res
    $(link) $(linkdebug) $(guiflags) -out:wizard.exe wizard.obj wizard.res $(guilibs) comctl32.lib
