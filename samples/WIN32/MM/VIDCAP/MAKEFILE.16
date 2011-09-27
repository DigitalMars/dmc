#===========================================================================;
#
#   MakeFile for the Vidcap32, using AVICap32
#
#
#############################################################################
#
#   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
#   KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
#   IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
#   PURPOSE.
#
#   Copyright (c) 1993 - 1995  Microsoft Corporation.  All Rights Reserved.
#
#############################################################################
#
#   to make a NON-DEBUG build, type the following line:
#       nmake DEBUG=NO
#   just typing 'nmake' will build a DEBUG build
#
#   You can also set the environment variable DEBUG to NO
#
#===========================================================================;

TARGETBASE	=vidcap
EXT             =exe
TARGETNAME	=vidcap32
LOBJ		=win32
LIBS            =libc.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib \
                shell32.lib winmm.lib advapi32.lib vfw32.lib msacm32.lib
OBJ1            =$(LOBJ)\vidcap.obj $(LOBJ)\dialogs.obj $(LOBJ)\arrow.obj 
OBJ2            =$(LOBJ)\rlmeter.obj $(LOBJ)\status.obj $(LOBJ)\help.obj
OBJ3            =$(LOBJ)\profile.obj $(LOBJ)\toolbar.obj $(LOBJ)\vidframe.obj
OBJS            =$(OBJ1) $(OBJ2) $(OBJ3)
GOALS           =$(LOBJ)\$(TARGETNAME).$(EXT) $(LOBJ)\$(TARGETNAME).sym

!if "$(DEBUG)" == "NO"
DEF             =
CDEBUG          =$(DEF)
LDEBUG          =-debug:none
!else
DEF             =-DDEBUG
CDEBUG          =-Zi $(DEF)
LDEBUG          =-debug:full -debugtype:cv
!endif

TARGETWARN      =-W3
TARGETOPT       =-Ox
CFLAGS          =$(TARGETWARN) $(TARGETOPT) -Gs -D_X86_ -DWIN32 $(CDEBUG) -I..\include -Fd$* -Fo$@
RCFLAGS         =-r -I..\include -DWIN32 $(RDEBUG)
LFLAGS          =-nodefaultlib -align:0x1000 $(LDEBUG)
RC              =rc
CC              =cl -c -nologo
LINK            =link /nologo
MAPSYM          =mapsym -nologo
MFLAGS          =

goal:   makedirs $(GOALS)
	@echo ***** Finished making $(TARGETBASE) *****

makedirs:
!if !exists($(LOBJ))
	@md $(LOBJ) >nul
!endif

Clean:
!if exists($(LOBJ))
	@echo y | del $(LOBJ) >nul
	@rd $(LOBJ) >nul
!endif
	@echo ***** Finished Cleaning $(TARGETBASE) *****

$(LOBJ)\vidcap.obj:	$$(@B).c  $$(@B).h
	@$(CC) @<<
$(CFLAGS) $(@B).c
<<

$(LOBJ)\dialogs.obj:	$$(@B).c  $$(@B).h vidcap.h
	@$(CC) @<<
$(CFLAGS) $(@B).c
<<

$(LOBJ)\arrow.obj:	$$(@B).c  $$(@B).h 
	@$(CC) @<<
$(CFLAGS) $(@B).c
<<

$(LOBJ)\profile.obj:	$$(@B).c  $$(@B).h vidcap.h
	@$(CC) @<<
$(CFLAGS) $(@B).c
<<

$(LOBJ)\toolbar.obj:	$$(@B).c  $$(@B).h vidcap.h
	@$(CC) @<<
$(CFLAGS) $(@B).c
<<

$(LOBJ)\vidframe.obj:	$$(@B).c  $$(@B).h vidcap.h
	@$(CC) @<<
$(CFLAGS) $(@B).c
<<

$(LOBJ)\rlmeter.obj:	$$(@B).c  $$(@B).h vidcap.h
	@$(CC) @<<
$(CFLAGS) $(@B).c
<<

$(LOBJ)\status.obj:	$$(@B).c  $$(@B).h vidcap.h
	@$(CC) @<<
$(CFLAGS) $(@B).c
<<

$(LOBJ)\help.obj:	$$(@B).c  $$(@B).h vidcap.h
	@$(CC) @<<
$(CFLAGS) $(@B).c
<<


$(LOBJ)\$(TARGETNAME).sym:      $$(@R).map
	@$(MAPSYM) $(MFLAGS) -mo $@ $*.map

$(LOBJ)\$(TARGETBASE).res: $$(@B).rc $$(@B).rcv $$(@B).h
	@$(RC) $(RCFLAGS) -fo$@ $(@B).rc

$(LOBJ)\$(TARGETNAME).$(EXT) $(LOBJ)\$(TARGETNAME).map: $(OBJS) $$(@D)\$(TARGETBASE).res 
	@$(LINK) $(LFLAGS) @<<
-out:$(@R).$(EXT)
-machine:i386
-subsystem:windows,4.0
-map:$(@R).map
$(@D)\$(TARGETBASE).res
$(LIBS)
$(OBJS)
<<
