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

PROJ		= ISVROTL
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

HEADERS		= status.h outlrc.h ..\..\..\include\win16\WINDOWS.H  \
		..\..\..\include\windows.h message.h defguid.h ..\..\..\include\initguid.h  \
		..\include\ctl3d.h outline.h ..\..\..\include\string.h ..\..\..\include\commdlg.h  \
		..\..\..\include\win16\COMMDLG.H ..\..\..\include\ole2.h ..\..\..\include\win16\OLE2.H ..\..\..\include\compobj.h  \
		..\..\..\include\win16\COMPOBJ.H ..\..\..\include\win16\scode.h ..\..\..\include\win16\coguid.h ..\..\..\include\win16\oleguid.h  \
		..\..\..\include\dvobj.h ..\..\..\include\win16\DVOBJ.H ..\..\..\include\storage.h ..\..\..\include\win16\STORAGE.H  \
		..\..\..\include\moniker.h ..\..\..\include\win16\MONIKER.H ..\..\..\include\ole2ui.h ..\..\..\include\win16\OLE2UI.H  \
		..\..\..\include\shellapi.h ..\..\..\include\win16\shellapi.h ..\..\..\include\dlgs.h ..\..\..\include\win16\DLGS.H  \
		..\..\..\include\win16\olestd.h ..\..\..\include\dos.h frametls.h ..\include\bttncur.h  \
		..\include\gizmobar.h heading.h oleoutl.h ..\include\msgfiltr.h  \
		svroutl.h ..\..\..\include\stdlib.h ..\..\..\include\time.h ..\..\..\include\win16\initguid.h  \
		..\..\..\include\ole2ver.h ..\..\..\include\win16\OLE2VER.H ..\..\..\include\print.h ..\..\..\include\win16\PRINT.H 

DEFFILE		= isvrotl.def

!IF $(DEBUG)
OUTPUTDIR	= isvrotl
!IF EXIST (isvrotl)
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

SYMROOT		=  isvrotl\outline 
SYMS		= $(SYMROOT).SYM 
LIBS		= bttncur.lib ctl3d.lib gizmobar.lib  \
		outlui.lib LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 

CFLAGS		=  -ml -C -WA -S -2 -a2 -c -H -HDisvrotl -HO- -w- -w2 -w3 -w6 -w7 -g 
LFLAGS		=  /CO /PAC:65530 /DE /PACKF /XU /A:16 /RC  -31 :isvrotl\isvrotl.RES
DEFINES		= -D_DEBUG=1 -DOLE_SERVER -DINPLACE_SVR
!ELSE
OUTPUTDIR	= isvrotl
!IF EXIST (isvrotl)
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

SYMROOT		=  isvrotl\outline 
SYMS		= $(SYMROOT).SYM 
LIBS		= bttncur.lib ctl3d.lib gizmobar.lib  \
		outlui.lib LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 

CFLAGS		=  -ml -C -o+time -WA -2 -a2 -c -H -HDisvrotl -HO- -w- -w2 -w3 -w6 -w7 -w12 
LFLAGS		=  /PAC:65530 /DE /PACKF /XU /A:16 /RC -k -31 :isvrotl\isvrotl.RES
DEFINES		= -DOLE_SERVER -DINPLACE_SVR
!ENDIF

HFLAGS		= $(CFLAGS) 
MFLAGS		= MASTERPROJ=$(PROJ) 
LIBFLAGS	=  /C /P:512 
RESFLAGS	=  
DEBUGGERFLAGS	=  
AFLAGS		= $(CFLAGS) 
HELPFLAGS	= 

MODEL		= L

PAR		= PROJS BATS OBJS

RCDEFINES	= 

INCLUDES	= -I.

INCLUDEDOBJS	= 

