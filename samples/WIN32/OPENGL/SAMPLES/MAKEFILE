
TARGETOS=WINNT

!include <ntwin32.mak>

opengllibs	= opengl32.lib glu32.lib glaux.lib

EXES	= taccum.exe \
	tbitmap1.exe \
	tbitmap2.exe \
	tcopy.exe \
	tdepth.exe \
	teval.exe \
	tfog.exe \
	tline.exe \
	tlogo.exe \
	tnurb.exe \
	tpoint.exe \
	tprim.exe \
	tquad.exe \
	tselect.exe \
	tspeed.exe \
	tsphere.exe \
	tstar.exe \
	tstencil.exe \
	tstretch.exe \
	ttexture.exe \
	ttri.exe \
	twave.exe

all: $(EXES)

.c.exe:
    $(cc) $(cflags) $(cdebug) $(cvars) $<
    $(link) $(linkdebug) $(guiflags) -subsystem:windows -entry:mainCRTStartup -out:$*.exe $*.obj $(opengllibs) $(guilibs)
