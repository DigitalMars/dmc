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

PROJ		= MFCUCASE
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

HEADERS		= ..\..\..\..\INCLUDE\ddeml.h ..\..\..\..\INCLUDE\win32\ddeml.h ..\..\..\..\INCLUDE\dlgs.h  \
		..\..\..\..\INCLUDE\win32\dlgs.h ..\..\..\..\INCLUDE\lzexpand.h ..\..\..\..\INCLUDE\win32\lzexpand.h ..\..\..\..\INCLUDE\mmsystem.h  \
		..\..\..\..\INCLUDE\win32\mmsystem.h ..\..\..\..\INCLUDE\nb30.h ..\..\..\..\INCLUDE\win32\nb30.h ..\..\..\..\INCLUDE\rpc.h  \
		..\..\..\..\INCLUDE\win32\rpc.h ..\..\..\..\INCLUDE\rpcbase.h ..\..\..\..\INCLUDE\win32\rpcbase.h ..\..\..\..\INCLUDE\rpcdce.h  \
		..\..\..\..\INCLUDE\win32\rpcdce.h ..\..\..\..\INCLUDE\rpcdcep.h ..\..\..\..\INCLUDE\win32\rpcdcep.h ..\..\..\..\INCLUDE\rpcnsi.h  \
		..\..\..\..\INCLUDE\win32\rpcnsi.h ..\..\..\..\INCLUDE\rpcnterr.h ..\..\..\..\INCLUDE\win32\rpcnterr.h ..\..\..\..\INCLUDE\shellapi.h  \
		..\..\..\..\INCLUDE\win32\shellapi.h ..\..\..\..\INCLUDE\winperf.h ..\..\..\..\INCLUDE\win32\winperf.h ..\..\..\..\INCLUDE\winsock.h  \
		..\..\..\..\INCLUDE\win32\winsock.h ..\..\..\..\INCLUDE\commdlg.h ..\..\..\..\INCLUDE\win32\commdlg.h ..\..\..\..\INCLUDE\winspool.h  \
		..\..\..\..\INCLUDE\win32\winspool.h ..\..\..\..\INCLUDE\prsht.h ..\..\..\..\INCLUDE\win32\prsht.h ..\..\..\..\INCLUDE\ole2.h  \
		..\..\..\..\INCLUDE\win32\ole2.h ..\..\..\..\INCLUDE\pshpack8.h ..\..\..\..\INCLUDE\win32\pshpack8.h ..\..\..\..\INCLUDE\objbase.h  \
		..\..\..\..\INCLUDE\win32\objbase.h ..\..\..\..\INCLUDE\rpcndr.h ..\..\..\..\INCLUDE\win32\rpcndr.h ..\..\..\..\INCLUDE\rpcnsip.h  \
		..\..\..\..\INCLUDE\win32\rpcnsip.h ..\..\..\..\INCLUDE\stdlib.h ..\..\..\..\INCLUDE\wtypes.h ..\..\..\..\INCLUDE\win32\wtypes.h  \
		..\..\..\..\INCLUDE\unknwn.h ..\..\..\..\INCLUDE\win32\unknwn.h ..\..\..\..\INCLUDE\objidl.h ..\..\..\..\INCLUDE\win32\objidl.h  \
		..\..\..\..\INCLUDE\cguid.h ..\..\..\..\INCLUDE\win32\cguid.h ..\..\..\..\INCLUDE\oleauto.h ..\..\..\..\INCLUDE\win32\oleauto.h  \
		..\..\..\..\INCLUDE\oaidl.h ..\..\..\..\INCLUDE\win32\oaidl.h ..\..\..\..\INCLUDE\oleidl.h ..\..\..\..\INCLUDE\win32\oleidl.h  \
		..\..\..\..\INCLUDE\winsvc.h ..\..\..\..\INCLUDE\win32\winsvc.h ..\..\..\..\INCLUDE\mcx.h ..\..\..\..\INCLUDE\win32\mcx.h  \
		..\..\..\..\INCLUDE\imm.h ..\..\..\..\INCLUDE\win32\imm.h ..\..\..\..\INCLUDE\httpfilt.h ..\..\..\..\INCLUDE\tchar.h  \
		..\..\..\..\INCLUDE\mbstring.h ..\..\..\..\INCLUDE\crtdbg.h mfcucase.h resource.h  \
		..\..\..\..\mfc\include\afxisapi.h ..\..\..\..\mfc\include\32-bit\afxisapi.h ..\..\..\..\include\httpext.h ..\..\..\..\include\win32\scdefs.h  \
		..\..\..\..\include\win32\HTTPEXT.H ..\..\..\..\include\windows.h ..\..\..\..\include\win32\WINDOWS.H ..\..\..\..\include\excpt.h  \
		..\..\..\..\include\stdarg.h ..\..\..\..\include\windef.h ..\..\..\..\include\win32\WINDEF.H ..\..\..\..\include\winnt.h  \
		..\..\..\..\include\win32\WINNT.H ..\..\..\..\include\ctype.h ..\..\..\..\include\pshpack4.h ..\..\..\..\include\win32\pshpack4.h  \
		..\..\..\..\include\poppack.h ..\..\..\..\include\win32\poppack.h ..\..\..\..\include\win32\pshpack2.h ..\..\..\..\include\string.h  \
		..\..\..\..\include\winbase.h ..\..\..\..\include\win32\WINBASE.H ..\..\..\..\include\winerror.h ..\..\..\..\include\win32\WINERROR.H  \
		..\..\..\..\include\wingdi.h ..\..\..\..\include\win32\WINGDI.H ..\..\..\..\include\pshpack1.h ..\..\..\..\include\win32\pshpack1.h  \
		..\..\..\..\include\pshpack2.h ..\..\..\..\include\winuser.h ..\..\..\..\include\win32\WINUSER.H ..\..\..\..\include\winnls.h  \
		..\..\..\..\include\win32\WINNLS.H ..\..\..\..\include\wincon.h ..\..\..\..\include\win32\WINCON.H ..\..\..\..\include\winver.h  \
		..\..\..\..\include\win32\WINVER.H ..\..\..\..\include\winreg.h ..\..\..\..\include\win32\WINREG.H ..\..\..\..\include\winnetwk.h  \
		..\..\..\..\include\win32\WINNETWK.H ..\..\..\..\include\cderr.h ..\..\..\..\include\win32\CDERR.H ..\..\..\..\include\dde.h  \
		..\..\..\..\include\win32\DDE.H ..\..\..\..\include\winsock2.h ..\..\..\..\include\win32\WINSOCK2.H ..\..\..\..\include\mswsock.h  \
		..\..\..\..\include\win32\MSWSOCK.H ..\..\..\..\include\wincrypt.h ..\..\..\..\include\win32\WINCRYPT.H ..\..\..\..\include\win32\HTTPFILT.H  \
		..\..\..\..\mfc\include\32-bit\winres.h ..\..\..\..\mfc\include\32-bit\afxres.h ..\..\..\..\mfc\include\afxres.h 

