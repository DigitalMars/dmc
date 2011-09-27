# Nmake macros for building Windows 32-Bit apps

APPVER=4.0

!include <win32.mak>

!if "$(CPU)" == "i386" # .syms are useful for Win95
SYM = midipl32.sym
!endif

all: midipl32.exe $(SYM)

LIBS=msacm32.lib vfw32.lib winmm.lib shell32.lib comctl32.lib $(int64lib)

OTHERCLOPTS=-DWIN32_LEAN_AND_MEAN -nologo -I. -I..\include
OTHERRCOPTS=-DWIN32_LEAN_AND_MEAN -I. -I..\..\include


# Update the resource if necessary

midiplyr.res: midiplyr.rc midiplyr.rcv
    $(rc) $(rcflags) $(rcvars) $(OTHERRCOPTS) midiplyr.rc


# Update the object file if necessary

.c.obj:
    $(cc) $(cdebug) $(cflags) $(cvars) $(OTHERCLOPTS) $<

# Update the executable file if necessary, and if so, add the resource back in.

midipl32.exe midipl32.map:   DEBUG.obj    \
                MAINWND.obj  \
                MIDIPLYR.obj \
                SEQUENCE.obj \
                SMF.obj      \
                SMFREAD.obj  \
                TIMEWND.obj  \
                UIUTILS.obj  \
                midiplyr.res
    $(link) $(linkdebug) $(guiflags) -out:midipl32.exe $** $(guilibs) $(LIBS) \
         -map:$*.map
    
midipl32.sym:	$*.map
	mapsym $*.map

clean:
    @if exist midipl32.exe del midipl32.exe
    @if exist *.obj del *.obj
    @if exist *.map del *.map
    @if exist *.sym del *.sym
    @if exist *.res del *.res
    @if exist *.pdb del *.pdb
    @if exist *.exp del *.exp
    @if exist *.lib del *.lib
