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

PROJ		= DEFO2V32
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

HEADERS		= precomp.h ..\..\..\include\windows.h ..\..\..\include\win32\scdefs.h  \
		..\..\..\include\win32\WINDOWS.H ..\..\..\include\excpt.h ..\..\..\include\stdarg.h ..\..\..\include\windef.h  \
		..\..\..\include\win32\WINDEF.H ..\..\..\include\winnt.h ..\..\..\include\win32\WINNT.H ..\..\..\include\ctype.h  \
		..\..\..\include\pshpack4.h ..\..\..\include\win32\pshpack4.h ..\..\..\include\poppack.h ..\..\..\include\win32\poppack.h  \
		..\..\..\include\win32\pshpack2.h ..\..\..\include\string.h ..\..\..\include\winbase.h ..\..\..\include\win32\WINBASE.H  \
		..\..\..\include\winerror.h ..\..\..\include\win32\WINERROR.H ..\..\..\include\wingdi.h ..\..\..\include\win32\WINGDI.H  \
		..\..\..\include\pshpack1.h ..\..\..\include\win32\pshpack1.h ..\..\..\include\pshpack2.h ..\..\..\include\winuser.h  \
		..\..\..\include\win32\WINUSER.H ..\..\..\include\winnls.h ..\..\..\include\win32\WINNLS.H ..\..\..\include\wincon.h  \
		..\..\..\include\win32\WINCON.H ..\..\..\include\winver.h ..\..\..\include\win32\WINVER.H ..\..\..\include\winreg.h  \
		..\..\..\include\win32\WINREG.H ..\..\..\include\winnetwk.h ..\..\..\include\win32\WINNETWK.H ..\..\..\include\ole2.h  \
		..\..\..\include\win32\OLE2.H ..\..\..\include\pshpack8.h ..\..\..\include\win32\pshpack8.h ..\..\..\include\objbase.h  \
		..\..\..\include\win32\objbase.h ..\..\..\include\rpc.h ..\..\..\include\win32\rpc.h ..\..\..\include\rpcdce.h  \
		..\..\..\include\win32\RPCDCE.H ..\..\..\include\rpcdcep.h ..\..\..\include\win32\RPCDCEP.H ..\..\..\include\rpcnsi.h  \
		..\..\..\include\win32\RPCNSI.H ..\..\..\include\rpcnterr.h ..\..\..\include\win32\RPCNTERR.H ..\..\..\include\rpcndr.h  \
		..\..\..\include\win32\RPCNDR.H ..\..\..\include\rpcnsip.h ..\..\..\include\win32\RPCNSIP.H ..\..\..\include\stdlib.h  \
		..\..\..\include\wtypes.h ..\..\..\include\win32\wtypes.h ..\..\..\include\unknwn.h ..\..\..\include\win32\unknwn.h  \
		..\..\..\include\objidl.h ..\..\..\include\win32\objidl.h ..\..\..\include\cguid.h ..\..\..\include\win32\CGUID.H  \
		..\..\..\include\oleauto.h ..\..\..\include\win32\OLEAUTO.H ..\..\..\include\oaidl.h ..\..\..\include\win32\oaidl.h  \
		..\..\..\include\oleidl.h ..\..\..\include\win32\oleidl.h ..\..\..\include\winsvc.h ..\..\..\include\win32\WINSVC.H  \
		..\..\..\include\mcx.h ..\..\..\include\win32\MCX.H ..\..\..\include\imm.h ..\..\..\include\win32\IMM.H  \
		..\..\..\include\windowsx.h ..\..\..\include\win32\WINDOWSX.H ..\..\..\include\tchar.h ..\..\..\include\wchar.h  \
		..\..\..\include\direct.h ..\..\..\include\io.h ..\..\..\include\process.h ..\..\..\include\sys\stat.h  \
		..\..\..\include\stdio.h ..\..\..\include\time.h defo2v.h resource.h  \
		util.h idataobj.h iadvsink.h idisp.h  \
		tofile.h ..\..\..\include\commdlg.h ..\..\..\include\win32\COMMDLG.H ..\..\..\include\math.h 

DEFFILE		= defo2v32.def

!IF $(DEBUG)
OUTPUTDIR	= .
!IF EXIST (.)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $(OUTPUTDIR)\*.* md $(OUTPUTDIR)
!ENDIF
TARGETDIR	= ..\bin
!IF EXIST (..\bin)
CREATETARGETDIR	=
!ELSE
CREATETARGETDIR	= if not exist $(TARGETDIR)\*.* md $(TARGETDIR)
!ENDIF

SYMROOT		=  .\precomp 
SYMS		= $(SYMROOT).SYM 
LIBS		= comdlg32.lib advapi32.lib KERNEL32.LIB GDI32.LIB USER32.LIB SHELL32.LIB OLE32.LIB OLEAUT32.LIB UUID.LIB 

CFLAGS		=  -cpp -mn -C -WD -S -3 -a8 -c -H -HO- -w- -w2 -w6 -w7 -g 
LFLAGS		=  /CO /M /DE /NOPACKF /XN /NT /ENTRY:_DllMainCRTStartup /VERS:1.0 /BAS:1048576 /A:512 /IMPL:$(TARGETDIR)\$$SCW$$.LIB /RC   :.\defo2v.RES
DEFINES		= -DWIN32 -DNULL=0 -DMT -DUNICODE -DDEBUG=1 -D_UNICODE=1
!ELSE
OUTPUTDIR	= .
!IF EXIST (.)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $(OUTPUTDIR)\*.* md $(OUTPUTDIR)
!ENDIF
TARGETDIR	= ..\bin
!IF EXIST (..\bin)
CREATETARGETDIR	=
!ELSE
CREATETARGETDIR	= if not exist $(TARGETDIR)\*.* md $(TARGETDIR)
!ENDIF

