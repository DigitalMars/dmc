# Nmake macros for building Windows 32-Bit apps

!include <ntwin32.mak>

all: showdib.exe

# Update the resource if necessary

showdib.res: showdib.rc showdib.h
    rc -r showdib.rc

# Update the object file if necessary

showdib.obj: showdib.c showdib.h
    $(cc) $(cflags) $(cvars) $(cdebug) showdib.c

print.obj: print.c showdib.h
    $(cc) $(cflags) $(cvars) $(cdebug) print.c

dlgopen.obj: dlgopen.c showdib.h
    $(cc) $(cflags) $(cvars) $(cdebug) dlgopen.c

dib.obj: dib.c showdib.h
    $(cc) $(cflags) $(cvars) $(cdebug) dib.c

drawdib.obj: drawdib.c showdib.h
    $(cc) $(cflags) $(cvars) $(cdebug) drawdib.c

# Update the executable file if necessary, and if so, add the resource back in.

showdib.exe: showdib.obj print.obj dib.obj dlgopen.obj drawdib.obj showdib.res showdib.def
    $(link) $(linkdebug) $(guiflags) -out:showdib.exe showdib.obj print.obj dib.obj dlgopen.obj drawdib.obj showdib.res $(guilibs)
