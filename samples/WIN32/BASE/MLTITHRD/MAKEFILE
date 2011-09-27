# Some nmake macros for building Win32 applications

!include <ntwin32.mak>

all: mltithrd.exe

# Update the object file if necessary

mltithrd.obj: mltithrd.c mltithrd.h
    $(cc) $(cflags) $(cvarsmt) $(cdebug) mltithrd.c

# Update the resources if necessary

mltithrd.res: mltithrd.rc mltithrd.h
    rc -r mltithrd.rc

# Update the executable file if necessary, and if so, add the resource back in.

mltithrd.exe: mltithrd.obj mltithrd.res
    $(link) $(linkdebug) $(guiflags) -out:mltithrd.exe mltithrd.obj mltithrd.res $(guilibsmt)
