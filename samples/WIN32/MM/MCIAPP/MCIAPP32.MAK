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

PROJ		= MCIAPP32
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

HEADERS		= ..\..\..\..\include\win32\pshpack4.h ..\include\verinfo.h ..\..\..\..\include\win32\WINVER.H  \
		..\..\..\..\include\winver.h ..\..\..\..\include\win32\DLGS.H ..\..\..\..\include\dlgs.h ..\..\..\..\include\win32\WINRESRC.H  \
		..\..\..\..\include\winresrc.h ..\..\..\..\include\win32\WINDOWS.H ..\..\..\..\include\win32\scdefs.h ..\..\..\..\include\windows.h  \
		debug.h mciapp.h app.h appport.h  \
		..\..\..\..\include\win32\poppack.h ..\..\..\..\include\win32\pshpack2.h 

DEFFILE		= mciapp32.def

!IF $(DEBUG)
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= comdlg32.lib msacm32.lib shell32.lib  \
		vfw32.lib winmm.lib KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -Jm -Ae -mn -C -WA -S -3 -a8 -c -w- -w2 -w3 -w6 -w7 -g 
LFLAGS		=  /CO /DE /PACKF /XN /NT /ENTRY:WinMainCRTStartup /VERS:1.0 /BAS:4194304 /A:512 /RC   :mciapp.RES
DEFINES		= -DDEBUG -D_WIN32 -D_WIN32_LEAN_AND_MEAN -DNULL=0
!ELSE
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= comdlg32.lib msacm32.lib shell32.lib  \
		vfw32.lib winmm.lib KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -Jm -Ae -mn -o+time -WA -3 -a8 -c -w 
LFLAGS		=  /DE /E /PACKF /XN /NT /ENTRY:WinMainCRTStartup /VERS:1.0 /BAS:4194304 /A:512 /RC   :mciapp.RES
DEFINES		= -DWIN32 -D_WIN32 -DWIN32_LEAN_AND_MEAN -DNULL=0
!ENDIF

HFLAGS		= $(CFLAGS) 
MFLAGS		= MASTERPROJ=$(PROJ) 
LIBFLAGS	=  /C 
RESFLAGS	=  -w- -32 
DEBUGGERFLAGS	=  
AFLAGS		= $(CFLAGS) 
HELPFLAGS	= 

MODEL		= N

PAR		= PROJS BATS OBJS

RCDEFINES	= 

INCLUDES	= -I..\include

INCLUDEDOBJS	= 

OBJS		= app.OBJ appinit.OBJ debug.OBJ  \
		mciapp.OBJ 

RCFILES		=  mciapp.RES 

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
app.OBJ+
appinit.OBJ+
debug.OBJ+
mciapp.OBJ
$$SCW$$.EXE
NUL
comdlg32.lib msacm32.lib shell32.lib vfw32.lib winmm.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
mciapp32.def;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
app.OBJ+
appinit.OBJ+
debug.OBJ+
mciapp.OBJ
$$SCW$$.EXE
NUL
comdlg32.lib msacm32.lib shell32.lib vfw32.lib winmm.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
mciapp32.def;
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
app.OBJ+
appinit.OBJ+
debug.OBJ+
mciapp.OBJ
$$SCW$$.EXE
NUL
comdlg32.lib msacm32.lib shell32.lib vfw32.lib winmm.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
mciapp32.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
app.OBJ+
appinit.OBJ+
debug.OBJ+
mciapp.OBJ
$$SCW$$.EXE
NUL
comdlg32.lib msacm32.lib shell32.lib vfw32.lib winmm.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
mciapp32.def;
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
		-del MCIAPP32.dpd
		-del $(OBJS)
		-del mciapp.RES

cleanres:
		-del mciapp.RES

res:		cleanres $(RCFILES) all


link:
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
app.OBJ+
appinit.OBJ+
debug.OBJ+
mciapp.OBJ
$$SCW$$.EXE
NUL
comdlg32.lib msacm32.lib shell32.lib vfw32.lib winmm.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
mciapp32.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
app.OBJ+
appinit.OBJ+
debug.OBJ+
mciapp.OBJ
$$SCW$$.EXE
NUL
comdlg32.lib msacm32.lib shell32.lib vfw32.lib winmm.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
mciapp32.def;
<<
!ENDIF

		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)




!IF EXIST (MCIAPP32.dpd)
!INCLUDE MCIAPP32.dpd
!ENDIF


