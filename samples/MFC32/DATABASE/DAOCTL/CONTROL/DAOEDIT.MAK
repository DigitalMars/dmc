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

PROJ		= DAOEDIT
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

HEADERS		= stdafx.h ..\..\..\..\..\MFC\INCLUDE\afxctl.h ..\..\..\..\..\MFC\INCLUDE\32-bit\afxctl.h  \
		..\..\..\..\..\MFC\INCLUDE\afxole.h ..\..\..\..\..\MFC\INCLUDE\32-bit\afxole.h ..\..\..\..\..\MFC\INCLUDE\afxext.h ..\..\..\..\..\MFC\INCLUDE\32-bit\afxext.h  \
		..\..\..\..\..\MFC\INCLUDE\afxwin.h ..\..\..\..\..\MFC\INCLUDE\32-bit\afxwin.h ..\..\..\..\..\MFC\INCLUDE\afx.h ..\..\..\..\..\MFC\INCLUDE\32-bit\afx.h  \
		..\..\..\..\..\MFC\INCLUDE\afxver_.h ..\..\..\..\..\MFC\INCLUDE\32-bit\afxver_.h ..\..\..\..\..\MFC\INCLUDE\afxv_w32.h ..\..\..\..\..\MFC\INCLUDE\32-bit\afxv_w32.h  \
		..\..\..\..\..\INCLUDE\windows.h ..\..\..\..\..\INCLUDE\win32\scdefs.h ..\..\..\..\..\INCLUDE\win32\windows.h ..\..\..\..\..\INCLUDE\excpt.h  \
		..\..\..\..\..\INCLUDE\stdarg.h ..\..\..\..\..\INCLUDE\windef.h ..\..\..\..\..\INCLUDE\win32\windef.h ..\..\..\..\..\INCLUDE\winnt.h  \
		..\..\..\..\..\INCLUDE\win32\winnt.h ..\..\..\..\..\INCLUDE\ctype.h ..\..\..\..\..\INCLUDE\pshpack4.h ..\..\..\..\..\INCLUDE\win32\pshpack4.h  \
		..\..\..\..\..\INCLUDE\poppack.h ..\..\..\..\..\INCLUDE\win32\poppack.h ..\..\..\..\..\INCLUDE\string.h ..\..\..\..\..\INCLUDE\winbase.h  \
		..\..\..\..\..\INCLUDE\win32\winbase.h ..\..\..\..\..\INCLUDE\winerror.h ..\..\..\..\..\INCLUDE\win32\winerror.h ..\..\..\..\..\INCLUDE\wingdi.h  \
		..\..\..\..\..\INCLUDE\win32\wingdi.h ..\..\..\..\..\INCLUDE\pshpack1.h ..\..\..\..\..\INCLUDE\win32\pshpack1.h ..\..\..\..\..\INCLUDE\pshpack2.h  \
		..\..\..\..\..\INCLUDE\win32\pshpack2.h ..\..\..\..\..\INCLUDE\winuser.h ..\..\..\..\..\INCLUDE\win32\winuser.h ..\..\..\..\..\INCLUDE\commctrl.h  \
		..\..\..\..\..\INCLUDE\win32\commctrl.h ..\..\..\..\..\INCLUDE\prsht.h ..\..\..\..\..\INCLUDE\win32\prsht.h ..\..\..\..\..\INCLUDE\tchar.h  \
		..\..\..\..\..\INCLUDE\mbstring.h ..\..\..\..\..\INCLUDE\mbctype.h ..\..\..\..\..\MFC\INCLUDE\afxv_dll.h ..\..\..\..\..\MFC\INCLUDE\32-bit\afxv_dll.h  \
		..\..\..\..\..\INCLUDE\stdio.h ..\..\..\..\..\INCLUDE\stdlib.h ..\..\..\..\..\INCLUDE\time.h ..\..\..\..\..\INCLUDE\limits.h  \
		..\..\..\..\..\INCLUDE\stddef.h ..\..\..\..\..\INCLUDE\crtdbg.h ..\..\..\..\..\MFC\INCLUDE\afxcoll.h ..\..\..\..\..\MFC\INCLUDE\32-bit\afxcoll.h  \
		..\..\..\..\..\MFC\INCLUDE\afxstat_.h ..\..\..\..\..\MFC\INCLUDE\32-bit\afxstat_.h ..\..\..\..\..\MFC\INCLUDE\afxtls_.h ..\..\..\..\..\MFC\INCLUDE\32-bit\afxtls_.h  \
		..\..\..\..\..\INCLUDE\shellapi.h ..\..\..\..\..\INCLUDE\win32\shellapi.h ..\..\..\..\..\MFC\INCLUDE\afxres.h ..\..\..\..\..\MFC\INCLUDE\32-bit\afxres.h  \
		..\..\..\..\..\MFC\INCLUDE\afxmsg_.h ..\..\..\..\..\MFC\INCLUDE\32-bit\afxmsg_.h ..\..\..\..\..\MFC\INCLUDE\afxdd_.h ..\..\..\..\..\MFC\INCLUDE\32-bit\afxdd_.h  \
		..\..\..\..\..\MFC\INCLUDE\afxdll_.h ..\..\..\..\..\MFC\INCLUDE\32-bit\afxdll_.h ..\..\..\..\..\MFC\INCLUDE\afxdlgs.h ..\..\..\..\..\MFC\INCLUDE\32-bit\afxdlgs.h  \
		..\..\..\..\..\INCLUDE\commdlg.h ..\..\..\..\..\INCLUDE\win32\commdlg.h ..\..\..\..\..\INCLUDE\richedit.h ..\..\..\..\..\INCLUDE\win32\richedit.h  \
		..\..\..\..\..\MFC\INCLUDE\afxdisp.h ..\..\..\..\..\MFC\INCLUDE\32-bit\afxdisp.h ..\..\..\..\..\INCLUDE\objbase.h ..\..\..\..\..\INCLUDE\win32\objbase.h  \
		..\..\..\..\..\INCLUDE\win32\rpcbase.h ..\..\..\..\..\INCLUDE\pshpack8.h ..\..\..\..\..\INCLUDE\win32\pshpack8.h ..\..\..\..\..\INCLUDE\wtypes.h  \
		..\..\..\..\..\INCLUDE\win32\wtypes.h ..\..\..\..\..\INCLUDE\unknwn.h ..\..\..\..\..\INCLUDE\win32\unknwn.h ..\..\..\..\..\INCLUDE\win32\ole2.h  \
		..\..\..\..\..\INCLUDE\oleauto.h ..\..\..\..\..\INCLUDE\win32\oleauto.h ..\..\..\..\..\INCLUDE\oaidl.h ..\..\..\..\..\INCLUDE\win32\oaidl.h  \
		..\..\..\..\..\INCLUDE\win32\objidl.h ..\..\..\..\..\INCLUDE\oleidl.h ..\..\..\..\..\INCLUDE\win32\oleidl.h ..\..\..\..\..\INCLUDE\cguid.h  \
		..\..\..\..\..\INCLUDE\win32\cguid.h ..\..\..\..\..\INCLUDE\olectl.h ..\..\..\..\..\INCLUDE\win32\olectl.h ..\..\..\..\..\INCLUDE\olectlid.h  \
		..\..\..\..\..\INCLUDE\win32\olectlid.h ..\..\..\..\..\MFC\INCLUDE\afxdb.h ..\..\..\..\..\MFC\INCLUDE\32-bit\afxdb.h ..\..\..\..\..\MFC\INCLUDE\afxdb_.h  \
		..\..\..\..\..\MFC\INCLUDE\32-bit\afxdb_.h ..\..\..\..\..\INCLUDE\sql.h ..\..\..\..\..\INCLUDE\win32\sql.h ..\..\..\..\..\INCLUDE\win32\sqltypes.h  \
		..\..\..\..\..\INCLUDE\sqlext.h ..\..\..\..\..\INCLUDE\win32\sqlext.h ..\..\..\..\..\MFC\INCLUDE\afxdao.h ..\..\..\..\..\MFC\INCLUDE\32-bit\afxdao.h  \
		..\..\..\..\..\INCLUDE\dbdaoint.h ..\..\..\..\..\INCLUDE\win32\dbdaoint.h ..\..\..\..\..\INCLUDE\daogetrw.h ..\..\..\..\..\INCLUDE\win32\daogetrw.h  \
		..\..\..\..\..\INCLUDE\dbdaoid.h ..\..\..\..\..\INCLUDE\win32\dbdaoid.h ..\..\..\..\..\INCLUDE\dbdaoerr.h ..\..\..\..\..\INCLUDE\win32\dbdaoerr.h  \
		DaoEdit.h resource.h DaoCtl.h DaoPpg.h  \
		..\..\..\..\..\include\winnls.h ..\..\..\..\..\include\win32\WINNLS.H ..\..\..\..\..\include\wincon.h ..\..\..\..\..\include\win32\WINCON.H  \
		..\..\..\..\..\include\winver.h ..\..\..\..\..\include\win32\WINVER.H ..\..\..\..\..\include\winreg.h ..\..\..\..\..\include\win32\WINREG.H  \
		..\..\..\..\..\include\winnetwk.h ..\..\..\..\..\include\win32\WINNETWK.H ..\..\..\..\..\include\rpc.h ..\..\..\..\..\include\win32\rpc.h  \
		..\..\..\..\..\include\rpcdce.h ..\..\..\..\..\include\win32\RPCDCE.H ..\..\..\..\..\include\rpcdcep.h ..\..\..\..\..\include\win32\RPCDCEP.H  \
		..\..\..\..\..\include\rpcnsi.h ..\..\..\..\..\include\win32\RPCNSI.H ..\..\..\..\..\include\rpcnterr.h ..\..\..\..\..\include\win32\RPCNTERR.H  \
		..\..\..\..\..\include\rpcndr.h ..\..\..\..\..\include\win32\RPCNDR.H ..\..\..\..\..\include\rpcnsip.h ..\..\..\..\..\include\win32\RPCNSIP.H  \
		..\..\..\..\..\include\objidl.h ..\..\..\..\..\include\ocidl.h ..\..\..\..\..\include\win32\OCIDL.H ..\..\..\..\..\include\docobj.h  \
		..\..\..\..\..\include\win32\DOCOBJ.H ..\..\..\..\..\include\win32\servprov.h ..\..\..\..\..\include\urlmon.h ..\..\..\..\..\include\win32\URLMON.H  \
		..\..\..\..\..\mfc\include\afxcom_.h ..\..\..\..\..\mfc\include\32-bit\afxcom_.h ..\..\..\..\..\include\sqltypes.h ..\..\..\..\..\mfc\include\32-bit\winres.h 

