# Nmake macros for building Windows 32-Bit apps

TARGETOS=WINNT

!include <ntwin32.mak>

all: prperf.exe

prperf.obj: prperf.c
    $(cc) $(cflags) $(cvars) prperf.c

prperf.exe: prperf.obj
    $(link) $(conflags) -out:prperf.exe prperf.obj $(conlibs) wsock32.lib
