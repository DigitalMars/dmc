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

PROJ		= sfcd42d
APPTYPE		= WIN32 DLL
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

HEADERS		= ..\stdafx.h ..\..\..\include\afx.h ..\..\..\include\32-bit\afx.h  \
		..\..\..\include\afxver_.h ..\..\..\include\32-bit\afxver_.h ..\..\..\include\afxv_w32.h ..\..\..\include\32-bit\afxv_w32.h  \
		..\..\..\..\..\sdkrtl\750b4\include\windows.h ..\..\..\..\..\sdkrtl\750b4\include\win32\scdefs.h ..\..\..\..\..\sdkrtl\750b4\include\win32\windows.h f:\sc\include\excpt.h  \
		f:\sc\include\stdarg.h ..\..\..\..\..\sdkrtl\750b4\include\windef.h ..\..\..\..\..\sdkrtl\750b4\include\win32\windef.h ..\..\..\..\..\sdkrtl\750b4\include\winnt.h  \
		..\..\..\..\..\sdkrtl\750b4\include\win32\winnt.h f:\sc\include\ctype.h ..\..\..\..\..\sdkrtl\750b4\include\pshpack4.h ..\..\..\..\..\sdkrtl\750b4\include\win32\pshpack4.h  \
		..\..\..\..\..\sdkrtl\750b4\include\poppack.h ..\..\..\..\..\sdkrtl\750b4\include\win32\poppack.h ..\..\..\..\..\sdkrtl\750b4\include\win32\pshpack2.h f:\sc\include\string.h  \
		..\..\..\..\..\sdkrtl\750b4\include\winbase.h ..\..\..\..\..\sdkrtl\750b4\include\win32\winbase.h ..\..\..\..\..\sdkrtl\750b4\include\winerror.h ..\..\..\..\..\sdkrtl\750b4\include\win32\winerror.h  \
		..\..\..\..\..\sdkrtl\750b4\include\wingdi.h ..\..\..\..\..\sdkrtl\750b4\include\win32\wingdi.h ..\..\..\..\..\sdkrtl\750b4\include\pshpack1.h ..\..\..\..\..\sdkrtl\750b4\include\win32\pshpack1.h  \
		..\..\..\..\..\sdkrtl\750b4\include\pshpack2.h 

DEFFILE		= SFCD42D.DEF

!IF $(DEBUG)
OUTPUTDIR	= DLLDD.W
!IF EXIST (DLLDD.W)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $(OUTPUTDIR)\*.* md $(OUTPUTDIR)
!ENDIF
TARGETDIR	= ..\..\..\BIN
!IF EXIST (..\..\..\BIN)
CREATETARGETDIR	=
!ELSE
CREATETARGETDIR	= if not exist $(TARGETDIR)\*.* md $(TARGETDIR)
!ENDIF

SYMROOT		=  DLLDD.W\stdafx 
SYMS		= $(SYMROOT).SYM 
LIBS		= uuid.lib uuid2.lib ..\..\..\LIB\SFC42D.LIB  \
		SFCo42d.lib KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -cpp -Ae -Ar -mn -C -D_WINDLL -ND -5 -a8 -c -H -HDDLLDD.W -HO- -wx -w- -w3 -w17 -g 
LFLAGS		=  /CO /NOI /DE /XN /NT /ENTRY:_DllMainCRTStartup /VERS:4.0 /BAS:0x5FC00000 /A:512 /IMPL:$(TARGETDIR)\$$SCW$$.LIB /RC   :DLLDD.W\mfcdb.RES
DEFINES		= -D_DEBUG -D_X86_ -D_AFXDLL -D_AFX_DB_IMPL -D_MT -D_MBCS=1
!ELSE
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= uuid.lib uuid2.lib ..\..\..\LIB\SFC42D.LIB  \
		SFCo42d.lib KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -mn -o+time -WD -3 -a8 -c 
LFLAGS		=  /NOI /DO /DE /E /PACKF /XN /NT /ENTRY:_DllMainCRTStartup /VERS:1.0 /BAS:268435456 /A:512 /IMPL:$(TARGETDIR)\$$SCW$$.LIB /RC   :..\mfcdb.RES
DEFINES		= 
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

RCDEFINES	= -D_DEBUG

INCLUDES	= -I..\..\..\include

INCLUDEDOBJS	= 

OBJS		=  $(OUTPUTDIR)\dlldb.OBJ  $(OUTPUTDIR)\dbcore.OBJ  $(OUTPUTDIR)\dbrfx.OBJ  \
		 $(OUTPUTDIR)\dbview.OBJ  $(OUTPUTDIR)\dbflt.OBJ  $(OUTPUTDIR)\dblong.OBJ  $(OUTPUTDIR)\dbvar.OBJ  \
		 $(OUTPUTDIR)\daocore.OBJ  $(OUTPUTDIR)\daodfx.OBJ  $(OUTPUTDIR)\daoview.OBJ 

RCFILES		= $(OUTPUTDIR)\mfcdb.RES

RESFILES	= 

HELPFILES	= 

BATS		= 

PRECOMPILE	= 

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
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -oDLLDD.W\$(*B).sym $*.h

.HPP.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -oDLLDD.W\$(*B).sym $*.hpp