OBJS		=  $(OUTPUTDIR)\classfac.OBJ  $(OUTPUTDIR)\clipbrd.OBJ  $(OUTPUTDIR)\debug.OBJ  \
		 $(OUTPUTDIR)\debug2.OBJ  $(OUTPUTDIR)\dialogs.OBJ  $(OUTPUTDIR)\dragdrop.OBJ  $(OUTPUTDIR)\frametls.OBJ  \
		 $(OUTPUTDIR)\heading.OBJ  $(OUTPUTDIR)\linking.OBJ  $(OUTPUTDIR)\main.OBJ  $(OUTPUTDIR)\memmgr.OBJ  \
		 $(OUTPUTDIR)\oleapp.OBJ  $(OUTPUTDIR)\oledoc.OBJ  $(OUTPUTDIR)\outlapp.OBJ  $(OUTPUTDIR)\outldoc.OBJ  \
		 $(OUTPUTDIR)\outlline.OBJ  $(OUTPUTDIR)\outllist.OBJ  $(OUTPUTDIR)\outlname.OBJ  $(OUTPUTDIR)\outlntbl.OBJ  \
		 $(OUTPUTDIR)\outltxtl.OBJ  $(OUTPUTDIR)\precomp.OBJ  $(OUTPUTDIR)\status.OBJ  $(OUTPUTDIR)\svrbase.OBJ  \
		 $(OUTPUTDIR)\svrinpl.OBJ  $(OUTPUTDIR)\svrpsobj.OBJ 

RCFILES		= $(OUTPUTDIR)\isvrotl.RES

RESFILES	= 

HELPFILES	= 

PROJS		= ..\ole2ui\outlui.prj ..\gizmobar\gizmobar.prj 

BATS		= 

PRECOMPILE	=  ?P?P?Bole2ui?Boutlui?Pprj  ?P?P?Bgizmobar?Bgizmobar?Pprj 

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
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -oisvrotl\$(*B).sym $*.h

.HPP.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -oisvrotl\$(*B).sym $*.hpp

.HXX.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -oisvrotl\$(*B).sym $*.hxx

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
isvrotl\outline.PCO+
isvrotl\classfac.OBJ+
isvrotl\clipbrd.OBJ+
isvrotl\debug.OBJ+
isvrotl\debug2.OBJ+
isvrotl\dialogs.OBJ+
isvrotl\dragdrop.OBJ+
isvrotl\frametls.OBJ+
isvrotl\heading.OBJ+
isvrotl\linking.OBJ+
isvrotl\main.OBJ+
isvrotl\memmgr.OBJ+
isvrotl\oleapp.OBJ+
isvrotl\oledoc.OBJ+
isvrotl\outlapp.OBJ+
isvrotl\outldoc.OBJ+
isvrotl\outlline.OBJ+
isvrotl\outllist.OBJ+
isvrotl\outlname.OBJ+
isvrotl\outlntbl.OBJ+
isvrotl\outltxtl.OBJ+
isvrotl\precomp.OBJ+
isvrotl\status.OBJ+
isvrotl\svrbase.OBJ+
isvrotl\svrinpl.OBJ+
isvrotl\svrpsobj.OBJ
..\bin\$$SCW$$.EXE
NUL
..\lib\ bttncur.lib ctl3d.lib gizmobar.lib outlui.lib LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
isvrotl.def;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
isvrotl\outline.PCO+
isvrotl\classfac.OBJ+
isvrotl\clipbrd.OBJ+
isvrotl\debug.OBJ+
isvrotl\debug2.OBJ+
isvrotl\dialogs.OBJ+
isvrotl\dragdrop.OBJ+
isvrotl\frametls.OBJ+
isvrotl\heading.OBJ+
isvrotl\linking.OBJ+
isvrotl\main.OBJ+
isvrotl\memmgr.OBJ+
isvrotl\oleapp.OBJ+
isvrotl\oledoc.OBJ+
isvrotl\outlapp.OBJ+
isvrotl\outldoc.OBJ+
isvrotl\outlline.OBJ+
isvrotl\outllist.OBJ+
isvrotl\outlname.OBJ+
isvrotl\outlntbl.OBJ+
isvrotl\outltxtl.OBJ+
isvrotl\precomp.OBJ+
isvrotl\status.OBJ+
isvrotl\svrbase.OBJ+
isvrotl\svrinpl.OBJ+
isvrotl\svrpsobj.OBJ
..\bin\$$SCW$$.EXE
NUL
..\lib\ bttncur.lib ctl3d.lib gizmobar.lib outlui.lib LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
isvrotl.def;
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
isvrotl\outline.PCO+
isvrotl\classfac.OBJ+
isvrotl\clipbrd.OBJ+
isvrotl\debug.OBJ+
isvrotl\debug2.OBJ+
isvrotl\dialogs.OBJ+
isvrotl\dragdrop.OBJ+
isvrotl\frametls.OBJ+
isvrotl\heading.OBJ+
isvrotl\linking.OBJ+
isvrotl\main.OBJ+
isvrotl\memmgr.OBJ+
isvrotl\oleapp.OBJ+
isvrotl\oledoc.OBJ+
isvrotl\outlapp.OBJ+
isvrotl\outldoc.OBJ+
isvrotl\outlline.OBJ+
isvrotl\outllist.OBJ+
isvrotl\outlname.OBJ+
isvrotl\outlntbl.OBJ+
isvrotl\outltxtl.OBJ+
isvrotl\precomp.OBJ+
isvrotl\status.OBJ+
isvrotl\svrbase.OBJ+
isvrotl\svrinpl.OBJ+
isvrotl\svrpsobj.OBJ
..\bin\$$SCW$$.EXE
NUL
..\lib\ bttncur.lib ctl3d.lib gizmobar.lib outlui.lib LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
isvrotl.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
isvrotl\outline.PCO+
isvrotl\classfac.OBJ+
isvrotl\clipbrd.OBJ+
isvrotl\debug.OBJ+
isvrotl\debug2.OBJ+
isvrotl\dialogs.OBJ+
isvrotl\dragdrop.OBJ+
isvrotl\frametls.OBJ+
isvrotl\heading.OBJ+
isvrotl\linking.OBJ+
isvrotl\main.OBJ+
isvrotl\memmgr.OBJ+
isvrotl\oleapp.OBJ+
isvrotl\oledoc.OBJ+
isvrotl\outlapp.OBJ+
isvrotl\outldoc.OBJ+
isvrotl\outlline.OBJ+
isvrotl\outllist.OBJ+
isvrotl\outlname.OBJ+
isvrotl\outlntbl.OBJ+
isvrotl\outltxtl.OBJ+
isvrotl\precomp.OBJ+
isvrotl\status.OBJ+
isvrotl\svrbase.OBJ+
isvrotl\svrinpl.OBJ+
isvrotl\svrpsobj.OBJ
..\bin\$$SCW$$.EXE
NUL
..\lib\ bttncur.lib ctl3d.lib gizmobar.lib outlui.lib LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
isvrotl.def;
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
		-del ISVROTL.dpd
		-del $(OBJS)
		-del $(SYMROOT).SYM
		-del $(SYMROOT).PCO
		-del $(OUTPUTDIR)\isvrotl.RES
		cd ..\ole2ui\.
		$(MAKE) -foutlui.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		cd ..\gizmobar\.
		$(MAKE) -fgizmobar.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@