DEFFILE		= daoedit.def

!IF $(DEBUG)
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		=  .\stdafx 
SYMS		= $(SYMROOT).SYM 
LIBS		= KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -cpp -Ae -mn -C -D_WINDOWS -D_WINDLL -S -ND -3 -a8 -c -H -HO- -g 
LFLAGS		=  /CO /NOI /DO /DE /NOPACKF /XN /NT /ENTRY:_DllMainCRTStartup /BAS:268435456 /A:512 /IMPL:$(TARGETDIR)\$$SCW$$.LIB /RC   :daoedit.RES
DEFINES		= -D_DLL -D_DEBUG=1 -D_AFXDLL -D_MBCS=1 -D_X86_=1 -D_MT=1 -D_AFXEXT=1
!ELSE
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		=  .\stdafx 
SYMS		= $(SYMROOT).SYM 
LIBS		= KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -cpp -Ae -mn -o+time -D_WINDOWS -D_WINDLL -ND -3 -a8 -c -H -HO- 
LFLAGS		=  /NOI /DO /DE /PACKF /XN /NT /ENTRY:_DllMainCRTStartup /BAS:268435456 /A:512 /IMPL:$(TARGETDIR)\$$SCW$$.LIB /RC   :daoedit.RES
DEFINES		= -D_AFXDLL -D_X86_=1 -D_MT=1 -D_AFXEXT=1 -D_MBCS=1
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

