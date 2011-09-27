# Nmake macros for building Windows 32-Bit apps

!include <ntwin32.mak>

all: cliptext.exe

# Update the resource if necessary

cliptext.res: cliptext.rc cliptext.h
    rc -r cliptext.rc

cliptext.rbj: cliptext.res
    cvtres -$(CPU) cliptext.res -o cliptext.rbj

# Update the object file if necessary

cliptext.obj: cliptext.c cliptext.h
    $(cc) $(cflags) $(cvars) $(cdebug) cliptext.c

# Update the executable file if necessary, and if so, add the resource back in.

cliptext.exe: cliptext.obj cliptext.rbj cliptext.def
    $(link) $(linkdebug) $(guiflags) -out:cliptext.exe cliptext.obj cliptext.rbj $(guilibs)
