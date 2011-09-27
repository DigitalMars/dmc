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

PROJ		= REALTIME
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

HEADERS		= stdafx.h ..\..\..\..\mfc\include\afxwin.h ..\..\..\..\mfc\include\16-bit\afxwin.h  \
		..\..\..\..\mfc\include\afx.h ..\..\..\..\mfc\include\16-bit\afx.h ..\..\..\..\mfc\include\afxver_.h ..\..\..\..\mfc\include\16-bit\afxver_.h  \
		..\..\..\..\include\tchar.h ..\..\..\..\include\string.h ..\..\..\..\include\stdio.h ..\..\..\..\include\stdlib.h  \
		..\..\..\..\include\time.h ..\..\..\..\include\windows.h ..\..\..\..\include\win16\WINDOWS.H ..\..\..\..\include\shellapi.h  \
		..\..\..\..\include\win16\shellapi.h ..\..\..\..\mfc\include\afxres.h ..\..\..\..\mfc\include\16-bit\afxres.h ..\..\..\..\mfc\include\afxcoll.h  \
		..\..\..\..\mfc\include\16-bit\afxcoll.h ..\..\..\..\include\print.h ..\..\..\..\include\win16\PRINT.H ..\..\..\..\mfc\include\afxmsg_.h  \
		..\..\..\..\mfc\include\16-bit\afxmsg_.h ..\..\..\..\mfc\include\afxdd_.h ..\..\..\..\mfc\include\16-bit\afxdd_.h ..\..\..\..\mfc\include\afxext.h  \
		..\..\..\..\mfc\include\16-bit\afxext.h ..\..\..\..\mfc\include\afxdlgs.h ..\..\..\..\mfc\include\16-bit\afxdlgs.h ..\..\..\..\include\commdlg.h  \
		..\..\..\..\include\win16\COMMDLG.H ..\..\..\..\include\ole2.h ..\..\..\..\include\win16\OLE2.H ..\..\..\..\include\compobj.h  \
		..\..\..\..\include\win16\COMPOBJ.H ..\..\..\..\include\win16\scode.h ..\..\..\..\include\win16\coguid.h ..\..\..\..\include\win16\oleguid.h  \
		..\..\..\..\include\dvobj.h ..\..\..\..\include\win16\DVOBJ.H ..\..\..\..\include\storage.h ..\..\..\..\include\win16\STORAGE.H  \
		..\..\..\..\include\moniker.h ..\..\..\..\include\win16\MONIKER.H utils.h realtdoc.h  \
		realtime.h resource.h mainfrm.h rtitem.h  \
		..\..\..\..\include\ole2ver.h ..\..\..\..\include\win16\OLE2VER.H rtformvi.h rtcntr.h  \
		..\prgidmon.h ..\..\..\..\include\win16\VER.H ..\..\..\..\include\ver.h ..\..\..\..\mfc\include\16-bit\winres.h  \
		..\..\..\..\mfc\include\winres.h 

DEFFILE		= realtime.def

!IF $(DEBUG)
OUTPUTDIR	= .
!IF EXIST (.)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $(OUTPUTDIR)\*.* md $(OUTPUTDIR)
!ENDIF
TARGETDIR	= ..\..\bin
!IF EXIST (..\..\bin)
CREATETARGETDIR	=
!ELSE
CREATETARGETDIR	= if not exist $(TARGETDIR)\*.* md $(TARGETDIR)
!ENDIF

SYMROOT		=  .\stdafx 
SYMS		= $(SYMROOT).SYM 
LIBS		= prgidmon.lib LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCWD.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB  MFCOLEUI.LIB 

CFLAGS		=  -cpp -HO -ml -C -WA -S -2 -a2 -c -H -w- -w2 -w6 -w7 -g 
LFLAGS		=  /CO /PAC:65530 /DE /PACKF /XU /A:16 /RC -k -31 :.\realtime.RES
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

SYMROOT		= 
SYMS		= 
LIBS		= prgidmon.lib LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCW.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB  MFCOLEUI.LIB 

CFLAGS		=  -ml -o+time -WA -2 -a2 -c -w- -w2 
LFLAGS		=  /DE /E /PACKF /REO /WI /XU /A:16 /RC -k -31 :.\realtime.RES
DEFINES		= 
!ENDIF

HFLAGS		= $(CFLAGS) 
MFLAGS		= MASTERPROJ=$(PROJ) 
LIBFLAGS	=  /C 
RESFLAGS	=  
DEBUGGERFLAGS	=  
AFLAGS		= $(CFLAGS) 
HELPFLAGS	= 

MODEL		= L

PAR		= PROJS BATS OBJS

RCDEFINES	= 

INCLUDES	= -I.

INCLUDEDOBJS	= 

OBJS		=  $(OUTPUTDIR)\mainfrm.OBJ  $(OUTPUTDIR)\realtdoc.OBJ  $(OUTPUTDIR)\realtime.OBJ  \
		 $(OUTPUTDIR)\rtcntr.OBJ  $(OUTPUTDIR)\rtformvi.OBJ  $(OUTPUTDIR)\rtitem.OBJ  $(OUTPUTDIR)\utils.OBJ  \
		 $(OUTPUTDIR)\stdafx.OBJ 

