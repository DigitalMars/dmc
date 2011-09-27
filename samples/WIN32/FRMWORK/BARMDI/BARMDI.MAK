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

PROJ		= BARMDI
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
		..\..\..\..\include\win32\IMM.H ..\..\..\..\include\windowsx.h ..\..\..\..\include\win32\WINDOWSX.H globals.h  \
		resource.h ..\..\..\..\include\commctrl.h ..\..\..\..\include\win32\COMMCTRL.H toolbar.h  \
		statbar.h ..\..\..\..\include\win32\WINRESRC.H ..\..\..\..\include\winresrc.h 

DEFFILE		= barmdi.def

!IF $(DEBUG)
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= comctl32.lib version.lib KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -Jm -HX -mn -C -WA -S -3 -a8 -c -w- -w2 -g 
LFLAGS		=  /CO /NOI /DE /PACKF /XN /NT /ENTRY:WinMainCRTStartup /VERS:1.0 /BAS:4194304 /A:512 /RC   :barmdi.RES
DEFINES		= 
!ELSE
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= comctl32.lib version.lib KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -Jm -HX -mn -o+time -WA -S -3 -a8 -c -w- -w2 
LFLAGS		=  /CO /NOI /DE /PACKF /XN /NT /ENTRY:WinMainCRTStartup /VERS:1.0 /BAS:4194304 /A:512 /RC   :barmdi.RES
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

RCDEFINES	= 

INCLUDES	= 

INCLUDEDOBJS	= 

OBJS		= about.OBJ barmdi.OBJ dispatch.OBJ  \
		init.OBJ mdichild.OBJ misc.OBJ statbar.OBJ  \
		toolbar.OBJ winmain.OBJ 

RCFILES		=  barmdi.RES 

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
about.OBJ+
barmdi.OBJ+
dispatch.OBJ+
init.OBJ+
mdichild.OBJ+
misc.OBJ+
statbar.OBJ+
toolbar.OBJ+
winmain.OBJ
$$SCW$$.EXE
NUL
comctl32.lib version.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
barmdi.def;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
about.OBJ+
barmdi.OBJ+
dispatch.OBJ+
init.OBJ+
mdichild.OBJ+
misc.OBJ+
statbar.OBJ+
toolbar.OBJ+
winmain.OBJ
$$SCW$$.EXE
NUL
comctl32.lib version.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
barmdi.def;
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
about.OBJ+
barmdi.OBJ+
dispatch.OBJ+
init.OBJ+
mdichild.OBJ+
misc.OBJ+
statbar.OBJ+
toolbar.OBJ+
winmain.OBJ
$$SCW$$.EXE
NUL
comctl32.lib version.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
barmdi.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
about.OBJ+
barmdi.OBJ+
dispatch.OBJ+
init.OBJ+
mdichild.OBJ+
misc.OBJ+
statbar.OBJ+
toolbar.OBJ+
winmain.OBJ
$$SCW$$.EXE
NUL
comctl32.lib version.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
barmdi.def;
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
		-del BARMDI.dpd
		-del $(OBJS)
		-del barmdi.RES

cleanres:
		-del barmdi.RES

res:		cleanres $(RCFILES) all


link:
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
about.OBJ+
barmdi.OBJ+
dispatch.OBJ+
init.OBJ+
mdichild.OBJ+
misc.OBJ+
statbar.OBJ+
toolbar.OBJ+
winmain.OBJ
$$SCW$$.EXE
NUL
comctl32.lib version.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
barmdi.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
about.OBJ+
barmdi.OBJ+
dispatch.OBJ+
init.OBJ+
mdichild.OBJ+
misc.OBJ+
statbar.OBJ+
toolbar.OBJ+
winmain.OBJ
$$SCW$$.EXE
NUL
comctl32.lib version.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
barmdi.def;
<<
!ENDIF

		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)




!IF EXIST (BARMDI.dpd)
!INCLUDE BARMDI.dpd
!ENDIF


