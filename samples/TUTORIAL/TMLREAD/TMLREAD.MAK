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

PROJ		= TMLREAD
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
		..\..\..\include\win16\COMMDLG.H dibapi.h ..\..\..\include\io.h ..\..\..\include\errno.h  \
		..\..\..\include\math.h ..\..\..\include\direct.h TMLRead.h resource.h  \
		tmlword.h tmllink.h tmlparse.h mainfrm.h  \
		PrefDial.h ViewHdrs.h tmlimage.h tmlvobjs.h  \
		ptrstack.h tmlrddoc.h tmlrdvw.h ..\..\..\include\ctype.h  \
		..\..\..\include\dir.h ..\..\..\include\win16\VER.H ..\..\..\include\ver.h ..\..\..\mfc\include\16-bit\winres.h  \
		..\..\..\mfc\include\winres.h 

DEFFILE		= tmlread.def

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

CFLAGS		=  -cpp -Jm -HO -ml -C -WA -S -2 -a2 -c -H -g -gp 
LFLAGS		=  /CO /L /DE /NOPACKF /XU /RC -k  :.\tmlread.RES
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
LIBS		= LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCW.LIB 

CFLAGS		=  -Jm -ml -o+time -WA -2 -a2 -c 
LFLAGS		=  /DE /E /PACKF /REO /WI /XU /RC -k  :.\tmlread.RES
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

OBJS		=  $(OUTPUTDIR)\dibapi.OBJ  $(OUTPUTDIR)\dibapi2.OBJ  $(OUTPUTDIR)\tmlimage.OBJ  \
		 $(OUTPUTDIR)\tmlparse.OBJ  $(OUTPUTDIR)\tmlrddoc.OBJ  $(OUTPUTDIR)\tmlrdvw.OBJ  $(OUTPUTDIR)\tmlread.OBJ  \
		 $(OUTPUTDIR)\mainfrm.OBJ  $(OUTPUTDIR)\stdafx.OBJ  $(OUTPUTDIR)\prefdial.OBJ 

RCFILES		= $(OUTPUTDIR)\tmlread.RES

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
.\dibapi.OBJ+
.\dibapi2.OBJ+
.\tmlimage.OBJ+
.\tmlparse.OBJ+
.\tmlrddoc.OBJ+
.\tmlrdvw.OBJ+
.\tmlread.OBJ+
.\mainfrm.OBJ+
.\stdafx.OBJ+
.\prefdial.OBJ
.\$$SCW$$.EXE
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCWD.LIB 
tmlread.def;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
.\dibapi.OBJ+
.\dibapi2.OBJ+
.\tmlimage.OBJ+
.\tmlparse.OBJ+
.\tmlrddoc.OBJ+
.\tmlrdvw.OBJ+
.\tmlread.OBJ+
.\mainfrm.OBJ+
.\stdafx.OBJ+
.\prefdial.OBJ
.\$$SCW$$.EXE
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCW.LIB 
tmlread.def;
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
.\dibapi.OBJ+
.\dibapi2.OBJ+
.\tmlimage.OBJ+
.\tmlparse.OBJ+
.\tmlrddoc.OBJ+
.\tmlrdvw.OBJ+
.\tmlread.OBJ+
.\mainfrm.OBJ+
.\stdafx.OBJ+
.\prefdial.OBJ
.\$$SCW$$.EXE
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCWD.LIB 
tmlread.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\dibapi.OBJ+
.\dibapi2.OBJ+
.\tmlimage.OBJ+
.\tmlparse.OBJ+
.\tmlrddoc.OBJ+
.\tmlrdvw.OBJ+
.\tmlread.OBJ+
.\mainfrm.OBJ+
.\stdafx.OBJ+
.\prefdial.OBJ
.\$$SCW$$.EXE
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCW.LIB 
tmlread.def;
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
		-del TMLREAD.dpd
		-del $(OBJS)
		-del $(SYMROOT).SYM
		-del $(SYMROOT).PCO
		-del $(OUTPUTDIR)\tmlread.RES

cleanres:
		-del $(OUTPUTDIR)\tmlread.RES

res:		cleanres $(RCFILES) all


link:
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\stdafx.PCO+
.\dibapi.OBJ+
.\dibapi2.OBJ+
.\tmlimage.OBJ+
.\tmlparse.OBJ+
.\tmlrddoc.OBJ+
.\tmlrdvw.OBJ+
.\tmlread.OBJ+
.\mainfrm.OBJ+
.\stdafx.OBJ+
.\prefdial.OBJ
.\$$SCW$$.EXE
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCWD.LIB 
tmlread.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\dibapi.OBJ+
.\dibapi2.OBJ+
.\tmlimage.OBJ+
.\tmlparse.OBJ+
.\tmlrddoc.OBJ+
.\tmlrdvw.OBJ+
.\tmlread.OBJ+
.\mainfrm.OBJ+
.\stdafx.OBJ+
.\prefdial.OBJ
.\$$SCW$$.EXE
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB LAFXCW.LIB 
tmlread.def;
<<
!ENDIF

		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)




!IF EXIST (TMLREAD.dpd)
!INCLUDE TMLREAD.dpd
!ENDIF



$(OUTPUTDIR)\dibapi.OBJ:	dibapi.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dibapi.obj dibapi.cpp



$(OUTPUTDIR)\dibapi2.OBJ:	dibapi2.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dibapi2.obj dibapi2.cpp



$(OUTPUTDIR)\tmlimage.OBJ:	tmlimage.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\tmlimage.obj tmlimage.cpp



$(OUTPUTDIR)\tmlparse.OBJ:	tmlparse.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\tmlparse.obj tmlparse.cpp



$(OUTPUTDIR)\tmlrddoc.OBJ:	tmlrddoc.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\tmlrddoc.obj tmlrddoc.cpp



$(OUTPUTDIR)\tmlrdvw.OBJ:	tmlrdvw.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\tmlrdvw.obj tmlrdvw.cpp



$(OUTPUTDIR)\tmlread.OBJ:	tmlread.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\tmlread.obj tmlread.cpp



$(OUTPUTDIR)\mainfrm.OBJ:	mainfrm.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\mainfrm.obj mainfrm.cpp



$(OUTPUTDIR)\stdafx.OBJ:	stdafx.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\stdafx.obj stdafx.cpp



$(OUTPUTDIR)\tmlread.RES:	tmlread.rc
		$(RC) $(RESFLAGS) $(RCDEFINES) $(INCLUDES) -o$(OUTPUTDIR)\tmlread.res tmlread.rc



$(OUTPUTDIR)\prefdial.OBJ:	prefdial.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\prefdial.obj prefdial.cpp


$(SYMROOT).SYM: stdafx.h
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF$(SYMROOT).SYM -o$(SYMROOT).PCO stdafx.h



