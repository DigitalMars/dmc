# Nmake macros for building Windows 32-Bit apps

!include <ntwin32.mak>

all: cursor.exe

# Update the resource if necessary

cursor.res: cursor.rc cursor.h bullseye.cur
    rc -r cursor.rc

cursor.rbj: cursor.res
    cvtres -$(CPU) cursor.res -o cursor.rbj   

# Update the object file if necessary

cursor.obj: cursor.c cursor.h
    $(cc) $(cflags) $(cvars) $(cdebug) cursor.c

# Update the executable file if necessary, and if so, add the resource back in.

cursor.exe: cursor.obj cursor.rbj cursor.def
    $(link) $(linkdebug) $(guiflags) -out:cursor.exe cursor.obj cursor.rbj $(guilibs)
