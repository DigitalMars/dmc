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

PROJ		= ICWALK32
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
		..\..\..\..\include\winnetwk.h ..\..\..\..\include\win32\WINNETWK.H ..\..\..\..\include\winsvc.h ..\..\..\..\include\win32\WINSVC.H  \
		..\..\..\..\include\mcx.h ..\..\..\..\include\win32\MCX.H ..\..\..\..\include\imm.h ..\..\..\..\include\win32\IMM.H  \
		..\..\..\..\include\windowsx.h ..\..\..\..\include\win32\WINDOWSX.H ..\..\..\..\include\vfw.h ..\..\..\..\include\win32\VFW.H  \
		..\..\..\..\include\mmsystem.h ..\..\..\..\include\win32\MMSYSTEM.H ..\..\..\..\include\win32\pshpack8.h ..\..\..\..\include\mmreg.h  \
		..\..\..\..\include\win32\MMREG.H ..\..\..\..\include\msacm.h ..\..\..\..\include\win32\MSACM.H icwalk.h  \
		..\include\verinfo.h ..\..\..\..\include\win32\DLGS.H ..\..\..\..\include\dlgs.h ..\..\..\..\include\win32\WINRESRC.H  \
		..\..\..\..\include\winresrc.h 

DEFFILE		= icwalk32.def

!IF $(DEBUG)
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= vfw32.lib winmm.lib KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -Jm -Ae -mn -C -WA -S -3 -a8 -c -w- -w2 -w3 -w7 -g 
LFLAGS		=  /CO /DE /PACKF /XN /NT /ENTRY:WinMainCRTStartup /VERS:1.0 /BAS:4194304 /A:512 /RC   :icwalk.RES
DEFINES		= -DWIN32_LEAN_AND_MEAN=1 -DNULL=0 -DWIN32=1 -D_WIN32 -D_X86=1 -D_DEBUG=1
!ELSE
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= vfw32.lib winmm.lib KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -Jm -Ae -mn -C -o+time -WA -S -3 -a8 -c -w- -w2 -w3 -w7 
LFLAGS		=  /CO /DE /NOPACKF /XN /NT /ENTRY:WinMainCRTStartup /VERS:1.0 /BAS:4194304 /A:512 /RC   :icwalk.RES
DEFINES		= -DWIN32_LEAN_AND_MEAN=1 -DNULL=0 -DWIN32=1 -D_WIN32 -D_X86=1 
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

RCDEFINES	= -DWIN32_LEAN_AND_MEAN=1

INCLUDES	= -I..\include

INCLUDEDOBJS	= 

OBJS		= icwalk.OBJ 

RCFILES		=  icwalk.RES 

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
icwalk.OBJ
$$SCW$$.EXE
NUL
vfw32.lib winmm.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
icwalk32.def;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
icwalk.OBJ
$$SCW$$.EXE
NUL
vfw32.lib winmm.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
icwalk32.def;
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
icwalk.OBJ
$$SCW$$.EXE
NUL
vfw32.lib winmm.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
icwalk32.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
icwalk.OBJ
$$SCW$$.EXE
NUL
vfw32.lib winmm.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
icwalk32.def;
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
		-del ICWALK32.dpd
		-del $(OBJS)
		-del icwalk.RES

cleanres:
		-del icwalk.RES

res:		cleanres $(RCFILES) all


link:
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
icwalk.OBJ
$$SCW$$.EXE
NUL
vfw32.lib winmm.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
icwalk32.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
icwalk.OBJ
$$SCW$$.EXE
NUL
vfw32.lib winmm.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
icwalk32.def;
<<
!ENDIF

		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)




!IF EXIST (ICWALK32.dpd)
!INCLUDE ICWALK32.dpd
!ENDIF


