# Nmake macros for building Windows 32-Bit apps

APPVER=4.0

!include <win32.mak>

!if "$(CPU)" == "i386" # .syms are useful for Win95
SYM = mixapp.sym
!endif

all: mixapp.exe $(SYM)

OBJS=  	mixapp.obj mainit.obj malines.obj mactrls.obj \
       	mameter.obj maswitch.obj mafader.obj malist.obj tlb.obj \
       	debug.obj macustom.obj manumber.obj maslider.obj matime.obj

OTHERCLOPTS=-DWIN32_LEAN_AND_MEAN -nologo -I. -I..\include
OTHERRCOPTS=-DWIN32_LEAN_AND_MEAN -I. -I..\include


# Update the resource if necessary

mixapp.res: mixapp.rc mixapp.h mixapp.rcv mixapp.ico
    $(rc) -r -DWIN32 $(OTHERRCOPTS) mixapp.rc


# Update the object file if necessary

mainit.obj: mainit.c mixapp.h debug.h
    $(cc) $(cdebug) $(cflags) $(cvars) $(OTHERCLOPTS) mainit.c

mactrls.obj: mactrls.c mixapp.h debug.h
    $(cc) $(cdebug) $(cflags) $(cvars) $(OTHERCLOPTS) mactrls.c

malines.obj: malines.c mixapp.h debug.h
    $(cc) $(cdebug) $(cflags) $(cvars) $(OTHERCLOPTS) malines.c

mameter.obj: mameter.c muldiv32.h mixapp.h debug.h
    $(cc) $(cdebug) $(cflags) $(cvars) $(OTHERCLOPTS) mameter.c

maswitch.obj: maswitch.c mixapp.h debug.h
    $(cc) $(cdebug) $(cflags) $(cvars) $(OTHERCLOPTS) maswitch.c

mixapp.obj: mixapp.c mixapp.h debug.h
    $(cc) $(cdebug) $(cflags) $(cvars) $(OTHERCLOPTS) mixapp.c

debug.obj: debug.c debug.h
    $(cc) $(cdebug) $(cflags) $(cvars) $(OTHERCLOPTS) debug.c

malist.obj: malist.c mixapp.h debug.h
    $(cc) $(cdebug) $(cflags) $(cvars) $(OTHERCLOPTS) malist.c

tlb.obj: tlb.c tlb.h
    $(cc) $(cdebug) $(cflags) $(cvars) $(OTHERCLOPTS) tlb.c

mafader.obj: mafader.c muldiv32.h mixapp.h debug.h
    $(cc) $(cdebug) $(cflags) $(cvars) $(OTHERCLOPTS) mafader.c

macustom.obj: macustom.c
    $(cc) $(cdebug) $(cflags) $(cvars) $(OTHERCLOPTS) macustom.c

manumber.obj: manumber.c
    $(cc) $(cdebug) $(cflags) $(cvars) $(OTHERCLOPTS) manumber.c

maslider.obj: maslider.c
    $(cc) $(cdebug) $(cflags) $(cvars) $(OTHERCLOPTS) maslider.c

matime.obj: matime.c
    $(cc) $(cdebug) $(cflags) $(cvars) $(OTHERCLOPTS) matime.c


# Update the executable file if necessary, and if so, add the resource back in.

mixapp.exe mixapp.map:  $(OBJS) mixapp.res
    $(link) $(linkdebug) $(guiflags) -out:mixapp.exe $(OBJS) mixapp.res \
        $(guilibs) $(int64lib) winmm.lib -map:$*.map
    
mixapp.sym:	$*.map
	mapsym $*.map

clean:
    @if exist mixapp.exe del mixapp.exe
    @if exist *.obj del *.obj
    @if exist *.map del *.map
    @if exist *.sym del *.sym
    @if exist *.res del *.res
    @if exist *.pdb del *.pdb
    @if exist *.exp del *.exp
    @if exist *.lib del *.lib