INCLUDES	= 

INCLUDEDOBJS	= 

OBJS		= daoctl.OBJ daoedit.OBJ daoppg.OBJ 

RCFILES		=  daoedit.RES 

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
daoctl.OBJ+
daoedit.OBJ+
daoppg.OBJ
$$SCW$$.DLL
NUL
KERNEL32.LIB GDI32.LIB USER32.LIB 
daoedit.def;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
.\stdafx.PCO+
daoctl.OBJ+
daoedit.OBJ+
daoppg.OBJ
$$SCW$$.DLL
NUL
KERNEL32.LIB GDI32.LIB USER32.LIB 
daoedit.def;
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
daoctl.OBJ+
daoedit.OBJ+
daoppg.OBJ
$$SCW$$.DLL
NUL
KERNEL32.LIB GDI32.LIB USER32.LIB 
daoedit.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\stdafx.PCO+
daoctl.OBJ+
daoedit.OBJ+
daoppg.OBJ
$$SCW$$.DLL
NUL
KERNEL32.LIB GDI32.LIB USER32.LIB 
daoedit.def;
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
		-del $(PROJ).CLE
		-del $(OUTPUTDIR)\SCPH.SYM
		-del DAOEDIT.dpd
		-del $(OBJS)
		-del $(SYMROOT).SYM
		-del $(SYMROOT).PCO
		-del daoedit.RES

cleanres:
		-del daoedit.RES

res:		cleanres $(RCFILES) all


link:
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\stdafx.PCO+
daoctl.OBJ+
daoedit.OBJ+
daoppg.OBJ
$$SCW$$.DLL
NUL
KERNEL32.LIB GDI32.LIB USER32.LIB 
daoedit.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\stdafx.PCO+
daoctl.OBJ+
daoedit.OBJ+
daoppg.OBJ
$$SCW$$.DLL
NUL
KERNEL32.LIB GDI32.LIB USER32.LIB 
daoedit.def;
<<
!ENDIF

		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)
			-del $(TARGETDIR)\$(PROJ).LIB
			-ren $(TARGETDIR)\$$SCW$$.LIB $(PROJ).LIB





!IF EXIST (DAOEDIT.dpd)
!INCLUDE DAOEDIT.dpd
!ENDIF
$(SYMROOT).SYM: stdafx.h
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF$(SYMROOT).SYM -o$(SYMROOT).PCO stdafx.h



