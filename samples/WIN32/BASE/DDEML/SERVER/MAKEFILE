# Nmake macros for building Windows 32-Bit apps

!include <ntwin32.mak>

all: server.exe

# Update the resource if necessary

server.res: server.rc
    rc -r server.rc

# Update the object file if necessary

dde.obj: dde.c
    $(cc) $(cflags) $(cvars) $(cdebug) dde.c

ddemlsv.obj: ddemlsv.c
    $(cc) $(cflags) $(cvars) $(cdebug) ddemlsv.c

dialog.obj: dialog.c
    $(cc) $(cflags) $(cvars) $(cdebug) dialog.c

huge.obj: huge.c
    $(cc) $(cflags) $(cvars) $(cdebug) huge.c

server.exe: dde.obj ddemlsv.obj dialog.obj huge.obj server.res server.def
    $(link) $(linkdebug) $(guiflags) -out:server.exe dde.obj ddemlsv.obj dialog.obj huge.obj server.res $(guilibs) advapi32.lib
