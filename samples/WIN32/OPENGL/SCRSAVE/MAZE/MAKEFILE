TARGETOS=WINNT

!include <win32.mak>

all: ssmaze.scr

cflags=$(cflags) -DFX_SINGLE 
#cflags=$(cflags) -DFX_SINGLE -DUNICODE -D_UNICODE -DWINVER=0x0400

HINCLUDES= -I..  -I..\common 

.c.obj:
    $(cc) $(cdebug) $(HINCLUDES) $(cflags) $(cvars) $(scall) $*.c

.cxx.obj:
    $(cc) $(cdebug) $(HINCLUDES) $(cflags) $(cvars) $(scall) $*.cxx

*.obj : *.h 

maze.res: maze.rc 
	$(rc) -r $(HINCLUDES) maze.rc

#ssmaze.scr: maze.res \
ssmaze.scr: \
	genmaze.obj\
	glmaze.obj \
	ssmaze.obj \
	slvmaze.obj\
	dialog.obj \
    maze.res\
	..\common\sscommon.lib
	$(link) $(linkdebug) $(savlflags) -machine:$(CPU) -out:$*.scr $** \
					$(guilibs) opengl32.lib glu32.lib glaux.lib \
					scrnsave.lib comdlg32.lib comctl32.lib 
