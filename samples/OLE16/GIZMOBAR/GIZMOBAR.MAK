ORIGIN		= Symantec C++
ORIGIN_VER	= Version 7.22
VERSION		= DEBUG

!IFDEF SUB_DEBUG
DEBUG		= $(SUB_DEBUG)
NDEBUG		= !$(SUB_DEBUG)
!ELSE
DEBUG		= 1
NDEBUG		= 0
!ENDIF

PROJ		= GIZMOBAR
APPTYPE		= WINDOWS DLL
PROJTYPE	= DLL

CC		= SC
CPP		= SPP
JAVAC		= sj
MAKE		= SMAKE
RC		= RCC
HC		= HC31
ASM		= SC
DISASM		= OBJ2ASM
LNK		= LINK
DLLS		= 

HEADERS		= ..\..\..\include\windows.h ..\..\..\include\win16\WINDOWS.H gizmoint.h  \
		..\include\bttncur.h ..\include\win1632.h gizmo.h gizmobar.h  \
		..\..\..\include\win16\VER.H ..\..\..\include\ver.h 

DEFFILE		= gizmobar.def

!IF $(DEBUG)
OUTPUTDIR	= .
!IF EXIST (.)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $(OUTPUTDIR)\*.* md $(OUTPUTDIR)
!ENDIF
TARGETDIR	= ..\bin
!IF EXIST (..\bin)
CREATETARGETDIR	=
!ELSE
CREATETARGETDIR	= if not exist $(TARGETDIR)\*.* md $(TARGETDIR)
!ENDIF

SYMROOT		= 
SYMS		= 
LIBS		= bttncur.lib LIBW.LIB COMMDLG.LIB SHELL.LIB 

CFLAGS		=  -msw -C -Wdwx -D_WINDOWS -D_WINDLL -S -2 -a2 -c -w- -w2 -w6 -w7 -g 
LFLAGS		=  /CO /PAC:65530 /DE /PACKF /XU /A:16 /IMPL:$(TARGETDIR)\$$SCW$$.LIB /RC  -31 :.\gizmobar.RES
DEFINES		= -D_DEBUG=1
!ELSE
OUTPUTDIR	= .
!IF EXIST (.)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $(OUTPUTDIR)\*.* md $(OUTPUTDIR)
!ENDIF
TARGETDIR	= ..\bin
!IF EXIST (..\bin)
CREATETARGETDIR	=
!ELSE
CREATETARGETDIR	= if not exist $(TARGETDIR)\*.* md $(TARGETDIR)
!ENDIF

SYMROOT		= 
SYMS		= 
LIBS		= bttncur.lib LIBW.LIB COMMDLG.LIB SHELL.LIB 

CFLAGS		=  -msw -C -o+time -Wdwx -D_WINDOWS -D_WINDLL -2 -a2 -c -w- -w2 -w6 -w7 
LFLAGS		=  /CO /PAC:65530 /DE /PACKF /XU /A:16 /IMPL:$(TARGETDIR)\$$SCW$$.LIB /RC  -31 :.\gizmobar.RES
DEFINES		= 
!ENDIF

HFLAGS		= $(CFLAGS) 
MFLAGS		= MASTERPROJ=$(PROJ) 
LIBFLAGS	=  /C 
RESFLAGS	=  
DEBUGGERFLAGS	=  
AFLAGS		= $(CFLAGS) 
HELPFLAGS	= 

MODEL		= S

PAR		= PROJS BATS OBJS

RCDEFINES	= 

INCLUDES	= -I.

INCLUDEDOBJS	= 

OBJS		=  $(OUTPUTDIR)\api.OBJ  $(OUTPUTDIR)\gizmo.OBJ  $(OUTPUTDIR)\gizmobar.OBJ  \
		 $(OUTPUTDIR)\init.OBJ  $(OUTPUTDIR)\paint.OBJ 

RCFILES		= $(OUTPUTDIR)\gizmobar.RES

RESFILES	= 

HELPFILES	= 

PROJS		= ..\bttncur\bttncur.prj 

BATS		= 

PRECOMPILE	=  ?P?P?Bbttncur?Bbttncur?Pprj 

POSTCOMPILE	= 

POSTLINK	=  movelib?Pbat 

.SUFFIXES: .C .CP .CPP .CXX .CC .H .HPP .HXX .COM .EXE .DLL .LIB .RTF .DLG .ASM .RES .RC .OBJ 

.C.OBJ:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.c

.CPP.OBJ:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.cpp

.CXX.OBJ:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.cxx

.CC.OBJ:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.cc

.CP.OBJ:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.cp

.H.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -o$(*B).sym $*.h

.HPP.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -o$(*B).sym $*.hpp

.HXX.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -o$(*B).sym $*.hxx

.C.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES)   $*.c   -o$*.lst

.CPP.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES) $*.cpp -o$*.lst

.CXX.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES) $*.cxx -o$*.lst

.CP.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES)  $*.cp  -o$*.lst

.CC.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES)  $*.cc  -o$*.lst

.ASM.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES) $*.asm -o$*.lst

.OBJ.COD:
	$(DISASM) $*.OBJ -c

