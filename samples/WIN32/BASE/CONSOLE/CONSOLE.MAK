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

PROJ		= CONSOLE
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

HEADERS		= ..\..\..\..\include\windows.h ..\..\..\..\include\win32\scdefs.h ..\..\..\..\include\win32\WINDOWS.H  \
		..\..\..\..\include\excpt.h ..\..\..\..\include\stdarg.h ..\..\..\..\include\windef.h ..\..\..\..\include\win32\WINDEF.H  \
		..\..\..\..\include\winnt.h ..\..\..\..\include\win32\WINNT.H ..\..\..\..\include\ctype.h ..\..\..\..\include\pshpack4.h  \
		..\..\..\..\include\win32\pshpack4.h ..\..\..\..\include\poppack.h ..\..\..\..\include\win32\poppack.h ..\..\..\..\include\win32\pshpack2.h  \
		..\..\..\..\include\string.h ..\..\..\..\include\winbase.h ..\..\..\..\include\win32\WINBASE.H ..\..\..\..\include\winerror.h  \
		..\..\..\..\include\win32\WINERROR.H ..\..\..\..\include\wingdi.h ..\..\..\..\include\win32\WINGDI.H ..\..\..\..\include\pshpack1.h  \
		..\..\..\..\include\win32\pshpack1.h ..\..\..\..\include\pshpack2.h ..\..\..\..\include\winuser.h ..\..\..\..\include\win32\WINUSER.H  \
		..\..\..\..\include\winnls.h ..\..\..\..\include\win32\WINNLS.H ..\..\..\..\include\wincon.h ..\..\..\..\include\win32\WINCON.H  \
		..\..\..\..\include\winver.h ..\..\..\..\include\win32\WINVER.H ..\..\..\..\include\winreg.h ..\..\..\..\include\win32\WINREG.H  \
		..\..\..\..\include\winnetwk.h ..\..\..\..\include\win32\WINNETWK.H ..\..\..\..\include\cderr.h ..\..\..\..\include\win32\CDERR.H  \
		..\..\..\..\include\dde.h ..\..\..\..\include\win32\DDE.H ..\..\..\..\include\ddeml.h ..\..\..\..\include\win32\DDEML.H  \
		..\..\..\..\include\dlgs.h ..\..\..\..\include\win32\DLGS.H ..\..\..\..\include\lzexpand.h ..\..\..\..\include\win32\LZEXPAND.H  \
		..\..\..\..\include\mmsystem.h ..\..\..\..\include\win32\MMSYSTEM.H ..\..\..\..\include\nb30.h ..\..\..\..\include\win32\NB30.H  \
		..\..\..\..\include\rpc.h ..\..\..\..\include\win32\rpc.h ..\..\..\..\include\rpcdce.h ..\..\..\..\include\win32\RPCDCE.H  \
		..\..\..\..\include\rpcdcep.h ..\..\..\..\include\win32\RPCDCEP.H ..\..\..\..\include\rpcnsi.h ..\..\..\..\include\win32\RPCNSI.H  \
		..\..\..\..\include\rpcnterr.h ..\..\..\..\include\win32\RPCNTERR.H ..\..\..\..\include\shellapi.h ..\..\..\..\include\win32\shellapi.h  \
		..\..\..\..\include\winperf.h ..\..\..\..\include\win32\WINPERF.H ..\..\..\..\include\winsock2.h ..\..\..\..\include\win32\WINSOCK2.H  \
		..\..\..\..\include\mswsock.h ..\..\..\..\include\win32\MSWSOCK.H ..\..\..\..\include\wincrypt.h ..\..\..\..\include\win32\WINCRYPT.H  \
		..\..\..\..\include\commdlg.h ..\..\..\..\include\win32\COMMDLG.H ..\..\..\..\include\winspool.h ..\..\..\..\include\win32\WINSPOOL.H  \
		..\..\..\..\include\prsht.h ..\..\..\..\include\win32\PRSHT.H ..\..\..\..\include\ole2.h ..\..\..\..\include\win32\OLE2.H  \
		..\..\..\..\include\pshpack8.h ..\..\..\..\include\win32\pshpack8.h ..\..\..\..\include\objbase.h ..\..\..\..\include\win32\objbase.h  \
		..\..\..\..\include\rpcndr.h ..\..\..\..\include\win32\RPCNDR.H ..\..\..\..\include\rpcnsip.h ..\..\..\..\include\win32\RPCNSIP.H  \
		..\..\..\..\include\stdlib.h ..\..\..\..\include\wtypes.h ..\..\..\..\include\win32\wtypes.h ..\..\..\..\include\unknwn.h  \
		..\..\..\..\include\win32\unknwn.h ..\..\..\..\include\objidl.h ..\..\..\..\include\win32\objidl.h ..\..\..\..\include\cguid.h  \
		..\..\..\..\include\win32\CGUID.H ..\..\..\..\include\oleauto.h ..\..\..\..\include\win32\OLEAUTO.H ..\..\..\..\include\oaidl.h  \
		..\..\..\..\include\win32\oaidl.h ..\..\..\..\include\oleidl.h ..\..\..\..\include\win32\oleidl.h ..\..\..\..\include\winsvc.h  \
		..\..\..\..\include\win32\WINSVC.H ..\..\..\..\include\mcx.h ..\..\..\..\include\win32\MCX.H ..\..\..\..\include\imm.h  \
		..\..\..\..\include\win32\IMM.H ..\..\..\..\include\stdio.h console.h ..\..\..\..\include\conio.h  \
		size.h ..\..\..\..\include\malloc.h ..\..\..\..\include\heapstat.h ..\..\..\..\include\memory.h 

