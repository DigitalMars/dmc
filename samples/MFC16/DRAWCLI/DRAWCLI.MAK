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

PROJ		= DRAWCLI
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
		..\..\..\include\win16\COMMDLG.H ..\..\..\mfc\include\afxole.h ..\..\..\mfc\include\16-bit\afxole.h ..\..\..\mfc\include\afxdisp.h  \
		..\..\..\mfc\include\16-bit\afxdisp.h ..\..\..\include\compobj.h ..\..\..\include\win16\COMPOBJ.H ..\..\..\include\win16\scode.h  \
		..\..\..\include\win16\coguid.h ..\..\..\include\scode.h ..\..\..\include\storage.h ..\..\..\include\win16\STORAGE.H  \
		..\..\..\include\dispatch.h ..\..\..\include\win16\DISPATCH.H ..\..\..\include\win16\variant.h ..\..\..\include\stddef.h  \
		..\..\..\include\ole2.h ..\..\..\include\win16\OLE2.H ..\..\..\include\win16\oleguid.h ..\..\..\include\dvobj.h  \
		..\..\..\include\win16\DVOBJ.H ..\..\..\include\moniker.h ..\..\..\include\win16\MONIKER.H ..\..\..\mfc\include\afxodlgs.h  \
		..\..\..\mfc\include\16-bit\afxodlgs.h ..\..\..\include\ole2ui.h ..\..\..\include\win16\OLE2UI.H ..\..\..\include\dlgs.h  \
		..\..\..\include\win16\DLGS.H ..\..\..\include\win16\olestd.h ..\..\..\include\dos.h drawcli.h  \
		resource.h drawdoc.h drawobj.h drawvw.h  \
		cntritem.h mainfrm.h splitfrm.h rectdlg.h  \
		drawtool.h ..\..\..\mfc\include\afxpriv.h ..\..\..\mfc\include\16-bit\afxpriv.h ..\..\..\include\win16\VER.H  \
		..\..\..\include\ver.h ..\..\..\mfc\include\16-bit\winres.h ..\..\..\mfc\include\winres.h ..\..\..\include\limits.h 

DEFFILE		= drawcli.def

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
LIBS		= odbc.lib LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCWD.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB  MFCOLEUI.LIB 

CFLAGS		=  -cpp -ml -C -WA -S -2 -a2 -c -H -HO- -w- -w2 -w6 -w7 -g 
LFLAGS		=  /CO /DE /NOPACKF /XU /RC -k  :.\drawcli.RES
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
LIBS		= odbc.lib LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCW.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB  MFCOLEUI.LIB 

CFLAGS		=  -cpp -ml -o+time -WA -2 -a2 -c -H -HO- -w- -w2 -w6 -w7 
LFLAGS		=  /DE /PACKF /XU /RC -k  :.\drawcli.RES
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

OBJS		=  $(OUTPUTDIR)\cntritem.OBJ  $(OUTPUTDIR)\drawcli.OBJ  $(OUTPUTDIR)\drawdoc.OBJ  \
		 $(OUTPUTDIR)\drawobj.OBJ  $(OUTPUTDIR)\drawtool.OBJ  $(OUTPUTDIR)\drawvw.OBJ  $(OUTPUTDIR)\mainfrm.OBJ  \
		 $(OUTPUTDIR)\rectdlg.OBJ  $(OUTPUTDIR)\splitfrm.OBJ 