cleanres:
		-del $(OUTPUTDIR)\isvrotl.RES

res:		cleanres $(RCFILES) all


link:
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
isvrotl\outline.PCO+
isvrotl\classfac.OBJ+
isvrotl\clipbrd.OBJ+
isvrotl\debug.OBJ+
isvrotl\debug2.OBJ+
isvrotl\dialogs.OBJ+
isvrotl\dragdrop.OBJ+
isvrotl\frametls.OBJ+
isvrotl\heading.OBJ+
isvrotl\linking.OBJ+
isvrotl\main.OBJ+
isvrotl\memmgr.OBJ+
isvrotl\oleapp.OBJ+
isvrotl\oledoc.OBJ+
isvrotl\outlapp.OBJ+
isvrotl\outldoc.OBJ+
isvrotl\outlline.OBJ+
isvrotl\outllist.OBJ+
isvrotl\outlname.OBJ+
isvrotl\outlntbl.OBJ+
isvrotl\outltxtl.OBJ+
isvrotl\precomp.OBJ+
isvrotl\status.OBJ+
isvrotl\svrbase.OBJ+
isvrotl\svrinpl.OBJ+
isvrotl\svrpsobj.OBJ
..\bin\$$SCW$$.EXE
NUL
..\lib\ bttncur.lib ctl3d.lib gizmobar.lib outlui.lib LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
isvrotl.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
isvrotl\outline.PCO+
isvrotl\classfac.OBJ+
isvrotl\clipbrd.OBJ+
isvrotl\debug.OBJ+
isvrotl\debug2.OBJ+
isvrotl\dialogs.OBJ+
isvrotl\dragdrop.OBJ+
isvrotl\frametls.OBJ+
isvrotl\heading.OBJ+
isvrotl\linking.OBJ+
isvrotl\main.OBJ+
isvrotl\memmgr.OBJ+
isvrotl\oleapp.OBJ+
isvrotl\oledoc.OBJ+
isvrotl\outlapp.OBJ+
isvrotl\outldoc.OBJ+
isvrotl\outlline.OBJ+
isvrotl\outllist.OBJ+
isvrotl\outlname.OBJ+
isvrotl\outlntbl.OBJ+
isvrotl\outltxtl.OBJ+
isvrotl\precomp.OBJ+
isvrotl\status.OBJ+
isvrotl\svrbase.OBJ+
isvrotl\svrinpl.OBJ+
isvrotl\svrpsobj.OBJ
..\bin\$$SCW$$.EXE
NUL
..\lib\ bttncur.lib ctl3d.lib gizmobar.lib outlui.lib LIBW.LIB COMMDLG.LIB SHELL.LIB OLE2.LIB OLECLI.LIB OLESVR.LIB SHELL.LIB OLE2DISP.LIB OLE2NLS.LIB COMPOBJ.LIB STORAGE.LIB TYPELIB.LIB 
isvrotl.def;
<<
!ENDIF

		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)