.HXX.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -oDLLDD.W\$(*B).sym $*.hxx

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
DLLDD.W\stdafx.PCO+
DLLDD.W\dlldb.OBJ+
DLLDD.W\dbcore.OBJ+
DLLDD.W\dbrfx.OBJ+
DLLDD.W\dbview.OBJ+
DLLDD.W\dbflt.OBJ+
DLLDD.W\dblong.OBJ+
DLLDD.W\dbvar.OBJ+
DLLDD.W\daocore.OBJ+
DLLDD.W\daodfx.OBJ+
DLLDD.W\daoview.OBJ
..\..\..\BIN\$$SCW$$.DLL
NUL
..\..\..\lib\ uuid.lib uuid2.lib ..\..\..\LIB\SFC42D.LIB SFCo42d.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
SFCD42D.DEF;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
..\dlldb.OBJ+
..\dbcore.OBJ+
..\dbrfx.OBJ+
..\dbview.OBJ+
..\dbflt.OBJ+
..\dblong.OBJ+
..\dbvar.OBJ+
..\daocore.OBJ+
..\daodfx.OBJ+
..\daoview.OBJ
$$SCW$$.DLL
NUL
uuid.lib uuid2.lib ..\..\..\LIB\SFC42D.LIB SFCo42d.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
SFCD42D.DEF;
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
DLLDD.W\stdafx.PCO+
DLLDD.W\dlldb.OBJ+
DLLDD.W\dbcore.OBJ+
DLLDD.W\dbrfx.OBJ+
DLLDD.W\dbview.OBJ+
DLLDD.W\dbflt.OBJ+
DLLDD.W\dblong.OBJ+
DLLDD.W\dbvar.OBJ+
DLLDD.W\daocore.OBJ+
DLLDD.W\daodfx.OBJ+
DLLDD.W\daoview.OBJ
..\..\..\BIN\$$SCW$$.DLL
NUL
..\..\..\lib\ uuid.lib uuid2.lib ..\..\..\LIB\SFC42D.LIB SFCo42d.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
SFCD42D.DEF;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
..\dlldb.OBJ+
..\dbcore.OBJ+
..\dbrfx.OBJ+
..\dbview.OBJ+
..\dbflt.OBJ+
..\dblong.OBJ+
..\dbvar.OBJ+
..\daocore.OBJ+
..\daodfx.OBJ+
..\daoview.OBJ
$$SCW$$.DLL
NUL
uuid.lib uuid2.lib ..\..\..\LIB\SFC42D.LIB SFCo42d.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
SFCD42D.DEF;
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
		-del $(TARGETDIR)\$(PROJ).CLE
		-del $(OUTPUTDIR)\SCPH.SYM
		-del sfcd42d.dpd
		-del $(OBJS)
		-del $(SYMROOT).SYM
		-del $(SYMROOT).PCO
		-del $(OUTPUTDIR)\mfcdb.RES

cleanres:
		-del $(OUTPUTDIR)\mfcdb.RES

res:		cleanres $(RCFILES) all


link:
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
DLLDD.W\stdafx.PCO+
DLLDD.W\dlldb.OBJ+
DLLDD.W\dbcore.OBJ+
DLLDD.W\dbrfx.OBJ+
DLLDD.W\dbview.OBJ+
DLLDD.W\dbflt.OBJ+
DLLDD.W\dblong.OBJ+
DLLDD.W\dbvar.OBJ+
DLLDD.W\daocore.OBJ+
DLLDD.W\daodfx.OBJ+
DLLDD.W\daoview.OBJ
..\..\..\BIN\$$SCW$$.DLL
NUL
..\..\..\lib\ uuid.lib uuid2.lib ..\..\..\LIB\SFC42D.LIB SFCo42d.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
SFCD42D.DEF;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
..\dlldb.OBJ+
..\dbcore.OBJ+
..\dbrfx.OBJ+
..\dbview.OBJ+
..\dbflt.OBJ+
..\dblong.OBJ+
..\dbvar.OBJ+
..\daocore.OBJ+
..\daodfx.OBJ+
..\daoview.OBJ
$$SCW$$.DLL
NUL
uuid.lib uuid2.lib ..\..\..\LIB\SFC42D.LIB SFCo42d.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
SFCD42D.DEF;
<<
!ENDIF

		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)
			-del $(TARGETDIR)\$(PROJ).LIB
			-ren $(TARGETDIR)\$$SCW$$.LIB $(PROJ).LIB





movelib?Pbat:
		movelib.bat 



!IF EXIST (sfcd42d.dpd)
!INCLUDE sfcd42d.dpd
!ENDIF



$(OUTPUTDIR)\mfcdb.RES:	..\mfcdb.rc
		$(RC) $(RESFLAGS) $(RCDEFINES) $(INCLUDES) -o$(OUTPUTDIR)\mfcdb.res ..\mfcdb.rc



$(OUTPUTDIR)\dlldb.OBJ:	..\dlldb.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dlldb.obj ..\dlldb.cpp



$(OUTPUTDIR)\dbcore.OBJ:	..\dbcore.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dbcore.obj ..\dbcore.cpp



$(OUTPUTDIR)\dbrfx.OBJ:	..\dbrfx.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dbrfx.obj ..\dbrfx.cpp



$(OUTPUTDIR)\dbview.OBJ:	..\dbview.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dbview.obj ..\dbview.cpp



$(OUTPUTDIR)\dbflt.OBJ:	..\dbflt.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dbflt.obj ..\dbflt.cpp



$(OUTPUTDIR)\dblong.OBJ:	..\dblong.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dblong.obj ..\dblong.cpp



$(OUTPUTDIR)\dbvar.OBJ:	..\dbvar.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dbvar.obj ..\dbvar.cpp



$(OUTPUTDIR)\daocore.OBJ:	..\daocore.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\daocore.obj ..\daocore.cpp



$(OUTPUTDIR)\daodfx.OBJ:	..\daodfx.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\daodfx.obj ..\daodfx.cpp



$(OUTPUTDIR)\daoview.OBJ:	..\daoview.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\daoview.obj ..\daoview.cpp


$(SYMROOT).SYM: ..\stdafx.h
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF$(SYMROOT).SYM -o$(SYMROOT).PCO ..\stdafx.h



