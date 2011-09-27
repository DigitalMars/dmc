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

PROJ		= CTRLTEST
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
		..\..\..\include\win16\COMMDLG.H ctrltest.h resource.h res\otherids.h  \
		..\..\..\mfc\include\afxpen.h ..\..\..\mfc\include\16-bit\afxpen.h ..\..\..\include\penwin.h ..\..\..\include\win16\penwin.h  \
		..\..\..\include\skbapi.h ..\..\..\include\win16\skbapi.h paredit.h spin.h  \
		muscroll.h ..\..\..\mfc\include\16-bit\winres.h ..\..\..\mfc\include\winres.h ..\..\..\include\limits.h 

DEFFILE		= CTRLTEST.DEF

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
LFLAGS		=  /CO /DE /NOPACKF /XU /RC -k  :.\ctrltest.RES
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
LFLAGS		=  /DE /PACKF /XU /RC -k  :.\ctrltest.RES
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

OBJS		=  $(OUTPUTDIR)\bbutton.OBJ  $(OUTPUTDIR)\ctrltest.OBJ  $(OUTPUTDIR)\custlist.OBJ  \
		 $(OUTPUTDIR)\custmenu.OBJ  $(OUTPUTDIR)\derpen.OBJ  $(OUTPUTDIR)\dertest.OBJ  $(OUTPUTDIR)\dlgpen.OBJ  \
		 $(OUTPUTDIR)\featpen.OBJ  $(OUTPUTDIR)\paredit.OBJ  $(OUTPUTDIR)\paredit2.OBJ  $(OUTPUTDIR)\spin.OBJ  \
		 $(OUTPUTDIR)\spintest.OBJ  $(OUTPUTDIR)\subtest.OBJ  $(OUTPUTDIR)\wclstest.OBJ 

RCFILES		= $(OUTPUTDIR)\ctrltest.RES

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
.\bbutton.OBJ+
.\ctrltest.OBJ+
.\custlist.OBJ+
.\custmenu.OBJ+
.\derpen.OBJ+
.\dertest.OBJ+
.\dlgpen.OBJ+
.\featpen.OBJ+
.\paredit.OBJ+
.\paredit2.OBJ+
.\spin.OBJ+
.\spintest.OBJ+
.\subtest.OBJ+
.\wclstest.OBJ
.\$$SCW$$.EXE
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCWD.LIB 
CTRLTEST.DEF;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
.\stdafx.PCO+
.\bbutton.OBJ+
.\ctrltest.OBJ+
.\custlist.OBJ+
.\custmenu.OBJ+
.\derpen.OBJ+
.\dertest.OBJ+
.\dlgpen.OBJ+
.\featpen.OBJ+
.\paredit.OBJ+
.\paredit2.OBJ+
.\spin.OBJ+
.\spintest.OBJ+
.\subtest.OBJ+
.\wclstest.OBJ
.\$$SCW$$.EXE
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCW.LIB 
CTRLTEST.DEF;
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
.\bbutton.OBJ+
.\ctrltest.OBJ+
.\custlist.OBJ+
.\custmenu.OBJ+
.\derpen.OBJ+
.\dertest.OBJ+
.\dlgpen.OBJ+
.\featpen.OBJ+
.\paredit.OBJ+
.\paredit2.OBJ+
.\spin.OBJ+
.\spintest.OBJ+
.\subtest.OBJ+
.\wclstest.OBJ
.\$$SCW$$.EXE
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCWD.LIB 
CTRLTEST.DEF;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\stdafx.PCO+
.\bbutton.OBJ+
.\ctrltest.OBJ+
.\custlist.OBJ+
.\custmenu.OBJ+
.\derpen.OBJ+
.\dertest.OBJ+
.\dlgpen.OBJ+
.\featpen.OBJ+
.\paredit.OBJ+
.\paredit2.OBJ+
.\spin.OBJ+
.\spintest.OBJ+
.\subtest.OBJ+
.\wclstest.OBJ
.\$$SCW$$.EXE
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCW.LIB 
CTRLTEST.DEF;
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
		-del CTRLTEST.dpd
		-del $(OBJS)
		-del $(SYMROOT).SYM
		-del $(SYMROOT).PCO
		-del $(OUTPUTDIR)\ctrltest.RES

cleanres:
		-del $(OUTPUTDIR)\ctrltest.RES

res:		cleanres $(RCFILES) all


link:
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\stdafx.PCO+
.\bbutton.OBJ+
.\ctrltest.OBJ+
.\custlist.OBJ+
.\custmenu.OBJ+
.\derpen.OBJ+
.\dertest.OBJ+
.\dlgpen.OBJ+
.\featpen.OBJ+
.\paredit.OBJ+
.\paredit2.OBJ+
.\spin.OBJ+
.\spintest.OBJ+
.\subtest.OBJ+
.\wclstest.OBJ
.\$$SCW$$.EXE
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCWD.LIB 
CTRLTEST.DEF;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\stdafx.PCO+
.\bbutton.OBJ+
.\ctrltest.OBJ+
.\custlist.OBJ+
.\custmenu.OBJ+
.\derpen.OBJ+
.\dertest.OBJ+
.\dlgpen.OBJ+
.\featpen.OBJ+
.\paredit.OBJ+
.\paredit2.OBJ+
.\spin.OBJ+
.\spintest.OBJ+
.\subtest.OBJ+
.\wclstest.OBJ
.\$$SCW$$.EXE
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCW.LIB 
CTRLTEST.DEF;
<<
!ENDIF

		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)




!IF EXIST (CTRLTEST.dpd)
!INCLUDE CTRLTEST.dpd
!ENDIF



$(OUTPUTDIR)\bbutton.OBJ:	bbutton.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\bbutton.obj bbutton.cpp



$(OUTPUTDIR)\ctrltest.OBJ:	ctrltest.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctrltest.obj ctrltest.cpp



$(OUTPUTDIR)\custlist.OBJ:	custlist.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\custlist.obj custlist.cpp



$(OUTPUTDIR)\custmenu.OBJ:	custmenu.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\custmenu.obj custmenu.cpp



$(OUTPUTDIR)\derpen.OBJ:	derpen.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\derpen.obj derpen.cpp



$(OUTPUTDIR)\dertest.OBJ:	dertest.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dertest.obj dertest.cpp



$(OUTPUTDIR)\dlgpen.OBJ:	dlgpen.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dlgpen.obj dlgpen.cpp



$(OUTPUTDIR)\featpen.OBJ:	featpen.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\featpen.obj featpen.cpp



$(OUTPUTDIR)\paredit.OBJ:	paredit.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\paredit.obj paredit.cpp



$(OUTPUTDIR)\paredit2.OBJ:	paredit2.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\paredit2.obj paredit2.cpp



$(OUTPUTDIR)\spin.OBJ:	spin.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\spin.obj spin.cpp



$(OUTPUTDIR)\spintest.OBJ:	spintest.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\spintest.obj spintest.cpp



$(OUTPUTDIR)\subtest.OBJ:	subtest.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\subtest.obj subtest.cpp



$(OUTPUTDIR)\wclstest.OBJ:	wclstest.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wclstest.obj wclstest.cpp



$(OUTPUTDIR)\ctrltest.RES:	ctrltest.rc
		$(RC) $(RESFLAGS) $(RCDEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctrltest.res ctrltest.rc


$(SYMROOT).SYM: stdafx.h
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF$(SYMROOT).SYM -o$(SYMROOT).PCO stdafx.h