?P?P?Bole2ui?Boutlui?Pprj:
		cd ..\ole2ui\.
		$(MAKE) -foutlui.mak "SUB_DEBUG=$(DEBUG)"

?P?P?Bgizmobar?Bgizmobar?Pprj:
		cd ..\gizmobar\.
		$(MAKE) -fgizmobar.mak "SUB_DEBUG=$(DEBUG)"





!IF EXIST (ISVROTL.dpd)
!INCLUDE ISVROTL.dpd
!ENDIF



$(OUTPUTDIR)\isvrotl.RES:	isvrotl.rc
		$(RC) $(RESFLAGS) $(RCDEFINES) $(INCLUDES) -o$(OUTPUTDIR)\isvrotl.res isvrotl.rc



$(OUTPUTDIR)\classfac.OBJ:	classfac.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\classfac.obj classfac.c



$(OUTPUTDIR)\clipbrd.OBJ:	clipbrd.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\clipbrd.obj clipbrd.c



$(OUTPUTDIR)\debug.OBJ:	debug.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\debug.obj debug.c



$(OUTPUTDIR)\debug2.OBJ:	debug2.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\debug2.obj debug2.c



$(OUTPUTDIR)\dialogs.OBJ:	dialogs.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dialogs.obj dialogs.c



$(OUTPUTDIR)\dragdrop.OBJ:	dragdrop.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dragdrop.obj dragdrop.c



$(OUTPUTDIR)\frametls.OBJ:	frametls.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\frametls.obj frametls.c



$(OUTPUTDIR)\heading.OBJ:	heading.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\heading.obj heading.c



$(OUTPUTDIR)\linking.OBJ:	linking.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\linking.obj linking.c



$(OUTPUTDIR)\main.OBJ:	main.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\main.obj main.c



$(OUTPUTDIR)\memmgr.OBJ:	memmgr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\memmgr.obj memmgr.c



$(OUTPUTDIR)\oleapp.OBJ:	oleapp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oleapp.obj oleapp.c



$(OUTPUTDIR)\oledoc.OBJ:	oledoc.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledoc.obj oledoc.c



$(OUTPUTDIR)\outlapp.OBJ:	outlapp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\outlapp.obj outlapp.c



$(OUTPUTDIR)\outldoc.OBJ:	outldoc.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\outldoc.obj outldoc.c



$(OUTPUTDIR)\outlline.OBJ:	outlline.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\outlline.obj outlline.c



$(OUTPUTDIR)\outllist.OBJ:	outllist.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\outllist.obj outllist.c



$(OUTPUTDIR)\outlname.OBJ:	outlname.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\outlname.obj outlname.c



$(OUTPUTDIR)\outlntbl.OBJ:	outlntbl.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\outlntbl.obj outlntbl.c



$(OUTPUTDIR)\outltxtl.OBJ:	outltxtl.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\outltxtl.obj outltxtl.c



$(OUTPUTDIR)\precomp.OBJ:	precomp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\precomp.obj precomp.c



$(OUTPUTDIR)\status.OBJ:	status.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\status.obj status.c



$(OUTPUTDIR)\svrbase.OBJ:	svrbase.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\svrbase.obj svrbase.c



$(OUTPUTDIR)\svrinpl.OBJ:	svrinpl.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\svrinpl.obj svrinpl.c



$(OUTPUTDIR)\svrpsobj.OBJ:	svrpsobj.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\svrpsobj.obj svrpsobj.c


$(SYMROOT).SYM: outline.h
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF$(SYMROOT).SYM -o$(SYMROOT).PCO outline.h



