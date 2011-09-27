# Nmake macros for building Windows 32-Bit apps

!include <ntwin32.mak>

all: ipxchat.exe

# Update the resource if necessary

ipxchat.res: ipxchat.rc globals.h
    rc -r ipxchat.rc

# Update the object file if necessary

ipxchat.obj: ipxchat.c
    $(cc) $(cflags) $(cvars) $(cdebug) ipxchat.c

about.obj: about.c globals.h
    $(cc) $(cflags) $(cvars) $(cdebug) about.c

init.obj: init.c globals.h
    $(cc) $(cflags) $(cvars) $(cdebug) init.c

dispatch.obj: dispatch.c globals.h
    $(cc) $(cflags) $(cvars) $(cdebug) dispatch.c

misc.obj: misc.c globals.h
    $(cc) $(cflags) $(cvars) $(cdebug) misc.c

winmain.obj: winmain.c globals.h
    $(cc) $(cflags) $(cvars) $(cdebug) winmain.c

listen.obj: listen.c globals.h
    $(cc) $(cflags) $(cvars) $(cdebug) listen.c

connect.obj: connect.c globals.h
    $(cc) $(cflags) $(cvars) $(cdebug) connect.c

# Update the executable file if necessary, and if so, add the resource back in.

ipxchat.exe: ipxchat.obj about.obj dispatch.obj init.obj misc.obj winmain.obj listen.obj connect.obj ipxchat.res
    $(link) $(linkdebug) $(guiflags) -out:ipxchat.exe about.obj dispatch.obj init.obj misc.obj winmain.obj listen.obj connect.obj ipxchat.obj ipxchat.res $(guilibs) wsock32.lib version.lib