RCFILES		= $(OUTPUTDIR)\realtime.RES

RESFILES	= 

HELPFILES	= 

PROJS		= ..\dataview\dataview.prj 

BATS		= 

PRECOMPILE	=  ?P?P?Bdataview?Bdataview?Pprj 

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
.\mainfrm.OBJ+
.\realtdoc.OBJ+
.\realtime.OBJ+
.\rtcntr.OBJ+
.\rtformvi.OBJ+
.\rtitem.OBJ+
.\utils.OBJ+
.\stdafx.OBJ
..\..\bin\$$SCW$$.EXE
NUL
..\..\lib\ prgidmon.lib LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCWD.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB  MFCOLEUI.LIB 
realtime.def;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
.\mainfrm.OBJ+
.\realtdoc.OBJ+
.\realtime.OBJ+
.\rtcntr.OBJ+
.\rtformvi.OBJ+
.\rtitem.OBJ+
.\utils.OBJ+
.\stdafx.OBJ
.\$$SCW$$.EXE
NUL
..\..\lib\ prgidmon.lib LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCW.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB  MFCOLEUI.LIB 
realtime.def;
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
.\mainfrm.OBJ+
.\realtdoc.OBJ+
.\realtime.OBJ+
.\rtcntr.OBJ+
.\rtformvi.OBJ+
.\rtitem.OBJ+
.\utils.OBJ+
.\stdafx.OBJ
..\..\bin\$$SCW$$.EXE
NUL
..\..\lib\ prgidmon.lib LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCWD.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB  MFCOLEUI.LIB 
realtime.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\mainfrm.OBJ+
.\realtdoc.OBJ+
.\realtime.OBJ+
.\rtcntr.OBJ+
.\rtformvi.OBJ+
.\rtitem.OBJ+
.\utils.OBJ+
.\stdafx.OBJ
.\$$SCW$$.EXE
NUL
..\..\lib\ prgidmon.lib LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCW.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB  MFCOLEUI.LIB 
realtime.def;
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
		-del REALTIME.dpd
		-del $(OBJS)
		-del $(SYMROOT).SYM
		-del $(SYMROOT).PCO
		-del $(OUTPUTDIR)\realtime.RES
		cd ..\dataview\.
		$(MAKE) -fdataview.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@

cleanres:
		-del $(OUTPUTDIR)\realtime.RES

res:		cleanres $(RCFILES) all


link:
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\stdafx.PCO+
.\mainfrm.OBJ+
.\realtdoc.OBJ+
.\realtime.OBJ+
.\rtcntr.OBJ+
.\rtformvi.OBJ+
.\rtitem.OBJ+
.\utils.OBJ+
.\stdafx.OBJ
..\..\bin\$$SCW$$.EXE
NUL
..\..\lib\ prgidmon.lib LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCWD.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB  MFCOLEUI.LIB 
realtime.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\mainfrm.OBJ+
.\realtdoc.OBJ+
.\realtime.OBJ+
.\rtcntr.OBJ+
.\rtformvi.OBJ+
.\rtitem.OBJ+
.\utils.OBJ+
.\stdafx.OBJ
.\$$SCW$$.EXE
NUL
..\..\lib\ prgidmon.lib LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCW.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB  MFCOLEUI.LIB 
realtime.def;
<<
!ENDIF

		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)


?P?P?Bdataview?Bdataview?Pprj:
		cd ..\dataview\.
		$(MAKE) -fdataview.mak "SUB_DEBUG=$(DEBUG)"





!IF EXIST (REALTIME.dpd)
!INCLUDE REALTIME.dpd
!ENDIF



$(OUTPUTDIR)\mainfrm.OBJ:	mainfrm.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\mainfrm.obj mainfrm.cpp



$(OUTPUTDIR)\realtdoc.OBJ:	realtdoc.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\realtdoc.obj realtdoc.cpp



$(OUTPUTDIR)\realtime.OBJ:	realtime.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\realtime.obj realtime.cpp



$(OUTPUTDIR)\rtcntr.OBJ:	rtcntr.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\rtcntr.obj rtcntr.cpp



$(OUTPUTDIR)\rtformvi.OBJ:	rtformvi.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\rtformvi.obj rtformvi.cpp



$(OUTPUTDIR)\rtitem.OBJ:	rtitem.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\rtitem.obj rtitem.cpp



$(OUTPUTDIR)\utils.OBJ:	utils.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\utils.obj utils.cpp



$(OUTPUTDIR)\realtime.RES:	realtime.rc
		$(RC) $(RESFLAGS) $(RCDEFINES) $(INCLUDES) -o$(OUTPUTDIR)\realtime.res realtime.rc



$(OUTPUTDIR)\stdafx.OBJ:	stdafx.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\stdafx.obj stdafx.cpp


$(SYMROOT).SYM: stdafx.h
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF$(SYMROOT).SYM -o$(SYMROOT).PCO stdafx.h



