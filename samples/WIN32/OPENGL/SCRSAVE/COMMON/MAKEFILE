TARGETOS=WINNT

!include <win32.mak>

all: sscommon.lib

#cflags=$(cflags) -DUNICODE -D_UNICODE -DWINVER=0x0400

.c.obj:
    $(cc) $(cdebug) $(cflags) $(cvars) $(scall) $*.c

.cxx.obj:
    $(cc) $(cdebug) $(cflags) $(cvars) $(scall) $*.cxx

*.obj : *.h *.hxx

sscommon.res: sscommon.rc 
    $(rc) -r sscommon.rc

sscommon.lib: \
		material.obj \
		color.obj \
		ssdib.obj \
		texture.obj \
		ssimage.obj \
		ssa8.obj \
		math.obj \
		dialog.obj \
		fastdib.obj \
		scrnsave.obj \
		ssinit.obj \
		sswindow.obj \
		sswproc.obj \
		ssutil.obj \
		dlgdraw.obj \
		glscrnsv.obj \
		clear.obj \
		palette.obj \
		util.obj
	$(implib) -machine:$(CPU) -out:$*.lib $** 
