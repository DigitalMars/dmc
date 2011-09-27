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

PROJ		= DLLHUSK
APPTYPE		= WINDOWS EXE
PROJTYPE	= EXE

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

HEADERS		= stdafx.h ..\..\..\mfc\include\afxwin.h ..\..\..\mfc\include\16-bit\afxwin.h  \
		..\..\..\mfc\include\afx.h ..\..\..\mfc\include\16-bit\afx.h ..\..\..\mfc\include\afxver_.h ..\..\..\mfc\include\16-bit\afxver_.h  \
		..\..\..\mfc\include\afxv_dll.h ..\..\..\mfc\include\16-bit\afxv_dll.h ..\..\..\include\tchar.h ..\..\..\include\string.h  \
		..\..\..\include\stdio.h ..\..\..\include\stdlib.h ..\..\..\include\time.h ..\..\..\include\windows.h  \
		..\..\..\include\win16\WINDOWS.H ..\..\..\include\shellapi.h ..\..\..\include\win16\shellapi.h ..\..\..\mfc\include\afxres.h  \
		..\..\..\mfc\include\16-bit\afxres.h ..\..\..\mfc\include\afxcoll.h ..\..\..\mfc\include\16-bit\afxcoll.h ..\..\..\include\print.h  \
		..\..\..\include\win16\PRINT.H ..\..\..\mfc\include\afxmsg_.h ..\..\..\mfc\include\16-bit\afxmsg_.h ..\..\..\mfc\include\afxdd_.h  \
		..\..\..\mfc\include\16-bit\afxdd_.h ..\..\..\mfc\include\afxdll_.h ..\..\..\mfc\include\16-bit\afxdll_.h ..\..\..\mfc\include\afxext.h  \
		..\..\..\mfc\include\16-bit\afxext.h ..\..\..\mfc\include\afxdlgs.h ..\..\..\mfc\include\16-bit\afxdlgs.h ..\..\..\include\commdlg.h  \
		..\..\..\include\win16\COMMDLG.H dllhusk.h resource.h mainfrm.h  \
		testdll1.h testdll2.h ..\..\..\mfc\include\16-bit\winres.h ..\..\..\mfc\include\winres.h  \
		..\..\..\include\limits.h 

DEFFILE		= DLLHUSK.DEF

!IF $(DEBUG)
OUTPUTDIR	= .
!IF EXIST (.)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $(OUTPUTDIR)\*.* md $(OUTPUTDIR)
!ENDIF
TARGETDIR	= .
!IF EXIST (.)
CREATETARGETDIR	=
!ELSE
CREATETARGETDIR	= if not exist $(TARGETDIR)\*.* md $(TARGETDIR)
!ENDIF

SYMROOT		= 
SYMS		= 
LIBS		= testdll1.lib testdll2.lib LIBW.LIB COMMDLG.LIB SHELL.LIB SMFC25D.LIB 

CFLAGS		=  -cpp -ml -C -Wsewtx -D_WINDOWS -D_WINDLL -S -2 -a2 -c -w- -w2 -w6 -w7 -g 
LFLAGS		=  /CO /DE /NOPACKF /XU /RC -k  :.\dllhusk.RES
DEFINES		= -D_DEBUG=1 -D_AFXDLL=1
!ELSE
OUTPUTDIR	= .
!IF EXIST (.)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $(OUTPUTDIR)\*.* md $(OUTPUTDIR)
!ENDIF
TARGETDIR	= .
!IF EXIST (.)
CREATETARGETDIR	=
!ELSE
CREATETARGETDIR	= if not exist $(TARGETDIR)\*.* md $(TARGETDIR)
!ENDIF

SYMROOT		= 
SYMS		= 
LIBS		= testdll1.lib testdll2.lib LIBW.LIB COMMDLG.LIB SHELL.LIB SMFC25.LIB 

CFLAGS		=  -cpp -ml -o+time -Wsewtx -D_WINDOWS -D_WINDLL -2 -a2 -c -w- -w2 -w6 -w7 
LFLAGS		=  /DE /PACKF /XU /RC -k  :.\dllhusk.RES
DEFINES		= -DDEBUG=1 -D_AFXDLL=1
!ENDIF

