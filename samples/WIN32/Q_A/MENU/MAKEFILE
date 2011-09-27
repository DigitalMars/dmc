# Nmake macros for building Windows 32-Bit apps

!include <ntwin32.mak>

all: menu.exe

# Update the resource if necessary

menu.res: menu.rc menu.h menu.dlg checkon.bmp checkoff.bmp menu.ico
    rc -r menu.rc

menu.rbj: menu.res
    cvtres -$(CPU) menu.res -o menu.rbj

# Update the object file if necessary

menu.obj: menu.c menu.h
    $(cc) $(cflags) $(cvars) $(cdebug) menu.c

# Update the executable file if necessary, and if so, add the resource back in.

menu.exe: menu.obj menu.rbj menu.def
    $(link) $(linkdebug) $(guiflags) -out:menu.exe menu.obj menu.rbj $(guilibs)
