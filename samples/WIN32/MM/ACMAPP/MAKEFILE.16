#===========================================================================;
#
#   ACMApp -- Audio Compression Manager Sample Application
#
#
#############################################################################
#
#   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
#   KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
#   IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
#   PURPOSE.
#
#   Copyright (c) 1992 - 1995  Microsoft Corporation.  All Rights Reserved.
#
#############################################################################
#
#   to make a NON-DEBUG build, type the following line:
#       nmake DEBUG=NO
#   just typing 'nmake' will build a DEBUG build
#
#   You can also set the environment variable DEBUG to NO
#
#   to make a 16-bit build, type the following line:
#       nmake WIN16=TRUE
#   just typing 'nmake' will build a 32-bit build
#
#   You can also set the environment variable WIN16 to TRUE
#
#===========================================================================;

TARGETBASE	=acmapp
EXT		=exe
!if "$(WIN16)" == "TRUE"
TARGETNAME	=$(TARGETBASE)16
TARGETDEF       =$(TARGETNAME).def
LOBJ		=win16
LIBS		=libw mlibcew commdlg shell mmsystem
!else
TARGETNAME	=$(TARGETBASE)32
TARGETDEF       =
LOBJ		=win32
LIBS		=libc.lib user32.lib kernel32.lib gdi32.lib comdlg32.lib shell32.lib winmm.lib
!endif
OBJ1		=$(LOBJ)\aadrvs.obj $(LOBJ)\aafile.obj $(LOBJ)\aainit.obj $(LOBJ)\aaplyrec.obj
OBJ2		=$(LOBJ)\aaprops.obj $(LOBJ)\aasysinf.obj $(LOBJ)\acmapp.obj $(LOBJ)\acmthunk.obj $(LOBJ)\debug.obj
OBJ3		=$(LOBJ)\waveio.obj $(LOBJ)\tlb.obj $(LOBJ)\aawavdev.obj $(LOBJ)\aachoose.obj
OBJS		=$(OBJ1) $(OBJ2) $(OBJ3)
GOALS		=$(LOBJ)\$(TARGETNAME).$(EXT) $(LOBJ)\$(TARGETNAME).sym

!if "$(DEBUG)" == "NO"
DEF		=
CDEBUG		=$(DEF)
RDEBUG		=$(DEF)
!if "$(WIN16)" == "TRUE"
LDEBUG		=
!else
LDEBUG		=-debug:none
!endif
!else
DEF		=-DDEBUG
CDEBUG		=-Zi $(DEF)
RDEBUG		=$(DEF)
!if "$(WIN16)" == "TRUE"
LDEBUG		=/CO/LI
!else
LDEBUG		=-debug:full -debugtype:cv
!endif
!endif

TARGETWARN	=-W3
TARGETOPT	=-Ox
!if "$(WIN16)" == "TRUE"
SEG		=-NT S
CFLAGS		=$(TARGETWARN) $(TARGETOPT) -Z7dp -AM -GsA3 $(CDEBUG) -I..\include -Fo$@
RCFLAGS		=-z -r -I..\include $(RDEBUG)
LFLAGS		=/L/MAP/NOPACKCODE/NOE/NOD/ONERROR:NOEXE/ALIGN:16$(LDEBUG)
RESFLAGS	=-t
RC		=rc -nologo
!else
SEG		=
CFLAGS		=$(TARGETWARN) $(TARGETOPT) -Gs -D_X86_ -DWIN32 $(CDEBUG) -I..\include -Fd$* -Fo$@
RCFLAGS		=-r -I..\include -DWIN32 $(RDEBUG)
LFLAGS		=-nodefaultlib -align:0x1000 $(LDEBUG)
RC		=rc
!endif
CC		=cl -c -nologo
LINK		=link /nologo
MAPSYM		=mapsym -nologo
MFLAGS		=

goal:	makedirs $(GOALS)
	@echo ***** Finished making $(TARGETBASE) *****

makedirs:
!if !exists($(LOBJ))
	@md $(LOBJ) >nul
