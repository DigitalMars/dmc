# Nmake macros for building Windows 32-Bit apps

APPVER=4.0

!include <win32.mak>

!if "$(CPU)" == "i386" # .syms are useful for Win95
SYM = mmcaps32.sym
!endif

all: mmcaps32.exe $(SYM)

LIBS=msacm32.lib vfw32.lib winmm.lib shell32.lib $(int64lib)

OTHERCLOPTS=-DWIN32_LEAN_AND_MEAN -nologo -I. -I..\include
OTHERRCOPTS=-DWIN32_LEAN_AND_MEAN -I. -I..\include


# Update the resource if necessary

mmcaps.res: mmcaps.rc mmcaps.rcv
    $(rc) $(rcflags) $(rcvars) $(OTHERRCOPTS) mmcaps.rc


# Update the object file if necessary

.c.obj:
    $(cc) $(cdebug) $(cflags) $(cvars) $(OTHERCLOPTS) $<

# Update the executable file if necessary, and if so, add the resource back in.

mmcaps32.exe mmcaps32.map:   \
                mmcaps.obj   \
                DEBUG.obj    \
                LOWLEVEL.obj \
                MIDSPIDS.obj \
                ZYZTLB.obj   \
                mmcaps.res
    $(link) $(linkdebug) $(guiflags) -out:mmcaps32.exe $** $(guilibs) $(LIBS) \
        -map:$*.map
    
mmcaps32.sym:	$*.map
	mapsym $*.map

clean:
    @if exist mmcaps32.exe del mmcaps32.exe
    @if exist *.obj del *.obj
    @if exist *.map del *.map
    @if exist *.sym del *.sym
    @if exist *.res del *.res
    @if exist *.pdb del *.pdb
    @if exist *.exp del *.exp
    @if exist *.lib del *.lib
