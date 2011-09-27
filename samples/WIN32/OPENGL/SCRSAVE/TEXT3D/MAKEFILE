
TARGETOS=WINNT

!include <win32.mak>

all: sstext3d.scr

#cflags=$(cflags) -DUNICODE -D_UNICODE -DWINVER=0x0400

include_paths = /I../common

.c.obj:
    $(cc) $(include_paths) $(cdebug) $(cflags) $(cvars) $(scall) $*.c

*.obj : *.h

sstext3d.res: sstext3d.rc 
    $(rc) $(include_paths) -r sstext3d.rc

sstext3d.scr: sstext3d.res \
              dialog.obj   \
              sstext3d.obj \
              font.obj     \
              ..\common\sscommon.lib
    $(link) $(linkdebug) $(savlflags) -machine:$(CPU) -out:$*.scr $** $(guilibs) opengl32.lib glu32.lib glaux.lib scrnsave.lib comdlg32.lib comctl32.lib
