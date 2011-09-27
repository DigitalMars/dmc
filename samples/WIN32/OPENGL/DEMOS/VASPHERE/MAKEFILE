
TARGETOS=WINNT

!include <ntwin32.mak>

opengllibs	    = opengl32.lib glu32.lib glaux.lib

EXES	=  vasphere.exe

all: $(EXES)

.c.exe:
    $(cc) $(cflags) $(cdebug) -DSILENT_WARN $(cvars) $<
    $(link) $(linkdebug) $(guiflags) -subsystem:windows -entry:mainCRTStartup -out:$*.exe $*.obj $(opengllibs) $(guilibs)
