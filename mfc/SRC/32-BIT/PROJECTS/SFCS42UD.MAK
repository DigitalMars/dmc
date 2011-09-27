ORIGIN		= Symantec Café
ORIGIN_VER	= Version 7.22
VERSION		= DEBUG

!IFDEF SUB_DEBUG
DEBUG		= $(SUB_DEBUG)
NDEBUG		= !$(SUB_DEBUG)
!ELSE
DEBUG		= 1
NDEBUG		= 0
!ENDIF

PROJ		= SFCS42UD
APPTYPE		= WIN32 LIB
PROJTYPE	= LIB

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

HEADERS		= ..\stdafx.h ..\..\..\include\afx.h ..\..\..\include\32-bit\afx.h  \
		..\..\..\include\afxver_.h ..\..\..\include\32-bit\afxver_.h ..\..\..\include\afxv_w32.h ..\..\..\include\32-bit\afxv_w32.h  \
		..\..\..\..\..\sdkrtl\750b4\include\windows.h ..\..\..\..\..\sdkrtl\750b4\include\win32\scdefs.h ..\..\..\..\..\sdkrtl\750b4\include\win32\windows.h f:\sc\include\excpt.h  \
		f:\sc\include\stdarg.h ..\..\..\..\..\sdkrtl\750b4\include\windef.h ..\..\..\..\..\sdkrtl\750b4\include\win32\windef.h ..\..\..\..\..\sdkrtl\750b4\include\winnt.h  \
		..\..\..\..\..\sdkrtl\750b4\include\win32\winnt.h f:\sc\include\ctype.h ..\..\..\..\..\sdkrtl\750b4\include\pshpack4.h ..\..\..\..\..\sdkrtl\750b4\include\win32\pshpack4.h  \
		..\..\..\..\..\sdkrtl\750b4\include\poppack.h ..\..\..\..\..\sdkrtl\750b4\include\win32\poppack.h ..\..\..\..\..\sdkrtl\750b4\include\win32\pshpack2.h f:\sc\include\string.h  \
		..\..\..\..\..\sdkrtl\750b4\include\winbase.h ..\..\..\..\..\sdkrtl\750b4\include\win32\winbase.h ..\..\..\..\..\sdkrtl\750b4\include\winerror.h ..\..\..\..\..\sdkrtl\750b4\include\win32\winerror.h  \
		..\..\..\..\..\sdkrtl\750b4\include\wingdi.h ..\..\..\..\..\sdkrtl\750b4\include\win32\wingdi.h ..\..\..\..\..\sdkrtl\750b4\include\pshpack1.h ..\..\..\..\..\sdkrtl\750b4\include\win32\pshpack1.h  \
		..\..\..\..\..\sdkrtl\750b4\include\pshpack2.h ..\..\..\..\..\sdkrtl\750b4\include\winuser.h ..\..\..\..\..\sdkrtl\750b4\include\win32\winuser.h 

DEFFILE		= SFCS42UD.DEF

!IF $(DEBUG)
OUTPUTDIR	= UDLLD.W
!IF EXIST (UDLLD.W)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $(OUTPUTDIR)\*.* md $(OUTPUTDIR)
!ENDIF
TARGETDIR	= ..\..\..\LIB
!IF EXIST (..\..\..\LIB)
CREATETARGETDIR	=
!ELSE
CREATETARGETDIR	= if not exist $(TARGETDIR)\*.* md $(TARGETDIR)
!ENDIF

SYMROOT		=  UDLLD.W\stdafx 
SYMS		= $(SYMROOT).SYM 
LIBS		= 

CFLAGS		=  -cpp -Ae -Ar -mn -C -D_WINDLL -NL -ND -5 -a8 -c -H -HDUDLLD.W -HO- -wx -w- -w3 -w17 -g 
LFLAGS		=  /CO /NOI /DE /XN /A:512
DEFINES		= -D_DEBUG -D_X86_ -D_AFXDLL -D_AFX_CORE_IMPL -D_UNICODE=1
!ELSE
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= 

