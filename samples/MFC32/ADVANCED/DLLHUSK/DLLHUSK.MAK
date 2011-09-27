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
APPTYPE		= WIN32 EXE
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

HEADERS		= stdafx.h ..\..\..\..\mfc\include\afxwin.h ..\..\..\..\mfc\include\32-bit\afxwin.h  \
		..\..\..\..\mfc\include\afx.h ..\..\..\..\mfc\include\32-bit\afx.h ..\..\..\..\mfc\include\afxver_.h ..\..\..\..\mfc\include\32-bit\afxver_.h  \
		..\..\..\..\mfc\include\afxv_w32.h ..\..\..\..\mfc\include\32-bit\afxv_w32.h ..\..\..\..\include\windows.h ..\..\..\..\include\win32\scdefs.h  \
		..\..\..\..\include\win32\WINDOWS.H ..\..\..\..\include\excpt.h ..\..\..\..\include\stdarg.h ..\..\..\..\include\windef.h  \
		..\..\..\..\include\win32\WINDEF.H ..\..\..\..\include\winnt.h ..\..\..\..\include\win32\WINNT.H ..\..\..\..\include\ctype.h  \
		..\..\..\..\include\pshpack4.h ..\..\..\..\include\win32\pshpack4.h ..\..\..\..\include\poppack.h ..\..\..\..\include\win32\poppack.h  \
		..\..\..\..\include\win32\pshpack2.h ..\..\..\..\include\string.h ..\..\..\..\include\winbase.h ..\..\..\..\include\win32\WINBASE.H  \
		..\..\..\..\include\winerror.h ..\..\..\..\include\win32\WINERROR.H ..\..\..\..\include\wingdi.h ..\..\..\..\include\win32\WINGDI.H  \
		..\..\..\..\include\pshpack1.h ..\..\..\..\include\win32\pshpack1.h ..\..\..\..\include\pshpack2.h ..\..\..\..\include\winuser.h  \
		..\..\..\..\include\win32\WINUSER.H ..\..\..\..\include\winnls.h ..\..\..\..\include\win32\WINNLS.H ..\..\..\..\include\wincon.h  \
		..\..\..\..\include\win32\WINCON.H ..\..\..\..\include\winver.h ..\..\..\..\include\win32\WINVER.H ..\..\..\..\include\winreg.h  \
		..\..\..\..\include\win32\WINREG.H ..\..\..\..\include\winnetwk.h ..\..\..\..\include\win32\WINNETWK.H ..\..\..\..\include\commctrl.h  \
		..\..\..\..\include\win32\COMMCTRL.H ..\..\..\..\include\prsht.h ..\..\..\..\include\win32\PRSHT.H ..\..\..\..\include\tchar.h  \
		..\..\..\..\mfc\include\afxv_dll.h ..\..\..\..\mfc\include\32-bit\afxv_dll.h ..\..\..\..\include\stdio.h ..\..\..\..\include\stdlib.h  \
		..\..\..\..\include\time.h ..\..\..\..\include\limits.h ..\..\..\..\include\stddef.h ..\..\..\..\include\crtdbg.h  \
		..\..\..\..\mfc\include\afxcoll.h ..\..\..\..\mfc\include\32-bit\afxcoll.h ..\..\..\..\mfc\include\afxstat_.h ..\..\..\..\mfc\include\32-bit\afxstat_.h  \
		..\..\..\..\mfc\include\afxtls_.h ..\..\..\..\mfc\include\32-bit\afxtls_.h ..\..\..\..\include\shellapi.h ..\..\..\..\include\win32\shellapi.h  \
		..\..\..\..\mfc\include\afxres.h ..\..\..\..\mfc\include\32-bit\afxres.h ..\..\..\..\mfc\include\afxmsg_.h ..\..\..\..\mfc\include\32-bit\afxmsg_.h  \
		..\..\..\..\mfc\include\afxdd_.h ..\..\..\..\mfc\include\32-bit\afxdd_.h ..\..\..\..\mfc\include\afxdll_.h ..\..\..\..\mfc\include\32-bit\afxdll_.h  \
		..\..\..\..\mfc\include\afxext.h ..\..\..\..\mfc\include\32-bit\afxext.h ..\..\..\..\mfc\include\afxdlgs.h ..\..\..\..\mfc\include\32-bit\afxdlgs.h  \
		..\..\..\..\include\commdlg.h ..\..\..\..\include\win32\COMMDLG.H ..\..\..\..\include\richedit.h ..\..\..\..\include\win32\RICHEDIT.H  \
		..\..\..\..\mfc\include\afxdisp.h ..\..\..\..\mfc\include\32-bit\afxdisp.h ..\..\..\..\include\objbase.h ..\..\..\..\include\win32\objbase.h  \
		..\..\..\..\include\rpc.h ..\..\..\..\include\win32\rpc.h ..\..\..\..\include\rpcdce.h ..\..\..\..\include\win32\RPCDCE.H  \
		..\..\..\..\include\rpcdcep.h ..\..\..\..\include\win32\RPCDCEP.H ..\..\..\..\include\rpcnsi.h ..\..\..\..\include\win32\RPCNSI.H  \
		..\..\..\..\include\rpcnterr.h ..\..\..\..\include\win32\RPCNTERR.H ..\..\..\..\include\rpcndr.h ..\..\..\..\include\win32\RPCNDR.H  \
		..\..\..\..\include\rpcnsip.h ..\..\..\..\include\win32\RPCNSIP.H ..\..\..\..\include\pshpack8.h ..\..\..\..\include\win32\pshpack8.h  \
		..\..\..\..\include\wtypes.h ..\..\..\..\include\win32\wtypes.h ..\..\..\..\include\unknwn.h ..\..\..\..\include\win32\unknwn.h  \
		..\..\..\..\include\win32\ole2.h ..\..\..\..\include\oleauto.h ..\..\..\..\include\win32\OLEAUTO.H ..\..\..\..\include\oaidl.h  \
		..\..\..\..\include\win32\oaidl.h ..\..\..\..\include\win32\objidl.h ..\..\..\..\include\oleidl.h ..\..\..\..\include\win32\oleidl.h  \
		..\..\..\..\include\objidl.h ..\..\..\..\include\cguid.h ..\..\..\..\include\win32\CGUID.H ..\..\..\..\include\olectl.h  \
		..\..\..\..\include\win32\OLECTL.H ..\..\..\..\include\ocidl.h ..\..\..\..\include\win32\OCIDL.H dllhusk.h  \
		resource.h mainfrm.h testdll1.h testdll2.h  \
		..\..\..\..\mfc\include\32-bit\winres.h 