!endif

clean:
!if exists($(LOBJ))
	@echo y | del $(LOBJ) >nul
	@rd $(LOBJ) >nul
!endif
	@echo ***** Finished cleaning $(TARGETBASE) *****

$(LOBJ)\aachoose.obj: $$(@B).c acmapp.h 
	@$(CC) $(CFLAGS) $(SEG:S=ACMAPP_TEXT) $(@B).c

$(LOBJ)\aadrvs.obj: $$(@B).c acmapp.h appport.h debug.h tlb.h
	@$(CC) $(CFLAGS) $(SEG:S=ACMAPP_TEXT) $(@B).c

$(LOBJ)\aafile.obj: $$(@B).c acmapp.h appport.h debug.h waveio.h
	@$(CC) $(CFLAGS) $(SEG:S=ACMAPP_TEXT) $(@B).c

$(LOBJ)\aainit.obj: $$(@B).c acmapp.h appport.h debug.h
	@$(CC) $(CFLAGS) $(SEG:S=ACMAPP_TEXT) $(@B).c

$(LOBJ)\aaplyrec.obj: $$(@B).c acmapp.h appport.h debug.h
	@$(CC) $(CFLAGS) $(SEG:S=ACMAPP_TEXT) $(@B).c

$(LOBJ)\aaprops.obj: $$(@B).c acmapp.h appport.h debug.h
	@$(CC) $(CFLAGS) $(SEG:S=ACMAPP_TEXT) $(@B).c

$(LOBJ)\aasysinf.obj: $$(@B).c acmapp.h appport.h debug.h
	@$(CC) $(CFLAGS) $(SEG:S=ACMAPP_TEXT) $(@B).c

$(LOBJ)\aawavdev.obj: $$(@B).c acmapp.h 
	@$(CC) $(CFLAGS) $(SEG:S=ACMAPP_TEXT) $(@B).c

$(LOBJ)\acmapp.obj: $$(@B).c acmapp.h appport.h debug.h
	@$(CC) $(CFLAGS) $(SEG:S=ACMAPP_TEXT) $(@B).c

$(LOBJ)\acmthunk.obj: $$(@B).c 
	@$(CC) $(CFLAGS) $(SEG:S=ACMAPP_TEXT) $(@B).c

$(LOBJ)\debug.obj:  $$(@B).c debug.h
	@$(CC) $(CFLAGS) $(SEG:S=_TEXT) $(@B).c

$(LOBJ)\tlb.obj: $$(@B).c appport.h debug.h tlb.h
	@$(CC) $(CFLAGS) $(SEG:S=_TEXT) $(@B).c

$(LOBJ)\waveio.obj: $$(@B).c appport.h debug.h waveio.h
	@$(CC) $(CFLAGS) $(SEG:S=ACMAPP_TEXT) $(@B).c

$(LOBJ)\$(TARGETNAME).sym:	$$(@R).map
	@$(MAPSYM) $(MFLAGS) -mo $@ $*.map

$(LOBJ)\$(TARGETBASE).res: $$(@B).rc $$(@B).rcv $$(@B).h
	@$(RC) $(RCFLAGS) -fo$@ $(@B).rc

$(LOBJ)\$(TARGETNAME).$(EXT) $(LOBJ)\$(TARGETNAME).map: $(OBJS) $$(@D)\$(TARGETBASE).res $(TARGETDEF)
	@$(LINK) $(LFLAGS) @<<
!if "$(WIN16)" == "TRUE"
$(OBJ1)+
$(OBJ2)+
$(OBJ3),
$(@R).$(EXT),
$(@R).map,
$(LIBS),
$(@B).def
<<
	@$(RC) $(RESFLAGS) $(@D)\$(TARGETBASE).res $*.$(EXT)
!else
-out:$(@R).$(EXT)
-machine:i386
-subsystem:windows
-map:$(@R).map
$(@D)\$(TARGETBASE).res
$(LIBS)
$(OBJS)
<<
!endif
