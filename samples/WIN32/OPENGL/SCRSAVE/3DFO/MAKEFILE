TARGETOS=WINNT

!include <win32.mak>

all: ss3dfo.scr

#cflags=$(cflags) -DUNICODE -D_UNICODE -DWINVER=0x0400

HINCLUDES= -I..  -I..\common 

.c.obj:
    $(cc) $(cdebug) $(HINCLUDES) $(cflags) $(cvars) $(scall) $*.c

*.obj : *.h

ss3dfo.res: ss3dfo.rc 
	$(rc) -r $(HINCLUDES) ss3dfo.rc

ss3dfo.scr: ss3dfo.res \
	gendrop.obj\
	genexpld.obj \
	genlem.obj \
	genstrip.obj\
	gentex.obj \
    genwin.obj \
    mesh.obj \
    ss3dfo.obj \
	..\common\sscommon.lib
	$(link) $(linkdebug) $(savlflags) -machine:$(CPU) -out:$*.scr $** \
					$(guilibs) opengl32.lib glu32.lib glaux.lib \
					scrnsave.lib comdlg32.lib comctl32.lib 