HFLAGS		= $(CFLAGS) 
MFLAGS		= MASTERPROJ=$(PROJ) 
LIBFLAGS	=  /C /P:512 
RESFLAGS	=  
DEBUGGERFLAGS	=  -DLL:C:\SC\SAMPLES\MFC25\DLLHUSK\DLLHUSK.EXE 
AFLAGS		= $(CFLAGS) 
HELPFLAGS	= 

MODEL		= L

PAR		= PROJS BATS OBJS

RCDEFINES	= 

INCLUDES	= -I.

INCLUDEDOBJS	= 

OBJS		=  $(OUTPUTDIR)\dllhusk.OBJ  $(OUTPUTDIR)\mainfrm.OBJ 

RCFILES		= $(OUTPUTDIR)\dllhusk.RES

RESFILES	= 

HELPFILES	= 

PROJS		= testdll1.prj testdll2.prj 

BATS		= 

PRECOMPILE	=  testdll1?Pprj  testdll2?Pprj 

POSTCOMPILE	= 

POSTLINK	= 

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
.\dllhusk.OBJ+
.\mainfrm.OBJ
.\$$SCW$$.EXE
NUL
.\ testdll1.lib testdll2.lib LIBW.LIB COMMDLG.LIB SHELL.LIB SMFC25D.LIB 
DLLHUSK.DEF;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
.\dllhusk.OBJ+
.\mainfrm.OBJ
.\$$SCW$$.EXE
NUL
.\ testdll1.lib testdll2.lib LIBW.LIB COMMDLG.LIB SHELL.LIB SMFC25.LIB 
DLLHUSK.DEF;
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
.\dllhusk.OBJ+
.\mainfrm.OBJ
.\$$SCW$$.EXE
NUL
.\ testdll1.lib testdll2.lib LIBW.LIB COMMDLG.LIB SHELL.LIB SMFC25D.LIB 
DLLHUSK.DEF;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\dllhusk.OBJ+
.\mainfrm.OBJ
.\$$SCW$$.EXE
NUL
.\ testdll1.lib testdll2.lib LIBW.LIB COMMDLG.LIB SHELL.LIB SMFC25.LIB 
DLLHUSK.DEF;
<<
!ENDIF

			-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)
			-echo $(TARGETDIR)\$(PROJ).$(PROJTYPE) built

_done:
		REM  Project is up to date

buildall:	clean	all


clean:
		-del $(TARGETDIR)\$$SCW$$.$(PROJTYPE)
		-del $(PROJ).CLE
		-del $(OUTPUTDIR)\SCPH.SYM
		-del DLLHUSK.dpd
		-del $(OBJS)
		-del $(OUTPUTDIR)\dllhusk.RES
		$(MAKE) -ftestdll1.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ftestdll2.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@

cleanres:
		-del $(OUTPUTDIR)\dllhusk.RES

res:		cleanres $(RCFILES) all


link:
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\dllhusk.OBJ+
.\mainfrm.OBJ
.\$$SCW$$.EXE
NUL
.\ testdll1.lib testdll2.lib LIBW.LIB COMMDLG.LIB SHELL.LIB SMFC25D.LIB 
DLLHUSK.DEF;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\dllhusk.OBJ+
.\mainfrm.OBJ
.\$$SCW$$.EXE
NUL
.\ testdll1.lib testdll2.lib LIBW.LIB COMMDLG.LIB SHELL.LIB SMFC25.LIB 
DLLHUSK.DEF;
<<
!ENDIF

		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)


testdll1?Pprj:
		$(MAKE) -ftestdll1.mak "SUB_DEBUG=$(DEBUG)"

testdll2?Pprj:
		$(MAKE) -ftestdll2.mak "SUB_DEBUG=$(DEBUG)"





!IF EXIST (DLLHUSK.dpd)
!INCLUDE DLLHUSK.dpd
!ENDIF



$(OUTPUTDIR)\dllhusk.OBJ:	dllhusk.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dllhusk.obj dllhusk.cpp



$(OUTPUTDIR)\mainfrm.OBJ:	mainfrm.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\mainfrm.obj mainfrm.cpp



$(OUTPUTDIR)\dllhusk.RES:	dllhusk.rc
		$(RC) $(RESFLAGS) $(RCDEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dllhusk.res dllhusk.rc




