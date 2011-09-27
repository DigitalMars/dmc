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

PROJ		= SPOLY2
APPTYPE		= WINDOWS EXE
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

HEADERS		= ..\..\..\include\win16\scode.h ..\..\..\include\win16\DISPATCH.H ..\..\..\include\dispatch.h  \
		..\..\..\include\win16\OLENLS.H ..\..\..\include\olenls.h ..\..\..\include\win16\MONIKER.H ..\..\..\include\moniker.h  \
		..\..\..\include\win16\STORAGE.H ..\..\..\include\storage.h ..\..\..\include\win16\DVOBJ.H ..\..\..\include\dvobj.h  \
		..\..\..\include\win16\COMPOBJ.H ..\..\..\include\compobj.h ..\..\..\include\string.h ..\..\..\include\win16\OLE2.H  \
		..\..\..\include\ole2.h ..\..\..\include\win16\WINDOWS.H ..\..\..\include\windows.h cpoly.h  \
		cpoint.h statbar.h resource.h spoly.h  \
		cenumpt.h ..\..\..\include\win16\variant.h ..\..\..\include\win16\oleguid.h hostenv.h  \
		clsid.h ..\..\..\include\win16\coguid.h ..\..\..\include\initguid.h ..\..\..\include\win16\initguid.h  \
		..\..\..\include\stdio.h ..\..\..\include\stdarg.h 

DEFFILE		= spoly2.def

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

SYMROOT		= 
SYMS		= 
LIBS		= LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 

CFLAGS		=  -ml -C -WA -S -2 -a2 -c -w- -w2 -w6 -w7 -g 
LFLAGS		=  /CO /DE /PACKF /XU /A:16 /RC  -31 :.\spoly2.RES
DEFINES		= -D_DEBUG=1
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
LIBS		= LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 

CFLAGS		=  -ml -o+time -WA -2 -a2 -c -w- -w2 -w6 -w7 
LFLAGS		=  /DE /PACKF /XU /A:16 /RC  -31 :.\spoly2.RES
DEFINES		= 
!ENDIF

HFLAGS		= $(CFLAGS) 
MFLAGS		= MASTERPROJ=$(PROJ) 
LIBFLAGS	=  /C 
RESFLAGS	=  
DEBUGGERFLAGS	=  
AFLAGS		= $(CFLAGS) 
HELPFLAGS	= 

MODEL		= L

PAR		= PROJS BATS OBJS

RCDEFINES	= 

INCLUDES	= -I.

INCLUDEDOBJS	= 

OBJS		=  $(OUTPUTDIR)\clsid.OBJ  $(OUTPUTDIR)\cenumpt.OBJ  $(OUTPUTDIR)\cpoint.OBJ  \
		 $(OUTPUTDIR)\cpoly.OBJ  $(OUTPUTDIR)\misc.OBJ  $(OUTPUTDIR)\statbar.OBJ  $(OUTPUTDIR)\tdata.OBJ  \
		 $(OUTPUTDIR)\winmain.OBJ 

RCFILES		= $(OUTPUTDIR)\spoly2.RES

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
.\clsid.OBJ+
.\cenumpt.OBJ+
.\cpoint.OBJ+
.\cpoly.OBJ+
.\misc.OBJ+
.\statbar.OBJ+
.\tdata.OBJ+
.\winmain.OBJ
..\bin\$$SCW$$.EXE
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
spoly2.def;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
.\clsid.OBJ+
.\cenumpt.OBJ+
.\cpoint.OBJ+
.\cpoly.OBJ+
.\misc.OBJ+
.\statbar.OBJ+
.\tdata.OBJ+
.\winmain.OBJ
..\bin\$$SCW$$.EXE
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
spoly2.def;
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
.\clsid.OBJ+
.\cenumpt.OBJ+
.\cpoint.OBJ+
.\cpoly.OBJ+
.\misc.OBJ+
.\statbar.OBJ+
.\tdata.OBJ+
.\winmain.OBJ
..\bin\$$SCW$$.EXE
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
spoly2.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\clsid.OBJ+
.\cenumpt.OBJ+
.\cpoint.OBJ+
.\cpoly.OBJ+
.\misc.OBJ+
.\statbar.OBJ+
.\tdata.OBJ+
.\winmain.OBJ
..\bin\$$SCW$$.EXE
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
spoly2.def;
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
		-del SPOLY2.dpd
		-del $(OBJS)
		-del $(OUTPUTDIR)\spoly2.RES

cleanres:
		-del $(OUTPUTDIR)\spoly2.RES

res:		cleanres $(RCFILES) all


link:
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\clsid.OBJ+
.\cenumpt.OBJ+
.\cpoint.OBJ+
.\cpoly.OBJ+
.\misc.OBJ+
.\statbar.OBJ+
.\tdata.OBJ+
.\winmain.OBJ
..\bin\$$SCW$$.EXE
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
spoly2.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
.\clsid.OBJ+
.\cenumpt.OBJ+
.\cpoint.OBJ+
.\cpoly.OBJ+
.\misc.OBJ+
.\statbar.OBJ+
.\tdata.OBJ+
.\winmain.OBJ
..\bin\$$SCW$$.EXE
NUL
.\ LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
spoly2.def;
<<
!ENDIF

		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)




!IF EXIST (SPOLY2.dpd)
!INCLUDE SPOLY2.dpd
!ENDIF



$(OUTPUTDIR)\clsid.OBJ:	clsid.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\clsid.obj clsid.c



$(OUTPUTDIR)\cenumpt.OBJ:	cenumpt.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\cenumpt.obj cenumpt.cpp



$(OUTPUTDIR)\cpoint.OBJ:	cpoint.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\cpoint.obj cpoint.cpp



$(OUTPUTDIR)\cpoly.OBJ:	cpoly.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\cpoly.obj cpoly.cpp



$(OUTPUTDIR)\misc.OBJ:	misc.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\misc.obj misc.cpp



$(OUTPUTDIR)\statbar.OBJ:	statbar.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\statbar.obj statbar.cpp



$(OUTPUTDIR)\tdata.OBJ:	tdata.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\tdata.obj tdata.cpp



$(OUTPUTDIR)\winmain.OBJ:	winmain.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\winmain.obj winmain.cpp



$(OUTPUTDIR)\spoly2.RES:	spoly2.rc
		$(RC) $(RESFLAGS) $(RCDEFINES) $(INCLUDES) -o$(OUTPUTDIR)\spoly2.res spoly2.rc




