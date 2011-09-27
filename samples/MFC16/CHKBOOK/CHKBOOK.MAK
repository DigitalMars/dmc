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

PROJ		= CHKBOOK
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
		..\..\..\include\tchar.h ..\..\..\include\string.h ..\..\..\include\stdio.h ..\..\..\include\stdlib.h  \
		..\..\..\include\time.h ..\..\..\include\windows.h ..\..\..\include\win16\WINDOWS.H ..\..\..\include\shellapi.h  \
		..\..\..\include\win16\shellapi.h ..\..\..\mfc\include\afxres.h ..\..\..\mfc\include\16-bit\afxres.h ..\..\..\mfc\include\afxcoll.h  \
		..\..\..\mfc\include\16-bit\afxcoll.h ..\..\..\include\print.h ..\..\..\include\win16\PRINT.H ..\..\..\mfc\include\afxmsg_.h  \
		..\..\..\mfc\include\16-bit\afxmsg_.h ..\..\..\mfc\include\afxdd_.h ..\..\..\mfc\include\16-bit\afxdd_.h ..\..\..\mfc\include\afxext.h  \
		..\..\..\mfc\include\16-bit\afxext.h ..\..\..\mfc\include\afxdlgs.h ..\..\..\mfc\include\16-bit\afxdlgs.h ..\..\..\include\commdlg.h  \
		..\..\..\include\win16\COMMDLG.H chkbook.h resource.h mainfrm.h  \
		chkbkfrm.h dollcent.h fxrecdoc.h checkdoc.h  \
		checkvw.h rowview.h bookvw.h ..\..\..\include\memory.h  \
		..\..\..\include\limits.h ..\..\..\mfc\include\16-bit\winres.h ..\..\..\mfc\include\winres.h 

DEFFILE		= CHKBOOK.DEF

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

SYMROOT		=  .\stdafx 
SYMS		= $(SYMROOT).SYM 
LIBS		= LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCWD.LIB 

CFLAGS		=  -cpp -ml -C -WA -S -2 -a2 -c -H -HO- -w- -w2 -w6 -w7 -g 
LFLAGS		=  /CO /DE /NOPACKF /XU /RC -k  :.\chkbook.RES
DEFINES		= -D_DEBUG=1
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

SYMROOT		=  .\stdafx 
SYMS		= $(SYMROOT).SYM 
LIBS		= LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCW.LIB 

CFLAGS		=  -cpp -ml -o+time -WA -2 -a2 -c -H -HO- -w- -w2 -w6 -w7 
LFLAGS		=  /DE /PACKF /XU /RC -k  :.\chkbook.RES
DEFINES		= 
!ENDIF

HFLAGS		= $(CFLAGS) 
MFLAGS		= MASTERPROJ=$(PROJ) 
LIBFLAGS	=  /C /P:512 
RESFLAGS	=  
DEBUGGERFLAGS	=  
AFLAGS		= $(CFLAGS) 
HELPFLAGS	= 

MODEL		= L

PAR		= PROJS BATS OBJS

RCDEFINES	= 

INCLUDES	= 

INCLUDEDOBJS	= 

OBJS		=  $(OUTPUTDIR)\bookvw.OBJ  $(OUTPUTDIR)\checkdoc.OBJ  $(OUTPUTDIR)\checkvw.OBJ  \
		 $(OUTPUTDIR)\chkbkfrm.OBJ  $(OUTPUTDIR)\chkbook.OBJ  $(OUTPUTDIR)\dollcent.OBJ  $(OUTPUTDIR)\fxrecdoc.OBJ  \
		 $(OUTPUTDIR)\mainfrm.OBJ  $(OUTPUTDIR)\rowview.OBJ 

RCFILES		= $(OUTPUTDIR)\chkbook.RES

RESFILES	= 

HELPFILES	= 

BATS		= 

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
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -o.\$(*B).sym $*.h

.HPP.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -o.\$(*B).sym $*.hpp

.HXX.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -o.\$(*B).sym $*.hxx

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
.\stdafx.PCO+
.\bookvw.OBJ+
.\checkdoc.OBJ+
.\checkvw.OBJ+
.\chkbkfrm.OBJ+
.\chkbook.OBJ+
.\dollcent.OBJ+
.\fxrecdoc.OBJ+
.\mainfrm.OBJ+
.\rowview.OBJ
.\$$SCW$$.EXE
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCWD.LIB 
CHKBOOK.DEF;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
.\stdafx.PCO+
.\bookvw.OBJ+
.\checkdoc.OBJ+
.\checkvw.OBJ+
.\chkbkfrm.OBJ+
.\chkbook.OBJ+
.\dollcent.OBJ+
.\fxrecdoc.OBJ+
.\mainfrm.OBJ+
.\rowview.OBJ
.\$$SCW$$.EXE
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCW.LIB 
CHKBOOK.DEF;
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
	