CFLAGS		=  -mn -o+time -D_WINDOWS -3 -a8 -c 
LFLAGS		=  /NOI /DE /E /PACKF /XN /A:512
DEFINES		= -D_UNICODE=1
!ENDIF

HFLAGS		= $(CFLAGS) 
MFLAGS		= MASTERPROJ=$(PROJ) 
LIBFLAGS	=  /C 
RESFLAGS	=  -32 
DEBUGGERFLAGS	=  
AFLAGS		= $(CFLAGS) 
HELPFLAGS	= 

MODEL		= N

PAR		= PROJS BATS OBJS

RCDEFINES	= 

INCLUDES	= -I..\..\..\include

INCLUDEDOBJS	= 

OBJS		=  $(OUTPUTDIR)\appmodul.OBJ  $(OUTPUTDIR)\dllmodul.OBJ  $(OUTPUTDIR)\nolib.OBJ  \
		 $(OUTPUTDIR)\oleexp.OBJ 

RCFILES		= 

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
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -oUDLLD.W\$(*B).sym $*.h

.HPP.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -oUDLLD.W\$(*B).sym $*.hpp

.HXX.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -oUDLLD.W\$(*B).sym $*.hxx

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
UDLLD.W\stdafx.PCO+
UDLLD.W\appmodul.OBJ+
UDLLD.W\dllmodul.OBJ+
UDLLD.W\nolib.OBJ+
UDLLD.W\oleexp.OBJ
..\..\..\LIB\$$SCW$$.LIB
NUL

SFCS42UD.DEF;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
..\appmodul.OBJ+
..\dllmodul.OBJ+
..\nolib.OBJ+
..\oleexp.OBJ
$$SCW$$.LIB
NUL

SFCS42UD.DEF;
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
	
$(TARGETDIR)\$(PROJ).$(PROJTYPE): $(SYMS) $(OBJS) $(INCLUDEDOBJS) $(RCFILES) $(RESFILES) $(HELPFILES) 
			-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		LIB $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(LIBFLAGS)\
				@<<
 + $(OUTPUTDIR)\appmodul.OBJ + $(OUTPUTDIR)\dllmodul.OBJ + $(OUTPUTDIR)\nolib.OBJ &
		 + $(OUTPUTDIR)\oleexp.OBJ
<<
			-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)
			-echo $(TARGETDIR)\$(PROJ).$(PROJTYPE) built

_done:
		REM  Project is up to date

buildall:	clean	all


clean:
		-del $(TARGETDIR)\$$SCW$$.$(PROJTYPE)
		-del $(TARGETDIR)\$(PROJ).CLE
		-del $(OUTPUTDIR)\SCPH.SYM
		-del SFCS42UD.dpd
		-del $(OBJS)
		-del $(SYMROOT).SYM
		-del $(SYMROOT).PCO

cleanres:

res:		cleanres $(RCFILES) all


link:
		LIB $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(LIBFLAGS)\
				@<<
 + $(OUTPUTDIR)\appmodul.OBJ + $(OUTPUTDIR)\dllmodul.OBJ + $(OUTPUTDIR)\nolib.OBJ &
		 + $(OUTPUTDIR)\oleexp.OBJ
<<
		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)




!IF EXIST (SFCS42UD.dpd)
!INCLUDE SFCS42UD.dpd
!ENDIF



$(OUTPUTDIR)\appmodul.OBJ:	..\appmodul.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\appmodul.obj ..\appmodul.cpp



$(OUTPUTDIR)\dllmodul.OBJ:	..\dllmodul.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dllmodul.obj ..\dllmodul.cpp



$(OUTPUTDIR)\nolib.OBJ:	..\nolib.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\nolib.obj ..\nolib.cpp



$(OUTPUTDIR)\oleexp.OBJ:	..\oleexp.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oleexp.obj ..\oleexp.cpp


$(SYMROOT).SYM: ..\stdafx.h
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF$(SYMROOT).SYM -o$(SYMROOT).PCO ..\stdafx.h



