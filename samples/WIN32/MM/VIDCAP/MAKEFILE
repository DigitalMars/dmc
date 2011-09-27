# Nmake macros for building Windows 32-Bit apps

APPVER=4.0

!include <win32.mak>

!if "$(CPU)" == "i386" # .syms are useful for Win95
SYM = vidcap32.sym
!endif

all: vidcap32.exe $(SYM)

LIBS=msacm32.lib vfw32.lib winmm.lib shell32.lib $(int64lib)

OTHERCLOPTS=-DWIN32_LEAN_AND_MEAN -nologo -I. -I..\include
OTHERRCOPTS=-DWIN32_LEAN_AND_MEAN -I. -I..\include


# Update the resource if necessary

vidcap.res: vidcap.rc vidcap.rcv
    $(rc) $(rcflags) $(rcvars) $(OTHERRCOPTS) vidcap.rc


# Update the object file if necessary

.c.obj:
    $(cc) $(cdebug) $(cflags) $(cvars) $(OTHERCLOPTS) $<

# Update the executable file if necessary, and if so, add the resource back in.

vidcap32.exe vidcap32.map:  \
                vidcap.obj \
                arrow.obj   \
                dialogs.obj \
                help.obj    \
                profile.obj \
                rlmeter.obj \
                status.obj  \
                toolbar.obj \
                vidframe.obj \
                vidcap.res
    $(link) $(linkdebug) $(guiflags) -out:vidcap32.exe $** $(guilibs) $(LIBS) \
         -map:$*.map
    
vidcap32.sym:	$*.map
	mapsym $*.map

clean:
    @if exist vidcap32.exe del vidcap32.exe
    @if exist *.obj del *.obj
    @if exist *.map del *.map
    @if exist *.sym del *.sym
    @if exist *.res del *.res
    @if exist *.pdb del *.pdb
    @if exist *.exp del *.exp
    @if exist *.lib del *.lib
