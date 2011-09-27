
TARGETOS=WINNT

!include <win32.mak>

all: sspipes.scr

#cflags=$(cflags) -DUNICODE -D_UNICODE -DWINVER=0x0400

include_paths = /I../common

.c.obj:
    $(cc) $(include_paths) $(cdebug) $(cflags) $(cvars) $(scall) $*.c

.cxx.obj:
    $(cc) $(include_paths) $(cdebug) $(cflags) $(cvars) $(scall) $*.cxx

*.obj : *.h 

sspipes.res: sspipes.rc 
    $(rc) $(include_paths) -r sspipes.rc

sspipes.scr: \
		   dialog.obj  \
           sspipes.obj \
           state.obj   \
           nstate.obj  \
           fstate.obj  \
           pipe.obj    \
           npipe.obj   \
           fpipe.obj   \
           view.obj    \
           node.obj    \
           objects.obj \
           eval.obj    \
           xc.obj      \
           sspipes.res \
		   ..\common\sscommon.lib
	$(link) $(linkdebug) $(savlflags) -machine:$(CPU) -out:$*.scr $** $(guilibs) opengl32.lib glu32.lib glaux.lib scrnsave.lib comdlg32.lib comctl32.lib
