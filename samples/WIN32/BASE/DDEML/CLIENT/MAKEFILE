# Nmake macros for building Windows 32-Bit apps

!include <ntwin32.mak>

all: client.exe

# Update the resource if necessary

client.res: client.rc
    rc -r client.rc

# Update the object file if necessary

clinit.obj: clinit.c
    $(cc) $(cflags) $(cvars) $(cdebug) clinit.c

dde.obj: dde.c
    $(cc) $(cflags) $(cvars) $(cdebug) dde.c

ddemlcl.obj: ddemlcl.c
    $(cc) $(cflags) $(cvars) $(cdebug) ddemlcl.c

dialog.obj: dialog.c
    $(cc) $(cflags) $(cvars) $(cdebug) dialog.c

huge.obj: huge.c
    $(cc) $(cflags) $(cvars) $(cdebug) huge.c

infoctrl.obj: infoctrl.c
    $(cc) $(cflags) $(cvars) $(cdebug) infoctrl.c

mem.obj: mem.c
    $(cc) $(cflags) $(cvars) $(cdebug) mem.c

track.obj: track.c
    $(cc) $(cflags) $(cvars) $(cdebug) track.c

client.exe: clinit.obj dde.obj ddemlcl.obj dialog.obj huge.obj infoctrl.obj mem.obj track.obj client.res client.def
    $(link) $(linkdebug) $(guiflags) -out:client.exe clinit.obj dde.obj ddemlcl.obj dialog.obj huge.obj infoctrl.obj mem.obj track.obj client.res $(guilibs)