SYMROOT		= 
SYMS		= 
LIBS		= comdlg32.lib advapi32.lib KERNEL32.LIB GDI32.LIB USER32.LIB SHELL32.LIB OLE32.LIB OLEAUT32.LIB UUID.LIB 

CFLAGS		=  -mn -o+time -WD -3 -a8 -c -w- -w2 -w6 -w7 
LFLAGS		=  /DE /PACKF /XN /NT /ENTRY:_DllMainCRTStartup /VERS:1.0 /BAS:1048576 /A:512 /IMPL:$(TARGETDIR)\$$SCW$$.LIB /RC   :.\defo2v.RES
DEFINES		= -DUNICODE -D_UNICODE=1
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

INCLUDES	= -I..\include -I.

INCLUDEDOBJS	= 

OBJS		=  $(OUTPUTDIR)\iadvsink.OBJ  $(OUTPUTDIR)\idataobj.OBJ  $(OUTPUTDIR)\idisp.OBJ  \
		 $(OUTPUTDIR)\tofile.OBJ  $(OUTPUTDIR)\util.OBJ 

RCFILES		= $(OUTPUTDIR)\defo2v.RES

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
.\precomp.PCO+
.\iadvsink.OBJ+
.\idataobj.OBJ+
.\idisp.OBJ+
.\tofile.OBJ+
.\util.OBJ
..\bin\$$SCW$$.DLL
DEFO2V32.MAP
..\lib\ comdlg32.lib advapi32.lib KERNEL32.LIB GDI32.LIB USER32.LIB SHELL32.LIB OLE32.LIB OLEAUT32.LIB UUID.LIB 
defo2v32.def;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
.\iadvsink.OBJ+
.\idataobj.OBJ+
.\idisp.OBJ+
.\tofile.OBJ+
.\util.OBJ
..\bin\$$SCW$$.DLL
NUL
.\ comdlg32.lib advapi32.lib KERNEL32.LIB GDI32.LIB USER32.LIB SHELL32.LIB OLE32.LIB OLEAUT32.LIB UUID.LIB 
defo2v32.def;
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
.\precomp.PCO+
.\iadvsink.OBJ+
.\idataobj.OBJ+
.\idisp.OBJ+
.\tofile.OBJ+
.\util.OBJ
..\bin\$$SCW$$.DLL
DEFO2V32.MAP
..\lib\ comdlg32.lib advapi32.lib KERNEL32.LIB GDI32.LIB USER32.LIB SHELL32.LIB OLE32.LIB OLEAUT32.LIB UUID.LIB 
defo2v32.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\iadvsink.OBJ+
.\idataobj.OBJ+
.\idisp.OBJ+
.\tofile.OBJ+
.\util.OBJ
..\bin\$$SCW$$.DLL
NUL
.\ comdlg32.lib advapi32.lib KERNEL32.LIB GDI32.LIB USER32.LIB SHELL32.LIB OLE32.LIB OLEAUT32.LIB UUID.LIB 
defo2v32.def;
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
		-del DEFO2V32.dpd
		-del $(OBJS)
		-del $(SYMROOT).SYM
		-del $(SYMROOT).PCO
		-del $(OUTPUTDIR)\defo2v.RES

cleanres:
		-del $(OUTPUTDIR)\defo2v.RES

res:		cleanres $(RCFILES) all


link:
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\precomp.PCO+
.\iadvsink.OBJ+
.\idataobj.OBJ+
.\idisp.OBJ+
.\tofile.OBJ+
.\util.OBJ
..\bin\$$SCW$$.DLL
DEFO2V32.MAP
..\lib\ comdlg32.lib advapi32.lib KERNEL32.LIB GDI32.LIB USER32.LIB SHELL32.LIB OLE32.LIB OLEAUT32.LIB UUID.LIB 
defo2v32.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\iadvsink.OBJ+
.\idataobj.OBJ+
.\idisp.OBJ+
.\tofile.OBJ+
.\util.OBJ
..\bin\$$SCW$$.DLL
NUL
.\ comdlg32.lib advapi32.lib KERNEL32.LIB GDI32.LIB USER32.LIB SHELL32.LIB OLE32.LIB OLEAUT32.LIB UUID.LIB 
defo2v32.def;
<<
!ENDIF

		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)
			-del $(TARGETDIR)\$(PROJ).LIB
			-ren $(TARGETDIR)\$$SCW$$.LIB $(PROJ).LIB





movelib?Pbat:
		..\bin\movelib.bat 



!IF EXIST (DEFO2V32.dpd)
!INCLUDE DEFO2V32.dpd
!ENDIF



$(OUTPUTDIR)\defo2v.RES:	defo2v.rc
		$(RC) $(RESFLAGS) $(RCDEFINES) $(INCLUDES) -o$(OUTPUTDIR)\defo2v.res defo2v.rc



$(OUTPUTDIR)\iadvsink.OBJ:	iadvsink.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\iadvsink.obj iadvsink.cpp



$(OUTPUTDIR)\idataobj.OBJ:	idataobj.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\idataobj.obj idataobj.cpp



$(OUTPUTDIR)\idisp.OBJ:	idisp.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\idisp.obj idisp.cpp



$(OUTPUTDIR)\tofile.OBJ:	tofile.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\tofile.obj tofile.cpp



$(OUTPUTDIR)\util.OBJ:	util.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\util.obj util.cpp


$(SYMROOT).SYM: precomp.h
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF$(SYMROOT).SYM -o$(SYMROOT).PCO precomp.h



