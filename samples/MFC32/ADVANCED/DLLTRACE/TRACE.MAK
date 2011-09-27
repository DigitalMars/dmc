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

PROJ		= TRACE
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

HEADERS		= ..\..\..\SC\MFC\INCLUDE\afxwin.h ..\..\..\SC\MFC\INCLUDE\32-bit\afxwin.h ..\..\..\SC\MFC\INCLUDE\afx.h  \
		..\..\..\SC\MFC\INCLUDE\32-bit\afx.h ..\..\..\SC\MFC\INCLUDE\afxver_.h ..\..\..\SC\MFC\INCLUDE\32-bit\afxver_.h ..\..\..\SC\MFC\INCLUDE\afxv_w32.h  \
		..\..\..\SC\MFC\INCLUDE\32-bit\afxv_w32.h ..\..\..\SC\INCLUDE\windows.h ..\..\..\SC\INCLUDE\win32\scdefs.h ..\..\..\SC\INCLUDE\win32\windows.h  \
		..\..\..\SC\INCLUDE\excpt.h ..\..\..\SC\INCLUDE\stdarg.h ..\..\..\SC\INCLUDE\windef.h ..\..\..\SC\INCLUDE\win32\windef.h  \
		..\..\..\SC\INCLUDE\winnt.h ..\..\..\SC\INCLUDE\win32\winnt.h ..\..\..\SC\INCLUDE\ctype.h ..\..\..\SC\INCLUDE\pshpack4.h  \
		..\..\..\SC\INCLUDE\win32\pshpack4.h ..\..\..\SC\INCLUDE\poppack.h ..\..\..\SC\INCLUDE\win32\poppack.h ..\..\..\SC\INCLUDE\win32\pshpack1.h  \
		..\..\..\SC\INCLUDE\string.h ..\..\..\SC\INCLUDE\winbase.h ..\..\..\SC\INCLUDE\win32\winbase.h ..\..\..\SC\INCLUDE\winerror.h  \
		..\..\..\SC\INCLUDE\win32\winerror.h ..\..\..\SC\INCLUDE\wingdi.h ..\..\..\SC\INCLUDE\win32\wingdi.h ..\..\..\SC\INCLUDE\pshpack1.h  \
		..\..\..\SC\INCLUDE\pshpack2.h ..\..\..\SC\INCLUDE\win32\pshpack2.h traceapi.h traceres.h  \
		..\..\..\SC\INCLUDE\winuser.h ..\..\..\SC\INCLUDE\win32\winuser.h ..\..\..\SC\INCLUDE\winnls.h ..\..\..\SC\INCLUDE\win32\winnls.h  \
		..\..\..\SC\INCLUDE\wincon.h ..\..\..\SC\INCLUDE\win32\wincon.h ..\..\..\SC\INCLUDE\winver.h ..\..\..\SC\INCLUDE\win32\winver.h  \
		..\..\..\SC\INCLUDE\winreg.h ..\..\..\SC\INCLUDE\win32\winreg.h ..\..\..\SC\INCLUDE\winnetwk.h ..\..\..\SC\INCLUDE\win32\winnetwk.h  \
		..\..\..\SC\INCLUDE\cderr.h ..\..\..\SC\INCLUDE\win32\cderr.h ..\..\..\SC\INCLUDE\dde.h ..\..\..\SC\INCLUDE\win32\dde.h  \
		..\..\..\SC\INCLUDE\ddeml.h ..\..\..\SC\INCLUDE\win32\ddeml.h ..\..\..\SC\INCLUDE\dlgs.h ..\..\..\SC\INCLUDE\win32\dlgs.h  \
		..\..\..\SC\INCLUDE\lzexpand.h ..\..\..\SC\INCLUDE\win32\lzexpand.h ..\..\..\SC\INCLUDE\mmsystem.h ..\..\..\SC\INCLUDE\win32\mmsystem.h  \
		..\..\..\SC\INCLUDE\nb30.h ..\..\..\SC\INCLUDE\win32\nb30.h ..\..\..\SC\INCLUDE\rpc.h ..\..\..\SC\INCLUDE\win32\rpc.h  \
		..\..\..\SC\INCLUDE\rpcbase.h ..\..\..\SC\INCLUDE\win32\rpcbase.h ..\..\..\SC\INCLUDE\rpcdce.h ..\..\..\SC\INCLUDE\win32\rpcdce.h  \
		..\..\..\SC\INCLUDE\rpcdcep.h ..\..\..\SC\INCLUDE\win32\rpcdcep.h ..\..\..\SC\INCLUDE\rpcnsi.h ..\..\..\SC\INCLUDE\win32\rpcnsi.h  \
		..\..\..\SC\INCLUDE\rpcnterr.h ..\..\..\SC\INCLUDE\win32\rpcnterr.h ..\..\..\SC\INCLUDE\shellapi.h ..\..\..\SC\INCLUDE\win32\shellapi.h  \
		..\..\..\SC\INCLUDE\winperf.h ..\..\..\SC\INCLUDE\win32\winperf.h ..\..\..\SC\INCLUDE\winsock.h ..\..\..\SC\INCLUDE\win32\winsock.h  \
		..\..\..\SC\INCLUDE\commdlg.h ..\..\..\SC\INCLUDE\win32\commdlg.h ..\..\..\SC\INCLUDE\winspool.h ..\..\..\SC\INCLUDE\win32\winspool.h  \
		..\..\..\SC\INCLUDE\prsht.h ..\..\..\SC\INCLUDE\win32\prsht.h ..\..\..\SC\INCLUDE\ole2.h ..\..\..\SC\INCLUDE\win32\ole2.h  \
		..\..\..\SC\INCLUDE\pshpack8.h ..\..\..\SC\INCLUDE\win32\pshpack8.h ..\..\..\SC\INCLUDE\objbase.h ..\..\..\SC\INCLUDE\win32\objbase.h  \
		..\..\..\SC\INCLUDE\rpcndr.h ..\..\..\SC\INCLUDE\win32\rpcndr.h ..\..\..\SC\INCLUDE\rpcnsip.h ..\..\..\SC\INCLUDE\win32\rpcnsip.h  \
		..\..\..\SC\INCLUDE\stdlib.h ..\..\..\SC\INCLUDE\wtypes.h ..\..\..\SC\INCLUDE\win32\wtypes.h ..\..\..\SC\INCLUDE\unknwn.h  \
		..\..\..\SC\INCLUDE\win32\unknwn.h ..\..\..\SC\INCLUDE\objidl.h ..\..\..\SC\INCLUDE\win32\objidl.h ..\..\..\SC\INCLUDE\cguid.h  \
		..\..\..\SC\INCLUDE\win32\cguid.h ..\..\..\SC\INCLUDE\oleauto.h ..\..\..\SC\INCLUDE\win32\oleauto.h ..\..\..\SC\INCLUDE\oaidl.h  \
		..\..\..\SC\INCLUDE\win32\oaidl.h ..\..\..\SC\INCLUDE\oleidl.h ..\..\..\SC\INCLUDE\win32\oleidl.h ..\..\..\SC\INCLUDE\winsvc.h  \
		..\..\..\SC\INCLUDE\win32\winsvc.h ..\..\..\SC\INCLUDE\mcx.h ..\..\..\SC\INCLUDE\win32\mcx.h ..\..\..\SC\INCLUDE\imm.h  \
		..\..\..\SC\INCLUDE\win32\imm.h ..\..\..\SC\INCLUDE\stdio.h ..\..\..\SC\INCLUDE\time.h ..\..\..\SC\INCLUDE\limits.h  \
		..\..\..\SC\INCLUDE\stddef.h ..\..\..\SC\INCLUDE\crtdbg.h ..\..\..\SC\MFC\INCLUDE\afxcoll.h ..\..\..\SC\MFC\INCLUDE\32-bit\afxcoll.h  \
		..\..\..\SC\MFC\INCLUDE\afxstat_.h ..\..\..\SC\MFC\INCLUDE\32-bit\afxstat_.h ..\..\..\SC\MFC\INCLUDE\afxtls_.h ..\..\..\SC\MFC\INCLUDE\32-bit\afxtls_.h  \
		..\..\..\SC\INCLUDE\commctrl.h ..\..\..\SC\INCLUDE\win32\commctrl.h ..\..\..\SC\INCLUDE\tchar.h ..\..\..\SC\MFC\INCLUDE\afxres.h  \
		..\..\..\SC\MFC\INCLUDE\32-bit\afxres.h ..\..\..\SC\MFC\INCLUDE\afxmsg_.h ..\..\..\SC\MFC\INCLUDE\32-bit\afxmsg_.h ..\..\..\SC\MFC\INCLUDE\afxdd_.h  \
		..\..\..\SC\MFC\INCLUDE\32-bit\afxdd_.h ..\..\..\..\mfc\include\32-bit\afxres.h ..\..\..\..\mfc\include\afxres.h ..\..\..\..\include\win32\poppack.h  \
		..\..\..\..\include\win32\pshpack2.h ..\..\..\..\include\win32\pshpack4.h ..\..\..\..\mfc\include\afxwin.h ..\..\..\..\mfc\include\32-bit\afxwin.h  \
		..\..\..\..\mfc\include\afx.h ..\..\..\..\mfc\include\32-bit\afx.h ..\..\..\..\mfc\include\afxver_.h ..\..\..\..\mfc\include\32-bit\afxver_.h  \
		..\..\..\..\mfc\include\afxv_w32.h ..\..\..\..\mfc\include\32-bit\afxv_w32.h ..\..\..\..\include\windows.h ..\..\..\..\include\win32\scdefs.h  \
		..\..\..\..\include\win32\WINDOWS.H ..\..\..\..\include\excpt.h ..\..\..\..\include\stdarg.h ..\..\..\..\include\windef.h  \
		..\..\..\..\include\win32\WINDEF.H ..\..\..\..\include\winnt.h ..\..\..\..\include\win32\WINNT.H ..\..\..\..\include\ctype.h  \
		..\..\..\..\include\pshpack4.h ..\..\..\..\include\poppack.h ..\..\..\..\include\string.h ..\..\..\..\include\winbase.h  \
		..\..\..\..\include\win32\WINBASE.H ..\..\..\..\include\winerror.h ..\..\..\..\include\win32\WINERROR.H ..\..\..\..\include\wingdi.h  \
		..\..\..\..\include\win32\WINGDI.H ..\..\..\..\include\pshpack1.h ..\..\..\..\include\win32\pshpack1.h ..\..\..\..\include\pshpack2.h  \
		..\..\..\..\include\winuser.h ..\..\..\..\include\win32\WINUSER.H ..\..\..\..\include\winnls.h ..\..\..\..\include\win32\WINNLS.H  \
		..\..\..\..\include\wincon.h ..\..\..\..\include\win32\WINCON.H ..\..\..\..\include\winver.h ..\..\..\..\include\win32\WINVER.H  \
		..\..\..\..\include\winreg.h ..\..\..\..\include\win32\WINREG.H ..\..\..\..\include\winnetwk.h ..\..\..\..\include\win32\WINNETWK.H  \
		..\..\..\..\include\winsvc.h ..\..\..\..\include\win32\WINSVC.H ..\..\..\..\include\mcx.h ..\..\..\..\include\win32\MCX.H  \
		..\..\..\..\include\imm.h ..\..\..\..\include\win32\IMM.H ..\..\..\..\include\commctrl.h ..\..\..\..\include\win32\COMMCTRL.H  \
		..\..\..\..\include\prsht.h ..\..\..\..\include\win32\PRSHT.H ..\..\..\..\include\tchar.h ..\..\..\..\include\stdio.h  \
		..\..\..\..\include\stdlib.h ..\..\..\..\include\time.h ..\..\..\..\include\limits.h ..\..\..\..\include\stddef.h  \
		..\..\..\..\include\crtdbg.h ..\..\..\..\mfc\include\afxcoll.h ..\..\..\..\mfc\include\32-bit\afxcoll.h ..\..\..\..\mfc\include\afxstat_.h  \
		..\..\..\..\mfc\include\32-bit\afxstat_.h ..\..\..\..\mfc\include\afxtls_.h ..\..\..\..\mfc\include\32-bit\afxtls_.h ..\..\..\..\include\shellapi.h  \
		..\..\..\..\include\win32\shellapi.h ..\..\..\..\mfc\include\afxmsg_.h ..\..\..\..\mfc\include\32-bit\afxmsg_.h ..\..\..\..\mfc\include\afxdd_.h  \
		..\..\..\..\mfc\include\32-bit\afxdd_.h ..\..\..\..\include\typeinfo.h ..\..\..\..\mfc\include\32-bit\winres.h 

