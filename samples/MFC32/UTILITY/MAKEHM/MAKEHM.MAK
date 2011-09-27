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

PROJ		= MAKEHM
APPTYPE		= WIN32 CONSOLE
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

HEADERS		= ..\..\..\..\INCLUDE\ctype.h ..\..\..\..\MFC\INCLUDE\afxcoll.h ..\..\..\..\MFC\INCLUDE\32-bit\afxcoll.h  \
		..\..\..\..\MFC\INCLUDE\afx.h ..\..\..\..\MFC\INCLUDE\32-bit\afx.h ..\..\..\..\MFC\INCLUDE\afxver_.h ..\..\..\..\MFC\INCLUDE\32-bit\afxver_.h  \
		..\..\..\..\MFC\INCLUDE\afxv_w32.h ..\..\..\..\MFC\INCLUDE\32-bit\afxv_w32.h ..\..\..\..\INCLUDE\windows.h ..\..\..\..\INCLUDE\win32\scdefs.h  \
		..\..\..\..\INCLUDE\win32\windows.h ..\..\..\..\INCLUDE\excpt.h ..\..\..\..\INCLUDE\stdarg.h ..\..\..\..\INCLUDE\windef.h  \
		..\..\..\..\INCLUDE\win32\windef.h ..\..\..\..\INCLUDE\winnt.h ..\..\..\..\INCLUDE\win32\winnt.h ..\..\..\..\INCLUDE\pshpack4.h  \
		..\..\..\..\INCLUDE\win32\pshpack4.h ..\..\..\..\INCLUDE\poppack.h ..\..\..\..\INCLUDE\win32\poppack.h ..\..\..\..\INCLUDE\win32\pshpack1.h  \
		..\..\..\..\INCLUDE\string.h ..\..\..\..\INCLUDE\winbase.h ..\..\..\..\INCLUDE\win32\winbase.h ..\..\..\..\INCLUDE\winerror.h  \
		..\..\..\..\INCLUDE\win32\winerror.h ..\..\..\..\INCLUDE\wingdi.h ..\..\..\..\INCLUDE\win32\wingdi.h ..\..\..\..\INCLUDE\pshpack1.h  \
		..\..\..\..\INCLUDE\pshpack2.h ..\..\..\..\INCLUDE\win32\pshpack2.h ..\..\..\..\INCLUDE\winuser.h ..\..\..\..\INCLUDE\win32\winuser.h  \
		..\..\..\..\INCLUDE\winnls.h ..\..\..\..\INCLUDE\win32\winnls.h ..\..\..\..\INCLUDE\wincon.h ..\..\..\..\INCLUDE\win32\wincon.h  \
		..\..\..\..\INCLUDE\winver.h ..\..\..\..\INCLUDE\win32\winver.h ..\..\..\..\INCLUDE\winreg.h ..\..\..\..\INCLUDE\win32\winreg.h  \
		..\..\..\..\INCLUDE\winnetwk.h ..\..\..\..\INCLUDE\win32\winnetwk.h ..\..\..\..\INCLUDE\winsvc.h ..\..\..\..\INCLUDE\win32\winsvc.h  \
		..\..\..\..\INCLUDE\mcx.h ..\..\..\..\INCLUDE\win32\mcx.h ..\..\..\..\INCLUDE\imm.h ..\..\..\..\INCLUDE\win32\imm.h  \
		..\..\..\..\INCLUDE\commctrl.h ..\..\..\..\INCLUDE\win32\commctrl.h ..\..\..\..\INCLUDE\prsht.h ..\..\..\..\INCLUDE\win32\prsht.h  \
		..\..\..\..\INCLUDE\tchar.h ..\..\..\..\INCLUDE\mbstring.h ..\..\..\..\INCLUDE\mbctype.h ..\..\..\..\INCLUDE\stdio.h  \
		..\..\..\..\INCLUDE\stdlib.h ..\..\..\..\INCLUDE\time.h ..\..\..\..\INCLUDE\limits.h ..\..\..\..\INCLUDE\stddef.h  \
		..\..\..\..\MFC\INCLUDE\afxstat_.h ..\..\..\..\MFC\INCLUDE\32-bit\afxstat_.h ..\..\..\..\MFC\INCLUDE\afxtls_.h ..\..\..\..\MFC\INCLUDE\32-bit\afxtls_.h  \
		..\..\..\..\include\crtdbg.h 

DEFFILE		= makehm.DEF

!IF $(DEBUG)
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -cpp -Ae -mn -C -WA -S -3 -a8 -c -g 
LFLAGS		=  /CO /NOI /DE /NOPACKF /XN /NT /ENTRY:mainCRTStartup /BAS:4194304 /A:512
DEFINES		= -D_DEBUG=1 -D_CONSOLE=1 -D_MBCS=1 -D_X86_=1 -D_MT=1
!ELSE
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -cpp -Ae -mn -o+time -WA -3 -a8 -c 
LFLAGS		=  /NOI /DE /PACKF /XN /NT /ENTRY:mainCRTStartup /BAS:4194304 /A:512
DEFINES		= -D_CONSOLE=1 -D_X86_=1 -D_MT=1 -D_MBCS=1
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

INCLUDES	= 

INCLUDEDOBJS	= 

OBJS		= makehm.OBJ 

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
makehm.OBJ
$$SCW$$.EXE
NUL
KERNEL32.LIB GDI32.LIB USER32.LIB 
makehm.DEF;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
makehm.OBJ
$$SCW$$.EXE
NUL
KERNEL32.LIB GDI32.LIB USER32.LIB 
makehm.DEF;
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
makehm.OBJ
$$SCW$$.EXE
NUL
KERNEL32.LIB GDI32.LIB USER32.LIB 
makehm.DEF;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
makehm.OBJ
$$SCW$$.EXE
NUL
KERNEL32.LIB GDI32.LIB USER32.LIB 
makehm.DEF;
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
		-del MAKEHM.dpd
		-del $(OBJS)

cleanres:

res:		cleanres $(RCFILES) all


link:
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
makehm.OBJ
$$SCW$$.EXE
NUL
KERNEL32.LIB GDI32.LIB USER32.LIB 
makehm.DEF;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
makehm.OBJ
$$SCW$$.EXE
NUL
KERNEL32.LIB GDI32.LIB USER32.LIB 
makehm.DEF;
<<
!ENDIF

		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)




!IF EXIST (MAKEHM.dpd)
!INCLUDE MAKEHM.dpd
!ENDIF