DEFFILE		= MFCUCASE.DEF

!IF $(DEBUG)
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -cpp -Ae -mn -C -WD -S -NLsndd.lib -3 -a8 -c -w- -w17 -g 
LFLAGS		=  /CO /NOI /DO /DE /NOPACKF /XN /NT /ENTRY:_DllMainCRTStartup /BAS:268435456 /A:512 /IMPL:$(TARGETDIR)\$$SCW$$.LIB /RC   :MFCUCASE.RES
DEFINES		= -D_DEBUG=1 -D_MBCS=1 -D_X86_=1 -D_MT=1 -D_WINDLL=1 -D_USRDLL=1
!ELSE
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -Ae -mn -C -o+time -WD -3 -a8 -c -w- -w17 
LFLAGS		=  /NOI /DO /DE /E /PACKF /XN /NT /ENTRY:_DllMainCRTStartup /BAS:268435456 /A:512 /IMPL:$(TARGETDIR)\$$SCW$$.LIB /RC   :MFCUCASE.RES
DEFINES		= -D_X86_=1 -D_MT=1 -D_WINDLL=1 -D_USRDLL=1 -D_MBCS=1
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

OBJS		= MFCUCASE.OBJ 

RCFILES		=  MFCUCASE.RES 

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
MFCUCASE.OBJ
$$SCW$$.DLL
NUL
KERNEL32.LIB GDI32.LIB USER32.LIB 
MFCUCASE.DEF;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
MFCUCASE.OBJ
$$SCW$$.DLL
NUL
KERNEL32.LIB GDI32.LIB USER32.LIB 
MFCUCASE.DEF;
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
MFCUCASE.OBJ
$$SCW$$.DLL
NUL
KERNEL32.LIB GDI32.LIB USER32.LIB 
MFCUCASE.DEF;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
MFCUCASE.OBJ
$$SCW$$.DLL
NUL
KERNEL32.LIB GDI32.LIB USER32.LIB 
MFCUCASE.DEF;
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
		-del MFCUCASE.dpd
		-del $(OBJS)
		-del MFCUCASE.RES

cleanres:
		-del MFCUCASE.RES

res:		cleanres $(RCFILES) all


link:
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
MFCUCASE.OBJ
$$SCW$$.DLL
NUL
KERNEL32.LIB GDI32.LIB USER32.LIB 
MFCUCASE.DEF;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
MFCUCASE.OBJ
$$SCW$$.DLL
NUL
KERNEL32.LIB GDI32.LIB USER32.LIB 
MFCUCASE.DEF;
<<
!ENDIF

		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)
			-del $(TARGETDIR)\$(PROJ).LIB
			-ren $(TARGETDIR)\$$SCW$$.LIB $(PROJ).LIB





!IF EXIST (MFCUCASE.dpd)
!INCLUDE MFCUCASE.dpd
!ENDIF