!IF $(DEBUG)
.OBJ.EXE:
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\api.OBJ+
.\gizmo.OBJ+
.\gizmobar.OBJ+
.\init.OBJ+
.\paint.OBJ
..\bin\$$SCW$$.DLL
NUL
..\lib\ bttncur.lib LIBW.LIB COMMDLG.LIB SHELL.LIB 
gizmobar.def;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
.\api.OBJ+
.\gizmo.OBJ+
.\gizmobar.OBJ+
.\init.OBJ+
.\paint.OBJ
..\bin\$$SCW$$.DLL
NUL
..\lib\ bttncur.lib LIBW.LIB COMMDLG.LIB SHELL.LIB 
gizmobar.def;
<<
!ENDIF

.RTF.HLP:
	$(HC) $(HELPFLAGS) $*.HPJ

.ASM.OBJ:
	$(ASM) $(AFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.asm

.RC.RES: 
	$(RC) $(RCDEFINES) $(RESFLAGS) $(INCLUDES) $*.rc -o$*.res

.DLG.RES:
	echo ^#include "windows.h" >$$$*.rc
	echo ^IF EXIST "$*.h" >>$$$*.rc
	echo ^#include "$*.h" >>$$$*.rc
	echo ^#include "$*.dlg" >>$$$*.rc
	$(RC) $(RCDEFINES) $(RESFLAGS) $(INCLUDES) $$$*.rc
	-del $*.res
	-ren $$$*.res $*.res



all:	noteout createdir $(PRECOMPILE) $(SYMS) $(OBJS) $(INCLUDEDOBJS) $(POSTCOMPILE) $(TARGETDIR)\$(PROJ).$(PROJTYPE) $(POSTLINK) _done


all2:	createdir $(PRECOMPILE) $(SYMS) $(OBJS) $(INCLUDEDOBJS) $(POSTCOMPILE) $(TARGETDIR)\$(PROJ).$(PROJTYPE) $(POSTLINK) _done

noteout:
	REM Output to $(OUTPUTDIR)

createdir:
	$(CREATEOUTPUTDIR)
	$(CREATETARGETDIR)
	
$(TARGETDIR)\$(PROJ).$(PROJTYPE): $(OBJS) $(INCLUDEDOBJS) $(RCFILES) $(RESFILES) $(HELPFILES) $(DEFFILE)
			-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\api.OBJ+
.\gizmo.OBJ+
.\gizmobar.OBJ+
.\init.OBJ+
.\paint.OBJ
..\bin\$$SCW$$.DLL
NUL
..\lib\ bttncur.lib LIBW.LIB COMMDLG.LIB SHELL.LIB 
gizmobar.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\api.OBJ+
.\gizmo.OBJ+
.\gizmobar.OBJ+
.\init.OBJ+
.\paint.OBJ
..\bin\$$SCW$$.DLL
NUL
..\lib\ bttncur.lib LIBW.LIB COMMDLG.LIB SHELL.LIB 
gizmobar.def;
<<
!ENDIF

			-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)
			-del $(TARGETDIR)\$(PROJ).LIB
			-ren $(TARGETDIR)\$$SCW$$.LIB $(PROJ).LIB
			-echo $(TARGETDIR)\$(PROJ).$(PROJTYPE) built

_done:
		REM  Project is up to date

buildall:	clean	all


clean:
		-del $(TARGETDIR)\$$SCW$$.$(PROJTYPE)
		-del $(PROJ).CLE
		-del $(OUTPUTDIR)\SCPH.SYM
		-del GIZMOBAR.dpd
		-del $(OBJS)
		-del $(OUTPUTDIR)\gizmobar.RES
		cd ..\bttncur\.
		$(MAKE) -fbttncur.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@

cleanres:
		-del $(OUTPUTDIR)\gizmobar.RES

res:		cleanres $(RCFILES) all


link:
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\api.OBJ+
.\gizmo.OBJ+
.\gizmobar.OBJ+
.\init.OBJ+
.\paint.OBJ
..\bin\$$SCW$$.DLL
NUL
..\lib\ bttncur.lib LIBW.LIB COMMDLG.LIB SHELL.LIB 
gizmobar.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\api.OBJ+
.\gizmo.OBJ+
.\gizmobar.OBJ+
.\init.OBJ+
.\paint.OBJ
..\bin\$$SCW$$.DLL
NUL
..\lib\ bttncur.lib LIBW.LIB COMMDLG.LIB SHELL.LIB 
gizmobar.def;
<<
!ENDIF

		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)
			-del $(TARGETDIR)\$(PROJ).LIB
			-ren $(TARGETDIR)\$$SCW$$.LIB $(PROJ).LIB



?P?P?Bbttncur?Bbttncur?Pprj:
		cd ..\bttncur\.
		$(MAKE) -fbttncur.mak "SUB_DEBUG=$(DEBUG)"





movelib?Pbat:
		..\bin\movelib.bat 



!IF EXIST (GIZMOBAR.dpd)
!INCLUDE GIZMOBAR.dpd
!ENDIF



$(OUTPUTDIR)\api.OBJ:	api.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\api.obj api.c



$(OUTPUTDIR)\gizmo.OBJ:	gizmo.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\gizmo.obj gizmo.c



$(OUTPUTDIR)\gizmobar.OBJ:	gizmobar.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\gizmobar.obj gizmobar.c



$(OUTPUTDIR)\init.OBJ:	init.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\init.obj init.c



$(OUTPUTDIR)\paint.OBJ:	paint.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\paint.obj paint.c



$(OUTPUTDIR)\gizmobar.RES:	gizmobar.rc
		$(RC) $(RESFLAGS) $(RCDEFINES) $(INCLUDES) -o$(OUTPUTDIR)\gizmobar.res gizmobar.rc




