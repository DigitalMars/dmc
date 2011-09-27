# Nmake macros for building Windows 32-Bit apps

!include <ntwin32.mak>

all: owncombo.exe

# Update the resource if necessary

owncombo.res: owncombo.rc owncombo.h
    rc -r owncombo.rc

owncombo.rbj: owncombo.res
    cvtres -$(CPU) owncombo.res -o owncombo.rbj

# Update the object file if necessary

owncombo.obj: owncombo.c owncombo.h
    $(cc) $(cflags) $(cvars) $(cdebug) owncombo.c

# Update the executable file if necessary, and if so, add the resource back in.

owncombo.exe: owncombo.obj owncombo.rbj owncombo.def
    $(link) $(linkdebug) $(guiflags) -out:owncombo.exe owncombo.obj owncombo.rbj $(guilibs)
