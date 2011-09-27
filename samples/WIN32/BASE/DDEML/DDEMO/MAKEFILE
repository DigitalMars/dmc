# Nmake macros for building Windows 32-Bit apps

!include <ntwin32.mak>

all: ddemo.exe

# Update the object file if necessary

ddemo.obj: ddemo.c 
    $(cc) $(cflags) $(cvars) $(cdebug) ddemo.c

# Update the executable file if necessary, and if so, add the resource back in.

ddemo.exe: ddemo.obj
    $(link) $(linkdebug) $(guiflags) -out:ddemo.exe ddemo.obj $(guilibs)