DEFFILE		= CONSOLE.DEF

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

SYMROOT		= 
SYMS		= 
LIBS		= advapi32.lib KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -Jm -mn -C -WA -S -3 -a8 -c -w- -w2 -w3 -w6 -g 
LFLAGS		=  /CO /NOI /DE /PACKF /XN /NT /ENTRY:mainCRTStartup /VERS:1.0 /BAS:4194304 /A:512 /RC   :.\console.RES
DEFINES		= -D_CONSOLE -D_CONSOLE=1
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
LIBS		= advapi32.lib KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -Jm -mn -o+time -WA -3 -a8 -c -w- -w2 -w3 -w6 
LFLAGS		=  /NOI /DE /E /PACKF /XN /NT /ENTRY:mainCRTStartup /VERS:1.0 /BAS:4194304 /A:512 /RC   :.\console.RES
DEFINES		= -D_CONSOLE -D_CONSOLE=1
!ENDIF

HFLAGS		= $(CFLAGS) 
MFLAGS		= MASTERPROJ=$(PROJ) 
LIBFLAGS	=  /C /P:512 
RESFLAGS	=  -32 
DEBUGGERFLAGS	=  
AFLAGS		= $(CFLAGS) 
HELPFLAGS	= 

MODEL		= N

PAR		= PROJS BATS OBJS

RCDEFINES	= 

INCLUDES	= -I.

INCLUDEDOBJS	= 

OBJS		=  $(OUTPUTDIR)\alocfree.OBJ  $(OUTPUTDIR)\codepage.OBJ  $(OUTPUTDIR)\coninfo.OBJ  \
		 $(OUTPUTDIR)\conmode.OBJ  $(OUTPUTDIR)\console.OBJ  $(OUTPUTDIR)\contitle.OBJ  $(OUTPUTDIR)\create.OBJ  \
		 $(OUTPUTDIR)\cursor.OBJ  $(OUTPUTDIR)\fillatt.OBJ  $(OUTPUTDIR)\fillchar.OBJ  $(OUTPUTDIR)\flush.OBJ  \
		 $(OUTPUTDIR)\getlrgst.OBJ  $(OUTPUTDIR)\getnumev.OBJ  $(OUTPUTDIR)\handler.OBJ  $(OUTPUTDIR)\numbut.OBJ  \
		 $(OUTPUTDIR)\readchar.OBJ  $(OUTPUTDIR)\readout.OBJ  $(OUTPUTDIR)\scroll.OBJ  $(OUTPUTDIR)\size.OBJ  \
		 $(OUTPUTDIR)\writein.OBJ 

RCFILES		= $(OUTPUTDIR)\console.RES

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
.\alocfree.OBJ+
.\codepage.OBJ+
.\coninfo.OBJ+
.\conmode.OBJ+
.\console.OBJ+
.\contitle.OBJ+
.\create.OBJ+
.\cursor.OBJ+
.\fillatt.OBJ+
.\fillchar.OBJ+
.\flush.OBJ+
.\getlrgst.OBJ+
.\getnumev.OBJ+
.\handler.OBJ+
.\numbut.OBJ+
.\readchar.OBJ+
.\readout.OBJ+
.\scroll.OBJ+
.\size.OBJ+
.\writein.OBJ
.\$$SCW$$.EXE
NUL
.\ advapi32.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
CONSOLE.DEF;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
.\alocfree.OBJ+
.\codepage.OBJ+
.\coninfo.OBJ+
.\conmode.OBJ+
.\console.OBJ+
.\contitle.OBJ+
.\create.OBJ+
.\cursor.OBJ+
.\fillatt.OBJ+
.\fillchar.OBJ+
.\flush.OBJ+
.\getlrgst.OBJ+
.\getnumev.OBJ+
.\handler.OBJ+
.\numbut.OBJ+
.\readchar.OBJ+
.\readout.OBJ+
.\scroll.OBJ+
.\size.OBJ+
.\writein.OBJ
.\$$SCW$$.EXE
NUL
.\ advapi32.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
CONSOLE.DEF;
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
.\alocfree.OBJ+
.\codepage.OBJ+
.\coninfo.OBJ+
.\conmode.OBJ+
.\console.OBJ+
.\contitle.OBJ+
.\create.OBJ+
.\cursor.OBJ+
.\fillatt.OBJ+
.\fillchar.OBJ+
.\flush.OBJ+
.\getlrgst.OBJ+
.\getnumev.OBJ+
.\handler.OBJ+
.\numbut.OBJ+
.\readchar.OBJ+
.\readout.OBJ+
.\scroll.OBJ+
.\size.OBJ+
.\writein.OBJ
.\$$SCW$$.EXE
NUL
.\ advapi32.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
CONSOLE.DEF;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\alocfree.OBJ+
.\codepage.OBJ+
.\coninfo.OBJ+
.\conmode.OBJ+
.\console.OBJ+
.\contitle.OBJ+
.\create.OBJ+
.\cursor.OBJ+
.\fillatt.OBJ+
.\fillchar.OBJ+
.\flush.OBJ+
.\getlrgst.OBJ+
.\getnumev.OBJ+
.\handler.OBJ+
.\numbut.OBJ+
.\readchar.OBJ+
.\readout.OBJ+
.\scroll.OBJ+
.\size.OBJ+
.\writein.OBJ
.\$$SCW$$.EXE
NUL
.\ advapi32.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
CONSOLE.DEF;
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
		-del CONSOLE.dpd
		-del $(OBJS)
		-del $(OUTPUTDIR)\console.RES

