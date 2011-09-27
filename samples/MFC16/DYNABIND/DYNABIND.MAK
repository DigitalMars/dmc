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

PROJ		= DYNABIND
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
		..\..\..\include\win16\COMMDLG.H ..\..\..\mfc\include\afxdb.h ..\..\..\mfc\include\16-bit\afxdb.h ..\..\..\include\sql.h  \
		..\..\..\include\win16\SQL.H ..\..\..\include\sqlext.h ..\..\..\include\win16\SQLEXT.H enroll.h  \
		resource.h sectset.h addfield.h ..\catalog\columnst.h  \
		columnst.h coursset.h enroldoc.h mainfrm.h  \
		sectform.h ..\..\..\include\win16\VER.H ..\..\..\include\ver.h ..\..\..\mfc\include\16-bit\winres.h  \
		..\..\..\mfc\include\winres.h ..\..\..\include\limits.h 

DEFFILE		= dynabind.def

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
LIBS		= odbc.lib LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCWD.LIB  ODBC.LIB

CFLAGS		=  -cpp -ml -C -Wrstx -D_WINDOWS -S -2 -a2 -c -H -HO- -w- -w2 -w6 -w7 -g 
LFLAGS		=  /CO /DE /NOPACKF /XU /RC -k  :.\dynabind.RES
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
LIBS		= odbc.lib LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCW.LIB  ODBC.LIB

CFLAGS		=  -cpp -ml -o+time -Wrstx -D_WINDOWS -2 -a2 -c -H -HO- -w- -w2 -w6 -w7 
LFLAGS		=  /DE /PACKF /XU /RC -k  :.\dynabind.RES
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

INCLUDES	= -I.

INCLUDEDOBJS	= 

OBJS		=  $(OUTPUTDIR)\addfield.OBJ  $(OUTPUTDIR)\columnst.OBJ  $(OUTPUTDIR)\coursset.OBJ  \
		 $(OUTPUTDIR)\enroldoc.OBJ  $(OUTPUTDIR)\enroll.OBJ  $(OUTPUTDIR)\mainfrm.OBJ  $(OUTPUTDIR)\sectform.OBJ  \
		 $(OUTPUTDIR)\sectset.OBJ 

RCFILES		= $(OUTPUTDIR)\dynabind.RES

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
.\addfield.OBJ+
.\columnst.OBJ+
.\coursset.OBJ+
.\enroldoc.OBJ+
.\enroll.OBJ+
.\mainfrm.OBJ+
.\sectform.OBJ+
.\sectset.OBJ
.\$$SCW$$.EXE
NUL
.\ odbc.lib LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCWD.LIB  ODBC.LIB
dynabind.def;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
.\stdafx.PCO+
.\addfield.OBJ+
.\columnst.OBJ+
.\coursset.OBJ+
.\enroldoc.OBJ+
.\enroll.OBJ+
.\mainfrm.OBJ+
.\sectform.OBJ+
.\sectset.OBJ
.\$$SCW$$.EXE
NUL
.\ odbc.lib LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCW.LIB  ODBC.LIB
dynabind.def;
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
.\addfield.OBJ+
.\columnst.OBJ+
.\coursset.OBJ+
.\enroldoc.OBJ+
.\enroll.OBJ+
.\mainfrm.OBJ+
.\sectform.OBJ+
.\sectset.OBJ
.\$$SCW$$.EXE
NUL
.\ odbc.lib LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCWD.LIB  ODBC.LIB
dynabind.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\stdafx.PCO+
.\addfield.OBJ+
.\columnst.OBJ+
.\coursset.OBJ+
.\enroldoc.OBJ+
.\enroll.OBJ+
.\mainfrm.OBJ+
.\sectform.OBJ+
.\sectset.OBJ
.\$$SCW$$.EXE
NUL
.\ odbc.lib LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCW.LIB  ODBC.LIB
dynabind.def;
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
		-del DYNABIND.dpd
		-del $(OBJS)
		-del $(SYMROOT).SYM
		-del $(SYMROOT).PCO
		-del $(OUTPUTDIR)\dynabind.RES

cleanres:
		-del $(OUTPUTDIR)\dynabind.RES

res:		cleanres $(RCFILES) all


link:
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\stdafx.PCO+
.\addfield.OBJ+
.\columnst.OBJ+
.\coursset.OBJ+
.\enroldoc.OBJ+
.\enroll.OBJ+
.\mainfrm.OBJ+
.\sectform.OBJ+
.\sectset.OBJ
.\$$SCW$$.EXE
NUL
.\ odbc.lib LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCWD.LIB  ODBC.LIB
dynabind.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\stdafx.PCO+
.\addfield.OBJ+
.\columnst.OBJ+
.\coursset.OBJ+
.\enroldoc.OBJ+
.\enroll.OBJ+
.\mainfrm.OBJ+
.\sectform.OBJ+
.\sectset.OBJ
.\$$SCW$$.EXE
NUL
.\ odbc.lib LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCW.LIB  ODBC.LIB
dynabind.def;
<<
!ENDIF

		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)




!IF EXIST (DYNABIND.dpd)
!INCLUDE DYNABIND.dpd
!ENDIF



$(OUTPUTDIR)\addfield.OBJ:	addfield.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\addfield.obj addfield.cpp



$(OUTPUTDIR)\columnst.OBJ:	columnst.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\columnst.obj columnst.cpp



$(OUTPUTDIR)\coursset.OBJ:	coursset.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\coursset.obj coursset.cpp



$(OUTPUTDIR)\enroldoc.OBJ:	enroldoc.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\enroldoc.obj enroldoc.cpp



$(OUTPUTDIR)\enroll.OBJ:	enroll.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\enroll.obj enroll.cpp



$(OUTPUTDIR)\mainfrm.OBJ:	mainfrm.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\mainfrm.obj mainfrm.cpp



$(OUTPUTDIR)\sectform.OBJ:	sectform.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\sectform.obj sectform.cpp



$(OUTPUTDIR)\sectset.OBJ:	sectset.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\sectset.obj sectset.cpp



$(OUTPUTDIR)\dynabind.RES:	dynabind.rc
		$(RC) $(RESFLAGS) $(RCDEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dynabind.res dynabind.rc


$(SYMROOT).SYM: stdafx.h
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF$(SYMROOT).SYM -o$(SYMROOT).PCO stdafx.h