RCFILES		= $(OUTPUTDIR)\drawcli.RES

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
.\cntritem.OBJ+
.\drawcli.OBJ+
.\drawdoc.OBJ+
.\drawobj.OBJ+
.\drawtool.OBJ+
.\drawvw.OBJ+
.\mainfrm.OBJ+
.\rectdlg.OBJ+
.\splitfrm.OBJ
.\$$SCW$$.EXE
NUL
.\ odbc.lib LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCWD.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB  MFCOLEUI.LIB 
drawcli.def;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
.\stdafx.PCO+
.\cntritem.OBJ+
.\drawcli.OBJ+
.\drawdoc.OBJ+
.\drawobj.OBJ+
.\drawtool.OBJ+
.\drawvw.OBJ+
.\mainfrm.OBJ+
.\rectdlg.OBJ+
.\splitfrm.OBJ
.\$$SCW$$.EXE
NUL
.\ odbc.lib LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCW.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB  MFCOLEUI.LIB 
drawcli.def;
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
.\cntritem.OBJ+
.\drawcli.OBJ+
.\drawdoc.OBJ+
.\drawobj.OBJ+
.\drawtool.OBJ+
.\drawvw.OBJ+
.\mainfrm.OBJ+
.\rectdlg.OBJ+
.\splitfrm.OBJ
.\$$SCW$$.EXE
NUL
.\ odbc.lib LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCWD.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB  MFCOLEUI.LIB 
drawcli.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\stdafx.PCO+
.\cntritem.OBJ+
.\drawcli.OBJ+
.\drawdoc.OBJ+
.\drawobj.OBJ+
.\drawtool.OBJ+
.\drawvw.OBJ+
.\mainfrm.OBJ+
.\rectdlg.OBJ+
.\splitfrm.OBJ
.\$$SCW$$.EXE
NUL
.\ odbc.lib LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCW.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB  MFCOLEUI.LIB 
drawcli.def;
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
		-del DRAWCLI.dpd
		-del $(OBJS)
		-del $(SYMROOT).SYM
		-del $(SYMROOT).PCO
		-del $(OUTPUTDIR)\drawcli.RES

cleanres:
		-del $(OUTPUTDIR)\drawcli.RES

res:		cleanres $(RCFILES) all


link:
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\stdafx.PCO+
.\cntritem.OBJ+
.\drawcli.OBJ+
.\drawdoc.OBJ+
.\drawobj.OBJ+
.\drawtool.OBJ+
.\drawvw.OBJ+
.\mainfrm.OBJ+
.\rectdlg.OBJ+
.\splitfrm.OBJ
.\$$SCW$$.EXE
NUL
.\ odbc.lib LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCWD.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB  MFCOLEUI.LIB 
drawcli.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\stdafx.PCO+
.\cntritem.OBJ+
.\drawcli.OBJ+
.\drawdoc.OBJ+
.\drawobj.OBJ+
.\drawtool.OBJ+
.\drawvw.OBJ+
.\mainfrm.OBJ+
.\rectdlg.OBJ+
.\splitfrm.OBJ
.\$$SCW$$.EXE
NUL
.\ odbc.lib LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCW.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB  MFCOLEUI.LIB 
drawcli.def;
<<
!ENDIF

		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)




!IF EXIST (DRAWCLI.dpd)
!INCLUDE DRAWCLI.dpd
!ENDIF



$(OUTPUTDIR)\cntritem.OBJ:	cntritem.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\cntritem.obj cntritem.cpp



$(OUTPUTDIR)\drawcli.OBJ:	drawcli.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\drawcli.obj drawcli.cpp



$(OUTPUTDIR)\drawdoc.OBJ:	drawdoc.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\drawdoc.obj drawdoc.cpp



$(OUTPUTDIR)\drawobj.OBJ:	drawobj.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\drawobj.obj drawobj.cpp



$(OUTPUTDIR)\drawtool.OBJ:	drawtool.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\drawtool.obj drawtool.cpp



$(OUTPUTDIR)\drawvw.OBJ:	drawvw.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\drawvw.obj drawvw.cpp



$(OUTPUTDIR)\mainfrm.OBJ:	mainfrm.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\mainfrm.obj mainfrm.cpp



$(OUTPUTDIR)\rectdlg.OBJ:	rectdlg.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\rectdlg.obj rectdlg.cpp



$(OUTPUTDIR)\splitfrm.OBJ:	splitfrm.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\splitfrm.obj splitfrm.cpp



$(OUTPUTDIR)\drawcli.RES:	drawcli.rc
		$(RC) $(RESFLAGS) $(RCDEFINES) $(INCLUDES) -o$(OUTPUTDIR)\drawcli.res drawcli.rc


$(SYMROOT).SYM: stdafx.h
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF$(SYMROOT).SYM -o$(SYMROOT).PCO stdafx.h



