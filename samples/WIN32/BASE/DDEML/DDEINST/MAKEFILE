# Nmake macros

!include <ntwin32.mak>

all: install.exe

install.res:   install.rc ddedlg.h ddeinst.h dialogs.dlg
   rc -r -fo install.res install.rc

ddemain.obj:   ddemain.c ddeinst.h ddeglbl.h
   $(cc) $(cdebug) $(cflags) $(cvarsmt) ddemain.c

ddeinst.obj:   ddeinst.c ddeinst.h ddextrn.h
   $(cc) $(cdebug) $(cflags) $(cvarsmt) ddeinst.c

ddeprocs.obj:  ddeprocs.c ddeinst.h ddextrn.h
   $(cc) $(cdebug) $(cflags) $(cvarsmt) ddeprocs.c

ddeadd.obj:    ddeadd.c ddeinst.h ddextrn.h dialogs.h
   $(cc) $(cdebug) $(cflags) $(cvarsmt) ddeadd.c

ddedlg.obj:    ddedlg.c ddeinst.h ddextrn.h dialogs.h
   $(cc) $(cdebug) $(cflags) $(cvarsmt) ddedlg.c

install.exe:   ddeinst.obj ddeprocs.obj ddemain.obj ddeadd.obj install.res \
               ddedlg.obj
   $(link) $(linkdebug) $(guiflags) -out:install.exe ddeprocs.obj ddedlg.obj \
   ddeinst.obj ddemain.obj ddeadd.obj install.res $(guilibsmt)
