# Nmake macros for building Windows 32-Bit apps

!include <ntwin32.mak>

all: clock.exe

# Update the resource if necessary

clockres.res: clockres.rc clock.dlg clock.ico
    rc -r clockres.rc

# Update the object file if necessary

clock.obj: clock.c clock.h
    $(cc) $(cflags) $(cvars) $(cdebug) clock.c

# Update the executable file if necessary, and if so, add the resource back in.

clock.exe: clock.obj clockres.res clock.def
    $(link) $(linkdebug) $(guiflags) -out:clock.exe clock.obj clockres.res $(guilibs) advapi32.lib
