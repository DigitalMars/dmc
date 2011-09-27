# Nmake macros for building Windows 32-Bit apps

TARGETOS=WIN95
APPVER=4.0

!include <win32.mak>

all: comdlg32.exe

# Update the resource if necessary

comdlg32.res: comdlg32.rc comdlg32.h resource.h
    $(rc) $(rcflags) $(rcvars) comdlg32.rc

# Update the object file if necessary

comdlg32.obj: comdlg32.c comdlg32.h
    $(cc) $(cflags) $(cvars) $(cdebug) comdlg32.c

# Update the executable file if necessary, and if so, add the resource back in.

comdlg32.exe: comdlg32.obj comdlg32.res
    $(link) $(linkdebug) $(guiflags) -out:comdlg32.exe comdlg32.obj comdlg32.res $(guilibs)
