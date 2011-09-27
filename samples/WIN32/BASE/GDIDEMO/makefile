# Some nmake macros for Win32 apps development

!include <ntwin32.mak>

# application specific C defines

cf = -DNT -DWIN -DNOT_IMPLEMENTED

# This line allows NMAKE to work as well

all: gdidemo.exe

# Update the object file if necessary

gdidemo.obj: gdidemo.c draw.h gdidemo.h maze.h poly.h xform.h
    $(cc) $(cflags) $(cvars) $(cdebug) $(cf) gdidemo.c

init.obj: init.c draw.h gdidemo.h maze.h poly.h xform.h
    $(cc) $(cflags) $(cvars) $(cdebug) $(cf) init.c

dialog.obj: dialog.c gdidemo.h
    $(cc) $(cflags) $(cvars) $(cdebug) $(cf) dialog.c

wininfo.obj: wininfo.c gdidemo.h
    $(cc) $(cflags) $(cvars) $(cdebug) $(cf) wininfo.c

poly.obj: poly.c gdidemo.h poly.h
    $(cc) $(cflags) $(cvars) $(cdebug) $(cf) poly.c

xform.obj: xform.c gdidemo.h xform.h
    $(cc) $(cflags) $(cvars) $(cdebug) $(cf) xform.c

maze.obj: maze.c gdidemo.h maze.h
    $(cc) $(cflags) $(cvars) $(cdebug) $(cf) maze.c

draw.obj: draw.c draw.h gdidemo.h
    $(cc) $(cflags) $(cvars) $(cdebug) $(cf) draw.c

bounce.obj: bounce.c bounce.h gdidemo.h
    $(cc) $(cflags) $(cvars) $(cdebug) $(cf) bounce.c

# Update the resources if necessary

gdidemo.res: gdidemo.rc gdidemo.h
    rc -r gdidemo.rc

# Update the executable file if necessary, and if so, add the resource back in.

gdidemo.exe: gdidemo.obj init.obj bounce.obj dialog.obj wininfo.obj \
	     poly.obj xform.obj maze.obj draw.obj gdidemo.res gdidemo.def
    $(link) $(linkdebug) $(guiflags) -out:gdidemo.exe   \
    gdidemo.obj bounce.obj init.obj dialog.obj wininfo.obj poly.obj \
    xform.obj maze.obj draw.obj gdidemo.res $(guilibs)