DEFFILE		= dllhusk.DEF

!IF $(DEBUG)
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		=  .\stdafx 
SYMS		= $(SYMROOT).SYM 
LIBS		= testdll1.LIB testdll2.LIB KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -cpp -Jm -Ae -Ar -mn -C -D_WINDOWS -S -ND -3 -a8 -c -H -HO- -w- -w6 -w17 -g 
LFLAGS		=  /CO /M /NOI /DE /DET /NOPACKF /XN /NT /ENTRY:WinMainCRTStartup /VERS:1.0 /BAS:4194304 /A:512 /RC   :dllhusk.RES
DEFINES		= -D_DEBUG -D_AFX_NO_DAO_SUPPORT -D_DEBUG=1 -D_X86_=1 -D_MT=1 -D_AFXDLL=1
!ELSE
OUTPUTDIR	= EXE
!IF EXIST (EXE)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $(OUTPUTDIR)\*.* md $(OUTPUTDIR)
!ENDIF
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		=  .\stdafx 
SYMS		= $(SYMROOT).SYM 
LIBS		= testdll1.LIB testdll2.LIB KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -cpp -Ae -Ar -mn -o+time -D_WINDOWS -ND -3 -a8 -c -H -HO- -w- -w6 -w17 
LFLAGS		=  /NOI /DE /PACKF /XN /NT /ENTRY:WinMainCRTStartup /VERS:1.0 /BAS:4194304 /A:512 /RC   :EXE\dllhusk.RES
DEFINES		= -D_X86_=1 -D_MT=1 -D_AFXDLL=1
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

OBJS		= dllhusk.OBJ mainfrm.OBJ 

RCFILES		=  dllhusk.RES 

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
dllhusk.OBJ+
mainfrm.OBJ
$$SCW$$.EXE
DLLHUSK.MAP
testdll1.LIB testdll2.LIB KERNEL32.LIB GDI32.LIB USER32.LIB 
dllhusk.DEF;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
.\stdafx.PCO+
EXE\dllhusk.OBJ+
EXE\mainfrm.OBJ
$$SCW$$.EXE
NUL
testdll1.LIB testdll2.LIB KERNEL32.LIB GDI32.LIB USER32.LIB 
dllhusk.DEF;
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
dllhusk.OBJ+
mainfrm.OBJ
$$SCW$$.EXE
DLLHUSK.MAP
testdll1.LIB testdll2.LIB KERNEL32.LIB GDI32.LIB USER32.LIB 
dllhusk.DEF;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\stdafx.PCO+
EXE\dllhusk.OBJ+
EXE\mainfrm.OBJ
$$SCW$$.EXE
NUL
testdll1.LIB testdll2.LIB KERNEL32.LIB GDI32.LIB USER32.LIB 
dllhusk.DEF;
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
		-del $(SYMROOT).SYM
		-del $(SYMROOT).PCO
		-del dllhusk.RES
		$(MAKE) -ftestdll1.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ftestdll2.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@

cleanres:
		-del dllhusk.RES

res:		cleanres $(RCFILES) all


link:
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\stdafx.PCO+
dllhusk.OBJ+
mainfrm.OBJ
$$SCW$$.EXE
DLLHUSK.MAP
testdll1.LIB testdll2.LIB KERNEL32.LIB GDI32.LIB USER32.LIB 
dllhusk.DEF;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\stdafx.PCO+
EXE\dllhusk.OBJ+
EXE\mainfrm.OBJ
$$SCW$$.EXE
NUL
testdll1.LIB testdll2.LIB KERNEL32.LIB GDI32.LIB USER32.LIB 
dllhusk.DEF;
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
$(SYMROOT).SYM: stdafx.h
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF$(SYMROOT).SYM -o$(SYMROOT).PCO stdafx.h