DEFFILE		= trace.def

!IF $(DEBUG)
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -cpp -Ae -mn -C -WD -S -3 -a8 -c -H -HO- -w- -w17 -g 
LFLAGS		=  /CO /NOI /DO /DE /NOPACKF /XN /NT /ENTRY:_DllMainCRTStartup /BAS:268435456 /A:512 /IMPL:$(TARGETDIR)\$$SCW$$.LIB /RC   :trace.RES
DEFINES		= -D_DEBUG -D_DEBUG=1 -D_X86_=1 -D_MT=1 -D_WINDLL=1 -D_USRDLL=1
!ELSE
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -cpp -Ae -Ar -mn -C -WD -S -3 -a8 -c -w- -w17 -g 
LFLAGS		=  /CO /NOI /DO /DE /NOPACKF /XN /NT /ENTRY:_DllMainCRTStartup /BAS:268435456 /A:512 /IMPL:$(TARGETDIR)\$$SCW$$.LIB /RC   :trace.RES
DEFINES		= -D_WINDLL -D_X86_=1 -D_MT=1 -D_WINDLL=1 -D_USRDLL=1
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

OBJS		= trace.OBJ 

RCFILES		=  trace.RES 

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
trace.OBJ
$$SCW$$.DLL
NUL
KERNEL32.LIB GDI32.LIB USER32.LIB 
trace.def;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
trace.OBJ
$$SCW$$.DLL
NUL
KERNEL32.LIB GDI32.LIB USER32.LIB 
trace.def;
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
trace.OBJ
$$SCW$$.DLL
NUL
KERNEL32.LIB GDI32.LIB USER32.LIB 
trace.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
trace.OBJ
$$SCW$$.DLL
NUL
KERNEL32.LIB GDI32.LIB USER32.LIB 
trace.def;
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
		-del TRACE.dpd
		-del $(OBJS)
		-del trace.RES

cleanres:
		-del trace.RES

res:		cleanres $(RCFILES) all


link:
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
trace.OBJ
$$SCW$$.DLL
NUL
KERNEL32.LIB GDI32.LIB USER32.LIB 
trace.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
trace.OBJ
$$SCW$$.DLL
NUL
KERNEL32.LIB GDI32.LIB USER32.LIB 
trace.def;
<<
!ENDIF

		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)
			-del $(TARGETDIR)\$(PROJ).LIB
			-ren $(TARGETDIR)\$$SCW$$.LIB $(PROJ).LIB





!IF EXIST (TRACE.dpd)
!INCLUDE TRACE.dpd
!ENDIF