$(TARGETDIR)\$(PROJ).$(PROJTYPE): $(SYMS) $(OBJS) $(INCLUDEDOBJS) $(RCFILES) $(RESFILES) $(HELPFILES) $(DEFFILE)
			-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\stdafx.PCO+
.\bookvw.OBJ+
.\checkdoc.OBJ+
.\checkvw.OBJ+
.\chkbkfrm.OBJ+
.\chkbook.OBJ+
.\dollcent.OBJ+
.\fxrecdoc.OBJ+
.\mainfrm.OBJ+
.\rowview.OBJ
.\$$SCW$$.EXE
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCWD.LIB 
CHKBOOK.DEF;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\stdafx.PCO+
.\bookvw.OBJ+
.\checkdoc.OBJ+
.\checkvw.OBJ+
.\chkbkfrm.OBJ+
.\chkbook.OBJ+
.\dollcent.OBJ+
.\fxrecdoc.OBJ+
.\mainfrm.OBJ+
.\rowview.OBJ
.\$$SCW$$.EXE
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCW.LIB 
CHKBOOK.DEF;
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
		-del CHKBOOK.dpd
		-del $(OBJS)
		-del $(SYMROOT).SYM
		-del $(SYMROOT).PCO
		-del $(OUTPUTDIR)\chkbook.RES

cleanres:
		-del $(OUTPUTDIR)\chkbook.RES

res:		cleanres $(RCFILES) all


link:
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\stdafx.PCO+
.\bookvw.OBJ+
.\checkdoc.OBJ+
.\checkvw.OBJ+
.\chkbkfrm.OBJ+
.\chkbook.OBJ+
.\dollcent.OBJ+
.\fxrecdoc.OBJ+
.\mainfrm.OBJ+
.\rowview.OBJ
.\$$SCW$$.EXE
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCWD.LIB 
CHKBOOK.DEF;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\stdafx.PCO+
.\bookvw.OBJ+
.\checkdoc.OBJ+
.\checkvw.OBJ+
.\chkbkfrm.OBJ+
.\chkbook.OBJ+
.\dollcent.OBJ+
.\fxrecdoc.OBJ+
.\mainfrm.OBJ+
.\rowview.OBJ
.\$$SCW$$.EXE
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCW.LIB 
CHKBOOK.DEF;
<<
!ENDIF

		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)




!IF EXIST (CHKBOOK.dpd)
!INCLUDE CHKBOOK.dpd
!ENDIF



$(OUTPUTDIR)\bookvw.OBJ:	bookvw.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\bookvw.obj bookvw.cpp



$(OUTPUTDIR)\checkdoc.OBJ:	checkdoc.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\checkdoc.obj checkdoc.cpp



$(OUTPUTDIR)\checkvw.OBJ:	checkvw.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\checkvw.obj checkvw.cpp



$(OUTPUTDIR)\chkbkfrm.OBJ:	chkbkfrm.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\chkbkfrm.obj chkbkfrm.cpp



$(OUTPUTDIR)\chkbook.OBJ:	chkbook.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\chkbook.obj chkbook.cpp



$(OUTPUTDIR)\dollcent.OBJ:	dollcent.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dollcent.obj dollcent.cpp



$(OUTPUTDIR)\fxrecdoc.OBJ:	fxrecdoc.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fxrecdoc.obj fxrecdoc.cpp



$(OUTPUTDIR)\mainfrm.OBJ:	mainfrm.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\mainfrm.obj mainfrm.cpp



$(OUTPUTDIR)\rowview.OBJ:	rowview.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\rowview.obj rowview.cpp



$(OUTPUTDIR)\chkbook.RES:	chkbook.rc
		$(RC) $(RESFLAGS) $(RCDEFINES) $(INCLUDES) -o$(OUTPUTDIR)\chkbook.res chkbook.rc


$(SYMROOT).SYM: stdafx.h
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF$(SYMROOT).SYM -o$(SYMROOT).PCO stdafx.h