cleanres:
		-del $(OUTPUTDIR)\console.RES

res:		cleanres $(RCFILES) all


link:
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\alocfree.OBJ+
.\codepage.OBJ+
.\coninfo.OBJ+
.\conmode.OBJ+
.\console.OBJ+
.\contitle.OBJ+
.\create.OBJ+
.\cursor.OBJ+
.\fillatt.OBJ+
.\fillchar.OBJ+
.\flush.OBJ+
.\getlrgst.OBJ+
.\getnumev.OBJ+
.\handler.OBJ+
.\numbut.OBJ+
.\readchar.OBJ+
.\readout.OBJ+
.\scroll.OBJ+
.\size.OBJ+
.\writein.OBJ
.\$$SCW$$.EXE
NUL
.\ advapi32.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
CONSOLE.DEF;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\alocfree.OBJ+
.\codepage.OBJ+
.\coninfo.OBJ+
.\conmode.OBJ+
.\console.OBJ+
.\contitle.OBJ+
.\create.OBJ+
.\cursor.OBJ+
.\fillatt.OBJ+
.\fillchar.OBJ+
.\flush.OBJ+
.\getlrgst.OBJ+
.\getnumev.OBJ+
.\handler.OBJ+
.\numbut.OBJ+
.\readchar.OBJ+
.\readout.OBJ+
.\scroll.OBJ+
.\size.OBJ+
.\writein.OBJ
.\$$SCW$$.EXE
NUL
.\ advapi32.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
CONSOLE.DEF;
<<
!ENDIF

		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)




!IF EXIST (CONSOLE.dpd)
!INCLUDE CONSOLE.dpd
!ENDIF



$(OUTPUTDIR)\alocfree.OBJ:	alocfree.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\alocfree.obj alocfree.c



$(OUTPUTDIR)\codepage.OBJ:	codepage.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\codepage.obj codepage.c



$(OUTPUTDIR)\coninfo.OBJ:	coninfo.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\coninfo.obj coninfo.c



$(OUTPUTDIR)\conmode.OBJ:	conmode.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\conmode.obj conmode.c



$(OUTPUTDIR)\console.OBJ:	console.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\console.obj console.c



$(OUTPUTDIR)\contitle.OBJ:	contitle.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\contitle.obj contitle.c



$(OUTPUTDIR)\create.OBJ:	create.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\create.obj create.c



$(OUTPUTDIR)\cursor.OBJ:	cursor.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\cursor.obj cursor.c



$(OUTPUTDIR)\fillatt.OBJ:	fillatt.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fillatt.obj fillatt.c



$(OUTPUTDIR)\fillchar.OBJ:	fillchar.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fillchar.obj fillchar.c



$(OUTPUTDIR)\flush.OBJ:	flush.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\flush.obj flush.c



$(OUTPUTDIR)\getlrgst.OBJ:	getlrgst.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\getlrgst.obj getlrgst.c



$(OUTPUTDIR)\getnumev.OBJ:	getnumev.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\getnumev.obj getnumev.c



$(OUTPUTDIR)\handler.OBJ:	handler.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\handler.obj handler.c



$(OUTPUTDIR)\numbut.OBJ:	numbut.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\numbut.obj numbut.c



$(OUTPUTDIR)\readchar.OBJ:	readchar.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\readchar.obj readchar.c



$(OUTPUTDIR)\readout.OBJ:	readout.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\readout.obj readout.c



$(OUTPUTDIR)\scroll.OBJ:	scroll.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\scroll.obj scroll.c



$(OUTPUTDIR)\size.OBJ:	size.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\size.obj size.c



$(OUTPUTDIR)\writein.OBJ:	writein.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\writein.obj writein.c



$(OUTPUTDIR)\console.RES:	console.rc
		$(RC) $(RESFLAGS) $(RCDEFINES) $(INCLUDES) -o$(OUTPUTDIR)\console.res console.rc




