
TARGETOS=WINNT

!include <win32.mak>


all: ssflwbox.scr

#cflags=$(cflags) -DUNICODE -D_UNICODE -DWINVER=0x0400

include_paths = /I../common

.c.obj:
    $(cc) $(include_paths) $(cdebug) $(cflags) $(cvars) $(scall) $*.c

*.obj : *.h

ssflwbox.res: ssflwbox.rc 
    $(rc) $(include_paths) -r ssflwbox.rc

ssflwbox.scr: \
              ssflwbox.res      \
			  geom.obj	        \
              ssflwbox.obj	    \
			  ..\common\sscommon.lib
    $(link) $(linkdebug) $(savlflags) -machine:$(CPU) -out:$@ $** $(guilibs) opengl32.lib glu32.lib glaux.lib scrnsave.lib comdlg32.lib comctl32.lib
