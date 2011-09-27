# Nmake macros for building Windows 32-Bit apps

!include <ntwin32.mak>

all: scrnsave.lib

cparrow.obj: cparrow.c
    $(cc) $(cflags) $(cvars) $(cdebug) $(scall) cparrow.c

skeleton.obj: skeleton.c
    $(cc) $(cflags) $(cvars) $(cdebug) $(scall) skeleton.c

scrnsave.lib: skeleton.obj cparrow.obj
    $(implib) -machine:$(CPU) -out:scrnsave.lib skeleton.obj cparrow.obj
