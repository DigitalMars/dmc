# Nmake macros for building Windows 32-Bit apps for OpenGL

!include <ntwin32.mak>

opengllibs	= opengl32.lib glu32.lib glaux.lib

all: jtext.exe

.c.exe:
    $(cc) $(cflags) $(cdebug) $(cvars) $<
    $(link) $(linkdebug) $(guiflags) -subsystem:windows -entry:mainCRTStartup -out:$*.exe $*.obj $(opengllibs) $(guilibs)
