# Nmake macros for building Windows 32-Bit apps

!include <win32.mak>

all: reversi.exe

# Update the resource if necessary

reversi.res: reversi.rc reversi.h
    $(rc) $(rcflags) $(rcvars) reversi.rc

# Update the object file if necessary

rev.obj: rev.c reversi.h
    $(cc) $(cflags) $(cvars) $(cdebug) rev.c

reversi.obj: reversi.c reversi.h
    $(cc) $(cflags) $(cvars) $(cdebug) reversi.c

# Update the executable file if necessary, and if so, add the resource back in.

reversi.exe: reversi.obj reversi.res rev.obj
    $(link) $(linkdebug) $(guilflags) -out:reversi.exe reversi.obj rev.obj reversi.res $(guilibs)
