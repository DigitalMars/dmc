# Nmake macros for building Windows 32-Bit apps

TARGETOS=WINNT

!include <ntwin32.mak>

all: mfmt.exe

mfmt.obj: mfmt.c
    $(cc) $(cflags) $(cvars) mfmt.c

mfmt.exe: mfmt.obj
    $(link) $(conflags) -out:mfmt.exe mfmt.obj $(conlibs)
