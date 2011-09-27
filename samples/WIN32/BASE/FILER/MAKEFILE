# Nmake macros for building FILER
# Define NODEBUG to build FILER without debugging information.
# Define UNICODE to build FILER with UNICODE characters.

TARGETOS=WINNT

!include <ntwin32.mak>

!ifdef UNICODE
unicode = -DUNICODE
!else
unicode =
!endif

#cflags=$(cflags) -DDEVL=1

all: filer.exe

# Update the resource if necessary

filer.res: filer.rc filer.dlg filer.h globals.h *.bmp filer.ico
    $(rc) $(rcvars) -r filer.rc

# Update the object files if necessary

enumdrv.obj: enumdrv.c enumdrv.h globals.h
    $(cc) $(cdebug) $(cflags) $(cvars) $(unicode) enumdrv.c

drvproc.obj: drvproc.c drvproc.h globals.h expdir.h filer.h
    $(cc) $(cdebug) $(cflags) $(cvars) $(unicode) drvproc.c

expdir.obj: expdir.c expdir.h filer.h drvproc.h globals.h
    $(cc) $(cdebug) $(cflags) $(cvars) $(unicode) expdir.c

filer.obj: filer.c filer.h enumdrv.h globals.h
    $(cc) $(cdebug) $(cflags) $(cvars) $(unicode) filer.c

# Update the executable file if necessary, and if so, add the resource back in.

filer.exe: filer.obj enumdrv.obj drvproc.obj expdir.obj filer.res makefile
    $(link) $(linkdebug)  $(guiflags) -out:filer.exe\
    filer.obj enumdrv.obj drvproc.obj expdir.obj filer.res\
    version.lib $(guilibsmt)
