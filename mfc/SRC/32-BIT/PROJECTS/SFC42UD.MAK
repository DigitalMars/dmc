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

PROJ		= SFC42UD
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

HEADERS		= 

DEFFILE		= SFC42UD.def

!IF $(DEBUG)
OUTPUTDIR	= UDLLD.W
!IF EXIST (UDLLD.W)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $(OUTPUTDIR)\*.* md $(OUTPUTDIR)
!ENDIF
TARGETDIR	= ..\..\..\BIN
!IF EXIST (..\..\..\BIN)
CREATETARGETDIR	=
!ELSE
CREATETARGETDIR	= if not exist $(TARGETDIR)\*.* md $(TARGETDIR)
!ENDIF

SYMROOT		=  UDLLD.W\stdafx 
SYMS		= $(SYMROOT).SYM 
LIBS		= uuid.lib uuid2.lib SNDD.LIB KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -cpp -Ae -Ar -mn -C -D_WINDLL -NL -ND -5 -a8 -c -H -HDUDLLD.W -HO- -wx -w- -w2 -w3 -w6 -w7 -w11 -w17 -g 
LFLAGS		=  /CO /NOD /NOI /DE /XN /NT /ENTRY:_DllMainCRTStartup /VERS:4.0 /BAS:0x5F800000 /A:512 /IMPL:$(TARGETDIR)\$$SCW$$.LIB /RC   :UDLLD.W\mfcdll.RES
DEFINES		= -D_DEBUG -D_X86_ -D_AFXDLL -D_AFX_CORE_IMPL -D_MT -D_UNICODE=1
!ELSE
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= uuid.lib uuid2.lib SNDD.LIB KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -mn -o+time -WD -3 -a8 -c 
LFLAGS		=  /NOI /DO /DE /E /PACKF /XN /NT /ENTRY:_DllMainCRTStartup /VERS:1.0 /BAS:268435456 /A:512 /IMPL:$(TARGETDIR)\$$SCW$$.LIB /RC   :..\mfcdll.RES
DEFINES		= -D_UNICODE=1
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

RCDEFINES	= -D_DEBUG -D_UNICODE

INCLUDES	= -I..\..\..\include

INCLUDEDOBJS	= 

OBJS		=  $(OUTPUTDIR)\objcore.OBJ  $(OUTPUTDIR)\except.OBJ  $(OUTPUTDIR)\validadd.OBJ  \
		 $(OUTPUTDIR)\dumpcont.OBJ  $(OUTPUTDIR)\dumpflt.OBJ  $(OUTPUTDIR)\arccore.OBJ  $(OUTPUTDIR)\arcobj.OBJ  \
		 $(OUTPUTDIR)\arcex.OBJ  $(OUTPUTDIR)\arcstrm.OBJ  $(OUTPUTDIR)\dumpinit.OBJ  $(OUTPUTDIR)\dumpout.OBJ  \
		 $(OUTPUTDIR)\afxasert.OBJ  $(OUTPUTDIR)\afxmem.OBJ  $(OUTPUTDIR)\afxabort.OBJ  $(OUTPUTDIR)\afxinl1.OBJ  \
		 $(OUTPUTDIR)\afxinl2.OBJ  $(OUTPUTDIR)\afxinl3.OBJ  $(OUTPUTDIR)\filecore.OBJ  $(OUTPUTDIR)\filetxt.OBJ  \
		 $(OUTPUTDIR)\filemem.OBJ  $(OUTPUTDIR)\fileshrd.OBJ  $(OUTPUTDIR)\filex.OBJ  $(OUTPUTDIR)\filest.OBJ  \
		 $(OUTPUTDIR)\array_b.OBJ  $(OUTPUTDIR)\array_d.OBJ  $(OUTPUTDIR)\array_p.OBJ  $(OUTPUTDIR)\array_o.OBJ  \
		 $(OUTPUTDIR)\array_s.OBJ  $(OUTPUTDIR)\array_u.OBJ  $(OUTPUTDIR)\array_w.OBJ  $(OUTPUTDIR)\list_o.OBJ  \
		 $(OUTPUTDIR)\list_p.OBJ  $(OUTPUTDIR)\list_s.OBJ  $(OUTPUTDIR)\map_pp.OBJ  $(OUTPUTDIR)\map_pw.OBJ  \
		 $(OUTPUTDIR)\map_so.OBJ  $(OUTPUTDIR)\map_sp.OBJ  $(OUTPUTDIR)\map_ss.OBJ  $(OUTPUTDIR)\map_wo.OBJ  \
		 $(OUTPUTDIR)\map_wp.OBJ  $(OUTPUTDIR)\plex.OBJ  $(OUTPUTDIR)\strcore.OBJ  $(OUTPUTDIR)\strex.OBJ  \
		 $(OUTPUTDIR)\timecore.OBJ  $(OUTPUTDIR)\afxdbcs.OBJ  $(OUTPUTDIR)\afxstate.OBJ  $(OUTPUTDIR)\afxtls.OBJ  \
		 $(OUTPUTDIR)\mtcore.OBJ  $(OUTPUTDIR)\mtex.OBJ  $(OUTPUTDIR)\wincore.OBJ  $(OUTPUTDIR)\winfrm.OBJ  \
		 $(OUTPUTDIR)\winfrm2.OBJ  $(OUTPUTDIR)\winfrmx.OBJ  $(OUTPUTDIR)\winmdi.OBJ  $(OUTPUTDIR)\tooltip.OBJ  \
		 $(OUTPUTDIR)\winmini.OBJ  $(OUTPUTDIR)\winhand.OBJ  $(OUTPUTDIR)\winmain.OBJ  $(OUTPUTDIR)\barcore.OBJ  \
		 $(OUTPUTDIR)\bartool.OBJ  $(OUTPUTDIR)\bardlg.OBJ  $(OUTPUTDIR)\barstat.OBJ  $(OUTPUTDIR)\bardock.OBJ  \
		 $(OUTPUTDIR)\dockcont.OBJ  $(OUTPUTDIR)\dockstat.OBJ  $(OUTPUTDIR)\dcprev.OBJ  $(OUTPUTDIR)\dcmeta.OBJ  \
		 $(OUTPUTDIR)\trckrect.OBJ  $(OUTPUTDIR)\winctrl1.OBJ  $(OUTPUTDIR)\winctrl2.OBJ  $(OUTPUTDIR)\winctrl3.OBJ  \
		 $(OUTPUTDIR)\winctrl4.OBJ  $(OUTPUTDIR)\winbtn.OBJ  $(OUTPUTDIR)\dlgcore.OBJ  $(OUTPUTDIR)\dlgdata.OBJ  \
		 $(OUTPUTDIR)\dlgfloat.OBJ  $(OUTPUTDIR)\dlgprop.OBJ  $(OUTPUTDIR)\dlgcomm.OBJ  $(OUTPUTDIR)\dlgfile.OBJ  \
		 $(OUTPUTDIR)\dlgprnt.OBJ  $(OUTPUTDIR)\dlgclr.OBJ  $(OUTPUTDIR)\dlgfnt.OBJ  $(OUTPUTDIR)\dlgfr.OBJ  \
		 $(OUTPUTDIR)\ccdata.OBJ  $(OUTPUTDIR)\dlgtempl.OBJ  $(OUTPUTDIR)\wingdi.OBJ  $(OUTPUTDIR)\wingdix.OBJ  \
		 $(OUTPUTDIR)\winstr.OBJ  $(OUTPUTDIR)\winmenu.OBJ  $(OUTPUTDIR)\auxdata.OBJ  $(OUTPUTDIR)\afxcrit.OBJ  \
		 $(OUTPUTDIR)\afxtrace.OBJ  $(OUTPUTDIR)\winutil.OBJ  $(OUTPUTDIR)\winocc.OBJ  $(OUTPUTDIR)\cmdtarg.OBJ  \
		 $(OUTPUTDIR)\doccore.OBJ  $(OUTPUTDIR)\doctempl.OBJ  $(OUTPUTDIR)\docsingl.OBJ  $(OUTPUTDIR)\docmulti.OBJ  \
		 $(OUTPUTDIR)\docmgr.OBJ  $(OUTPUTDIR)\viewcore.OBJ  $(OUTPUTDIR)\viewprnt.OBJ  $(OUTPUTDIR)\winsplit.OBJ  \
		 $(OUTPUTDIR)\viewscrl.OBJ  $(OUTPUTDIR)\viewform.OBJ  $(OUTPUTDIR)\viewedit.OBJ  $(OUTPUTDIR)\viewprev.OBJ  \
		 $(OUTPUTDIR)\viewcmn.OBJ  $(OUTPUTDIR)\docmapi.OBJ  $(OUTPUTDIR)\thrdcore.OBJ  $(OUTPUTDIR)\appcore.OBJ  \
		 $(OUTPUTDIR)\appinit.OBJ  $(OUTPUTDIR)\appterm.OBJ  $(OUTPUTDIR)\appui.OBJ  $(OUTPUTDIR)\appui2.OBJ  \
		 $(OUTPUTDIR)\appui3.OBJ  $(OUTPUTDIR)\appgray.OBJ  $(OUTPUTDIR)\appdlg.OBJ  $(OUTPUTDIR)\app3d.OBJ  \
		 $(OUTPUTDIR)\appprnt.OBJ  $(OUTPUTDIR)\apphelp.OBJ  $(OUTPUTDIR)\apphelpx.OBJ  $(OUTPUTDIR)\filelist.OBJ  \
		 $(OUTPUTDIR)\olelock.OBJ  $(OUTPUTDIR)\dllinit.OBJ  $(OUTPUTDIR)\inet.OBJ  $(OUTPUTDIR)\inetimpl.OBJ  \
		 $(OUTPUTDIR)\filefind.OBJ 

RCFILES		= $(OUTPUTDIR)\mfcdll.RES

RESFILES	= 

HELPFILES	= 

PROJS		= SFCS42UD.PRJ 

BATS		= 

PRECOMPILE	=  SFCS42UD?PPRJ 

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
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -oUDLLD.W\$(*B).sym $*.h

.HPP.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -oUDLLD.W\$(*B).sym $*.hpp

.HXX.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -oUDLLD.W\$(*B).sym $*.hxx

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
UDLLD.W\stdafx.PCO+
UDLLD.W\objcore.OBJ+
UDLLD.W\except.OBJ+
UDLLD.W\validadd.OBJ+
UDLLD.W\dumpcont.OBJ+
UDLLD.W\dumpflt.OBJ+
UDLLD.W\arccore.OBJ+
UDLLD.W\arcobj.OBJ+
UDLLD.W\arcex.OBJ+
UDLLD.W\arcstrm.OBJ+
UDLLD.W\dumpinit.OBJ+
UDLLD.W\dumpout.OBJ+
UDLLD.W\afxasert.OBJ+
UDLLD.W\afxmem.OBJ+
UDLLD.W\afxabort.OBJ+
UDLLD.W\afxinl1.OBJ+
UDLLD.W\afxinl2.OBJ+
UDLLD.W\afxinl3.OBJ+
UDLLD.W\filecore.OBJ+
UDLLD.W\filetxt.OBJ+
UDLLD.W\filemem.OBJ+
UDLLD.W\fileshrd.OBJ+
UDLLD.W\filex.OBJ+
UDLLD.W\filest.OBJ+
UDLLD.W\array_b.OBJ+
UDLLD.W\array_d.OBJ+
UDLLD.W\array_p.OBJ+
UDLLD.W\array_o.OBJ+
UDLLD.W\array_s.OBJ+
UDLLD.W\array_u.OBJ+
UDLLD.W\array_w.OBJ+
UDLLD.W\list_o.OBJ+
UDLLD.W\list_p.OBJ+
UDLLD.W\list_s.OBJ+
UDLLD.W\map_pp.OBJ+
UDLLD.W\map_pw.OBJ+
UDLLD.W\map_so.OBJ+
UDLLD.W\map_sp.OBJ+
UDLLD.W\map_ss.OBJ+
UDLLD.W\map_wo.OBJ+
UDLLD.W\map_wp.OBJ+
UDLLD.W\plex.OBJ+
UDLLD.W\strcore.OBJ+
UDLLD.W\strex.OBJ+
UDLLD.W\timecore.OBJ+
UDLLD.W\afxdbcs.OBJ+
UDLLD.W\afxstate.OBJ+
UDLLD.W\afxtls.OBJ+
UDLLD.W\mtcore.OBJ+
UDLLD.W\mtex.OBJ+
UDLLD.W\wincore.OBJ+
UDLLD.W\winfrm.OBJ+
UDLLD.W\winfrm2.OBJ+
UDLLD.W\winfrmx.OBJ+
UDLLD.W\winmdi.OBJ+
UDLLD.W\tooltip.OBJ+
UDLLD.W\winmini.OBJ+
UDLLD.W\winhand.OBJ+
UDLLD.W\winmain.OBJ+
UDLLD.W\barcore.OBJ+
UDLLD.W\bartool.OBJ+
UDLLD.W\bardlg.OBJ+
UDLLD.W\barstat.OBJ+
UDLLD.W\bardock.OBJ+
UDLLD.W\dockcont.OBJ+
UDLLD.W\dockstat.OBJ+
UDLLD.W\dcprev.OBJ+
UDLLD.W\dcmeta.OBJ+
UDLLD.W\trckrect.OBJ+
UDLLD.W\winctrl1.OBJ+
UDLLD.W\winctrl2.OBJ+
UDLLD.W\winctrl3.OBJ+
UDLLD.W\winctrl4.OBJ+
UDLLD.W\winbtn.OBJ+
UDLLD.W\dlgcore.OBJ+
UDLLD.W\dlgdata.OBJ+
UDLLD.W\dlgfloat.OBJ+
UDLLD.W\dlgprop.OBJ+
UDLLD.W\dlgcomm.OBJ+
UDLLD.W\dlgfile.OBJ+
UDLLD.W\dlgprnt.OBJ+
UDLLD.W\dlgclr.OBJ+
UDLLD.W\dlgfnt.OBJ+
UDLLD.W\dlgfr.OBJ+
UDLLD.W\ccdata.OBJ+
UDLLD.W\dlgtempl.OBJ+
UDLLD.W\wingdi.OBJ+
UDLLD.W\wingdix.OBJ+
UDLLD.W\winstr.OBJ+
UDLLD.W\winmenu.OBJ+
UDLLD.W\auxdata.OBJ+
UDLLD.W\afxcrit.OBJ+
UDLLD.W\afxtrace.OBJ+
UDLLD.W\winutil.OBJ+
UDLLD.W\winocc.OBJ+
UDLLD.W\cmdtarg.OBJ+
UDLLD.W\doccore.OBJ+
UDLLD.W\doctempl.OBJ+
UDLLD.W\docsingl.OBJ+
UDLLD.W\docmulti.OBJ+
UDLLD.W\docmgr.OBJ+
UDLLD.W\viewcore.OBJ+
UDLLD.W\viewprnt.OBJ+
UDLLD.W\winsplit.OBJ+
UDLLD.W\viewscrl.OBJ+
UDLLD.W\viewform.OBJ+
UDLLD.W\viewedit.OBJ+
UDLLD.W\viewprev.OBJ+
UDLLD.W\viewcmn.OBJ+
UDLLD.W\docmapi.OBJ+
UDLLD.W\thrdcore.OBJ+
UDLLD.W\appcore.OBJ+
UDLLD.W\appinit.OBJ+
UDLLD.W\appterm.OBJ+
UDLLD.W\appui.OBJ+
UDLLD.W\appui2.OBJ+
UDLLD.W\appui3.OBJ+
UDLLD.W\appgray.OBJ+
UDLLD.W\appdlg.OBJ+
UDLLD.W\app3d.OBJ+
UDLLD.W\appprnt.OBJ+
UDLLD.W\apphelp.OBJ+
UDLLD.W\apphelpx.OBJ+
UDLLD.W\filelist.OBJ+
UDLLD.W\olelock.OBJ+
UDLLD.W\dllinit.OBJ+
UDLLD.W\inet.OBJ+
UDLLD.W\inetimpl.OBJ+
UDLLD.W\filefind.OBJ
..\..\..\BIN\$$SCW$$.DLL
NUL
uuid.lib uuid2.lib SNDD.LIB KERNEL32.LIB GDI32.LIB USER32.LIB 
SFC42UD.def;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
..\objcore.OBJ+
..\except.OBJ+
..\validadd.OBJ+
..\dumpcont.OBJ+
..\dumpflt.OBJ+
..\arccore.OBJ+
..\arcobj.OBJ+
..\arcex.OBJ+
..\arcstrm.OBJ+
..\dumpinit.OBJ+
..\dumpout.OBJ+
..\afxasert.OBJ+
..\afxmem.OBJ+
..\afxabort.OBJ+
..\afxinl1.OBJ+
..\afxinl2.OBJ+
..\afxinl3.OBJ+
..\filecore.OBJ+
..\filetxt.OBJ+
..\filemem.OBJ+
..\fileshrd.OBJ+
..\filex.OBJ+
..\filest.OBJ+
..\array_b.OBJ+
..\array_d.OBJ+
..\array_p.OBJ+
..\array_o.OBJ+
..\array_s.OBJ+
..\array_u.OBJ+
..\array_w.OBJ+
..\list_o.OBJ+
..\list_p.OBJ+
..\list_s.OBJ+
..\map_pp.OBJ+
..\map_pw.OBJ+
..\map_so.OBJ+
..\map_sp.OBJ+
..\map_ss.OBJ+
..\map_wo.OBJ+
..\map_wp.OBJ+
..\plex.OBJ+
..\strcore.OBJ+
..\strex.OBJ+
..\timecore.OBJ+
..\afxdbcs.OBJ+
..\afxstate.OBJ+
..\afxtls.OBJ+
..\mtcore.OBJ+
..\mtex.OBJ+
..\wincore.OBJ+
..\winfrm.OBJ+
..\winfrm2.OBJ+
..\winfrmx.OBJ+
..\winmdi.OBJ+
..\tooltip.OBJ+
..\winmini.OBJ+
..\winhand.OBJ+
..\winmain.OBJ+
..\barcore.OBJ+
..\bartool.OBJ+
..\bardlg.OBJ+
..\barstat.OBJ+
..\bardock.OBJ+
..\dockcont.OBJ+
..\dockstat.OBJ+
..\dcprev.OBJ+
..\dcmeta.OBJ+
..\trckrect.OBJ+
..\winctrl1.OBJ+
..\winctrl2.OBJ+
..\winctrl3.OBJ+
..\winctrl4.OBJ+
..\winbtn.OBJ+
..\dlgcore.OBJ+
..\dlgdata.OBJ+
..\dlgfloat.OBJ+
..\dlgprop.OBJ+
..\dlgcomm.OBJ+
..\dlgfile.OBJ+
..\dlgprnt.OBJ+
..\dlgclr.OBJ+
..\dlgfnt.OBJ+
..\dlgfr.OBJ+
..\ccdata.OBJ+
..\dlgtempl.OBJ+
..\wingdi.OBJ+
..\wingdix.OBJ+
..\winstr.OBJ+
..\winmenu.OBJ+
..\auxdata.OBJ+
..\afxcrit.OBJ+
..\afxtrace.OBJ+
..\winutil.OBJ+
..\winocc.OBJ+
..\cmdtarg.OBJ+
..\doccore.OBJ+
..\doctempl.OBJ+
..\docsingl.OBJ+
..\docmulti.OBJ+
..\docmgr.OBJ+
..\viewcore.OBJ+
..\viewprnt.OBJ+
..\winsplit.OBJ+
..\viewscrl.OBJ+
..\viewform.OBJ+
..\viewedit.OBJ+
..\viewprev.OBJ+
..\viewcmn.OBJ+
..\docmapi.OBJ+
..\thrdcore.OBJ+
..\appcore.OBJ+
..\appinit.OBJ+
..\appterm.OBJ+
..\appui.OBJ+
..\appui2.OBJ+
..\appui3.OBJ+
..\appgray.OBJ+
..\appdlg.OBJ+
..\app3d.OBJ+
..\appprnt.OBJ+
..\apphelp.OBJ+
..\apphelpx.OBJ+
..\filelist.OBJ+
..\olelock.OBJ+
..\dllinit.OBJ+
..\inet.OBJ+
..\inetimpl.OBJ+
..\filefind.OBJ
$$SCW$$.DLL
NUL
uuid.lib uuid2.lib SNDD.LIB KERNEL32.LIB GDI32.LIB USER32.LIB 
SFC42UD.def;
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
UDLLD.W\stdafx.PCO+
UDLLD.W\objcore.OBJ+
UDLLD.W\except.OBJ+
UDLLD.W\validadd.OBJ+
UDLLD.W\dumpcont.OBJ+
UDLLD.W\dumpflt.OBJ+
UDLLD.W\arccore.OBJ+
UDLLD.W\arcobj.OBJ+
UDLLD.W\arcex.OBJ+
UDLLD.W\arcstrm.OBJ+
UDLLD.W\dumpinit.OBJ+
UDLLD.W\dumpout.OBJ+
UDLLD.W\afxasert.OBJ+
UDLLD.W\afxmem.OBJ+
UDLLD.W\afxabort.OBJ+
UDLLD.W\afxinl1.OBJ+
UDLLD.W\afxinl2.OBJ+
UDLLD.W\afxinl3.OBJ+
UDLLD.W\filecore.OBJ+
UDLLD.W\filetxt.OBJ+
UDLLD.W\filemem.OBJ+
UDLLD.W\fileshrd.OBJ+
UDLLD.W\filex.OBJ+
UDLLD.W\filest.OBJ+
UDLLD.W\array_b.OBJ+
UDLLD.W\array_d.OBJ+
UDLLD.W\array_p.OBJ+
UDLLD.W\array_o.OBJ+
UDLLD.W\array_s.OBJ+
UDLLD.W\array_u.OBJ+
UDLLD.W\array_w.OBJ+
UDLLD.W\list_o.OBJ+
UDLLD.W\list_p.OBJ+
UDLLD.W\list_s.OBJ+
UDLLD.W\map_pp.OBJ+
UDLLD.W\map_pw.OBJ+
UDLLD.W\map_so.OBJ+
UDLLD.W\map_sp.OBJ+
UDLLD.W\map_ss.OBJ+
UDLLD.W\map_wo.OBJ+
UDLLD.W\map_wp.OBJ+
UDLLD.W\plex.OBJ+
UDLLD.W\strcore.OBJ+
UDLLD.W\strex.OBJ+
UDLLD.W\timecore.OBJ+
UDLLD.W\afxdbcs.OBJ+
UDLLD.W\afxstate.OBJ+
UDLLD.W\afxtls.OBJ+
UDLLD.W\mtcore.OBJ+
UDLLD.W\mtex.OBJ+
UDLLD.W\wincore.OBJ+
UDLLD.W\winfrm.OBJ+
UDLLD.W\winfrm2.OBJ+
UDLLD.W\winfrmx.OBJ+
UDLLD.W\winmdi.OBJ+
UDLLD.W\tooltip.OBJ+
UDLLD.W\winmini.OBJ+
UDLLD.W\winhand.OBJ+
UDLLD.W\winmain.OBJ+
UDLLD.W\barcore.OBJ+
UDLLD.W\bartool.OBJ+
UDLLD.W\bardlg.OBJ+
UDLLD.W\barstat.OBJ+
UDLLD.W\bardock.OBJ+
UDLLD.W\dockcont.OBJ+
UDLLD.W\dockstat.OBJ+
UDLLD.W\dcprev.OBJ+
UDLLD.W\dcmeta.OBJ+
UDLLD.W\trckrect.OBJ+
UDLLD.W\winctrl1.OBJ+
UDLLD.W\winctrl2.OBJ+
UDLLD.W\winctrl3.OBJ+
UDLLD.W\winctrl4.OBJ+
UDLLD.W\winbtn.OBJ+
UDLLD.W\dlgcore.OBJ+
UDLLD.W\dlgdata.OBJ+
UDLLD.W\dlgfloat.OBJ+
UDLLD.W\dlgprop.OBJ+
UDLLD.W\dlgcomm.OBJ+
UDLLD.W\dlgfile.OBJ+
UDLLD.W\dlgprnt.OBJ+
UDLLD.W\dlgclr.OBJ+
UDLLD.W\dlgfnt.OBJ+
UDLLD.W\dlgfr.OBJ+
UDLLD.W\ccdata.OBJ+
UDLLD.W\dlgtempl.OBJ+
UDLLD.W\wingdi.OBJ+
UDLLD.W\wingdix.OBJ+
UDLLD.W\winstr.OBJ+
UDLLD.W\winmenu.OBJ+
UDLLD.W\auxdata.OBJ+
UDLLD.W\afxcrit.OBJ+
UDLLD.W\afxtrace.OBJ+
UDLLD.W\winutil.OBJ+
UDLLD.W\winocc.OBJ+
UDLLD.W\cmdtarg.OBJ+
UDLLD.W\doccore.OBJ+
UDLLD.W\doctempl.OBJ+
UDLLD.W\docsingl.OBJ+
UDLLD.W\docmulti.OBJ+
UDLLD.W\docmgr.OBJ+
UDLLD.W\viewcore.OBJ+
UDLLD.W\viewprnt.OBJ+
UDLLD.W\winsplit.OBJ+
UDLLD.W\viewscrl.OBJ+
UDLLD.W\viewform.OBJ+
UDLLD.W\viewedit.OBJ+
UDLLD.W\viewprev.OBJ+
UDLLD.W\viewcmn.OBJ+
UDLLD.W\docmapi.OBJ+
UDLLD.W\thrdcore.OBJ+
UDLLD.W\appcore.OBJ+
UDLLD.W\appinit.OBJ+
UDLLD.W\appterm.OBJ+
UDLLD.W\appui.OBJ+
UDLLD.W\appui2.OBJ+
UDLLD.W\appui3.OBJ+
UDLLD.W\appgray.OBJ+
UDLLD.W\appdlg.OBJ+
UDLLD.W\app3d.OBJ+
UDLLD.W\appprnt.OBJ+
UDLLD.W\apphelp.OBJ+
UDLLD.W\apphelpx.OBJ+
UDLLD.W\filelist.OBJ+
UDLLD.W\olelock.OBJ+
UDLLD.W\dllinit.OBJ+
UDLLD.W\inet.OBJ+
UDLLD.W\inetimpl.OBJ+
UDLLD.W\filefind.OBJ
..\..\..\BIN\$$SCW$$.DLL
NUL
uuid.lib uuid2.lib SNDD.LIB KERNEL32.LIB GDI32.LIB USER32.LIB 
SFC42UD.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
..\objcore.OBJ+
..\except.OBJ+
..\validadd.OBJ+
..\dumpcont.OBJ+
..\dumpflt.OBJ+
..\arccore.OBJ+
..\arcobj.OBJ+
..\arcex.OBJ+
..\arcstrm.OBJ+
..\dumpinit.OBJ+
..\dumpout.OBJ+
..\afxasert.OBJ+
..\afxmem.OBJ+
..\afxabort.OBJ+
..\afxinl1.OBJ+
..\afxinl2.OBJ+
..\afxinl3.OBJ+
..\filecore.OBJ+
..\filetxt.OBJ+
..\filemem.OBJ+
..\fileshrd.OBJ+
..\filex.OBJ+
..\filest.OBJ+
..\array_b.OBJ+
..\array_d.OBJ+
..\array_p.OBJ+
..\array_o.OBJ+
..\array_s.OBJ+
..\array_u.OBJ+
..\array_w.OBJ+
..\list_o.OBJ+
..\list_p.OBJ+
..\list_s.OBJ+
..\map_pp.OBJ+
..\map_pw.OBJ+
..\map_so.OBJ+
..\map_sp.OBJ+
..\map_ss.OBJ+
..\map_wo.OBJ+
..\map_wp.OBJ+
..\plex.OBJ+
..\strcore.OBJ+
..\strex.OBJ+
..\timecore.OBJ+
..\afxdbcs.OBJ+
..\afxstate.OBJ+
..\afxtls.OBJ+
..\mtcore.OBJ+
..\mtex.OBJ+
..\wincore.OBJ+
..\winfrm.OBJ+
..\winfrm2.OBJ+
..\winfrmx.OBJ+
..\winmdi.OBJ+
..\tooltip.OBJ+
..\winmini.OBJ+
..\winhand.OBJ+
..\winmain.OBJ+
..\barcore.OBJ+
..\bartool.OBJ+
..\bardlg.OBJ+
..\barstat.OBJ+
..\bardock.OBJ+
..\dockcont.OBJ+
..\dockstat.OBJ+
..\dcprev.OBJ+
..\dcmeta.OBJ+
..\trckrect.OBJ+
..\winctrl1.OBJ+
..\winctrl2.OBJ+
..\winctrl3.OBJ+
..\winctrl4.OBJ+
..\winbtn.OBJ+
..\dlgcore.OBJ+
..\dlgdata.OBJ+
..\dlgfloat.OBJ+
..\dlgprop.OBJ+
..\dlgcomm.OBJ+
..\dlgfile.OBJ+
..\dlgprnt.OBJ+
..\dlgclr.OBJ+
..\dlgfnt.OBJ+
..\dlgfr.OBJ+
..\ccdata.OBJ+
..\dlgtempl.OBJ+
..\wingdi.OBJ+
..\wingdix.OBJ+
..\winstr.OBJ+
..\winmenu.OBJ+
..\auxdata.OBJ+
..\afxcrit.OBJ+
..\afxtrace.OBJ+
..\winutil.OBJ+
..\winocc.OBJ+
..\cmdtarg.OBJ+
..\doccore.OBJ+
..\doctempl.OBJ+
..\docsingl.OBJ+
..\docmulti.OBJ+
..\docmgr.OBJ+
..\viewcore.OBJ+
..\viewprnt.OBJ+
..\winsplit.OBJ+
..\viewscrl.OBJ+
..\viewform.OBJ+
..\viewedit.OBJ+
..\viewprev.OBJ+
..\viewcmn.OBJ+
..\docmapi.OBJ+
..\thrdcore.OBJ+
..\appcore.OBJ+
..\appinit.OBJ+
..\appterm.OBJ+
..\appui.OBJ+
..\appui2.OBJ+
..\appui3.OBJ+
..\appgray.OBJ+
..\appdlg.OBJ+
..\app3d.OBJ+
..\appprnt.OBJ+
..\apphelp.OBJ+
..\apphelpx.OBJ+
..\filelist.OBJ+
..\olelock.OBJ+
..\dllinit.OBJ+
..\inet.OBJ+
..\inetimpl.OBJ+
..\filefind.OBJ
$$SCW$$.DLL
NUL
uuid.lib uuid2.lib SNDD.LIB KERNEL32.LIB GDI32.LIB USER32.LIB 
SFC42UD.def;
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
		-del $(TARGETDIR)\$(PROJ).CLE
		-del $(OUTPUTDIR)\SCPH.SYM
		-del $(OBJS)
		-del $(SYMROOT).SYM
		-del $(SYMROOT).PCO
		-del $(OUTPUTDIR)\mfcdll.RES
		$(MAKE) -fsfcs42ud.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@

cleanres:
		-del $(OUTPUTDIR)\mfcdll.RES

res:		cleanres $(RCFILES) all


link:
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
UDLLD.W\stdafx.PCO+
UDLLD.W\objcore.OBJ+
UDLLD.W\except.OBJ+
UDLLD.W\validadd.OBJ+
UDLLD.W\dumpcont.OBJ+
UDLLD.W\dumpflt.OBJ+
UDLLD.W\arccore.OBJ+
UDLLD.W\arcobj.OBJ+
UDLLD.W\arcex.OBJ+
UDLLD.W\arcstrm.OBJ+
UDLLD.W\dumpinit.OBJ+
UDLLD.W\dumpout.OBJ+
UDLLD.W\afxasert.OBJ+
UDLLD.W\afxmem.OBJ+
UDLLD.W\afxabort.OBJ+
UDLLD.W\afxinl1.OBJ+
UDLLD.W\afxinl2.OBJ+
UDLLD.W\afxinl3.OBJ+
UDLLD.W\filecore.OBJ+
UDLLD.W\filetxt.OBJ+
UDLLD.W\filemem.OBJ+
UDLLD.W\fileshrd.OBJ+
UDLLD.W\filex.OBJ+
UDLLD.W\filest.OBJ+
UDLLD.W\array_b.OBJ+
UDLLD.W\array_d.OBJ+
UDLLD.W\array_p.OBJ+
UDLLD.W\array_o.OBJ+
UDLLD.W\array_s.OBJ+
UDLLD.W\array_u.OBJ+
UDLLD.W\array_w.OBJ+
UDLLD.W\list_o.OBJ+
UDLLD.W\list_p.OBJ+
UDLLD.W\list_s.OBJ+
UDLLD.W\map_pp.OBJ+
UDLLD.W\map_pw.OBJ+
UDLLD.W\map_so.OBJ+
UDLLD.W\map_sp.OBJ+
UDLLD.W\map_ss.OBJ+
UDLLD.W\map_wo.OBJ+
UDLLD.W\map_wp.OBJ+
UDLLD.W\plex.OBJ+
UDLLD.W\strcore.OBJ+
UDLLD.W\strex.OBJ+
UDLLD.W\timecore.OBJ+
UDLLD.W\afxdbcs.OBJ+
UDLLD.W\afxstate.OBJ+
UDLLD.W\afxtls.OBJ+
UDLLD.W\mtcore.OBJ+
UDLLD.W\mtex.OBJ+
UDLLD.W\wincore.OBJ+
UDLLD.W\winfrm.OBJ+
UDLLD.W\winfrm2.OBJ+
UDLLD.W\winfrmx.OBJ+
UDLLD.W\winmdi.OBJ+
UDLLD.W\tooltip.OBJ+
UDLLD.W\winmini.OBJ+
UDLLD.W\winhand.OBJ+
UDLLD.W\winmain.OBJ+
UDLLD.W\barcore.OBJ+
UDLLD.W\bartool.OBJ+
UDLLD.W\bardlg.OBJ+
UDLLD.W\barstat.OBJ+
UDLLD.W\bardock.OBJ+
UDLLD.W\dockcont.OBJ+
UDLLD.W\dockstat.OBJ+
UDLLD.W\dcprev.OBJ+
UDLLD.W\dcmeta.OBJ+
UDLLD.W\trckrect.OBJ+
UDLLD.W\winctrl1.OBJ+
UDLLD.W\winctrl2.OBJ+
UDLLD.W\winctrl3.OBJ+
UDLLD.W\winctrl4.OBJ+
UDLLD.W\winbtn.OBJ+
UDLLD.W\dlgcore.OBJ+
UDLLD.W\dlgdata.OBJ+
UDLLD.W\dlgfloat.OBJ+
UDLLD.W\dlgprop.OBJ+
UDLLD.W\dlgcomm.OBJ+
UDLLD.W\dlgfile.OBJ+
UDLLD.W\dlgprnt.OBJ+
UDLLD.W\dlgclr.OBJ+
UDLLD.W\dlgfnt.OBJ+
UDLLD.W\dlgfr.OBJ+
UDLLD.W\ccdata.OBJ+
UDLLD.W\dlgtempl.OBJ+
UDLLD.W\wingdi.OBJ+
UDLLD.W\wingdix.OBJ+
UDLLD.W\winstr.OBJ+
UDLLD.W\winmenu.OBJ+
UDLLD.W\auxdata.OBJ+
UDLLD.W\afxcrit.OBJ+
UDLLD.W\afxtrace.OBJ+
UDLLD.W\winutil.OBJ+
UDLLD.W\winocc.OBJ+
UDLLD.W\cmdtarg.OBJ+
UDLLD.W\doccore.OBJ+
UDLLD.W\doctempl.OBJ+
UDLLD.W\docsingl.OBJ+
UDLLD.W\docmulti.OBJ+
UDLLD.W\docmgr.OBJ+
UDLLD.W\viewcore.OBJ+
UDLLD.W\viewprnt.OBJ+
UDLLD.W\winsplit.OBJ+
UDLLD.W\viewscrl.OBJ+
UDLLD.W\viewform.OBJ+
UDLLD.W\viewedit.OBJ+
UDLLD.W\viewprev.OBJ+
UDLLD.W\viewcmn.OBJ+
UDLLD.W\docmapi.OBJ+
UDLLD.W\thrdcore.OBJ+
UDLLD.W\appcore.OBJ+
UDLLD.W\appinit.OBJ+
UDLLD.W\appterm.OBJ+
UDLLD.W\appui.OBJ+
UDLLD.W\appui2.OBJ+
UDLLD.W\appui3.OBJ+
UDLLD.W\appgray.OBJ+
UDLLD.W\appdlg.OBJ+
UDLLD.W\app3d.OBJ+
UDLLD.W\appprnt.OBJ+
UDLLD.W\apphelp.OBJ+
UDLLD.W\apphelpx.OBJ+
UDLLD.W\filelist.OBJ+
UDLLD.W\olelock.OBJ+
UDLLD.W\dllinit.OBJ+
UDLLD.W\inet.OBJ+
UDLLD.W\inetimpl.OBJ+
UDLLD.W\filefind.OBJ
..\..\..\BIN\$$SCW$$.DLL
NUL
uuid.lib uuid2.lib SNDD.LIB KERNEL32.LIB GDI32.LIB USER32.LIB 
SFC42UD.def;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
..\objcore.OBJ+
..\except.OBJ+
..\validadd.OBJ+
..\dumpcont.OBJ+
..\dumpflt.OBJ+
..\arccore.OBJ+
..\arcobj.OBJ+
..\arcex.OBJ+
..\arcstrm.OBJ+
..\dumpinit.OBJ+
..\dumpout.OBJ+
..\afxasert.OBJ+
..\afxmem.OBJ+
..\afxabort.OBJ+
..\afxinl1.OBJ+
..\afxinl2.OBJ+
..\afxinl3.OBJ+
..\filecore.OBJ+
..\filetxt.OBJ+
..\filemem.OBJ+
..\fileshrd.OBJ+
..\filex.OBJ+
..\filest.OBJ+
..\array_b.OBJ+
..\array_d.OBJ+
..\array_p.OBJ+
..\array_o.OBJ+
..\array_s.OBJ+
..\array_u.OBJ+
..\array_w.OBJ+
..\list_o.OBJ+
..\list_p.OBJ+
..\list_s.OBJ+
..\map_pp.OBJ+
..\map_pw.OBJ+
..\map_so.OBJ+
..\map_sp.OBJ+
..\map_ss.OBJ+
..\map_wo.OBJ+
..\map_wp.OBJ+
..\plex.OBJ+
..\strcore.OBJ+
..\strex.OBJ+
..\timecore.OBJ+
..\afxdbcs.OBJ+
..\afxstate.OBJ+
..\afxtls.OBJ+
..\mtcore.OBJ+
..\mtex.OBJ+
..\wincore.OBJ+
..\winfrm.OBJ+
..\winfrm2.OBJ+
..\winfrmx.OBJ+
..\winmdi.OBJ+
..\tooltip.OBJ+
..\winmini.OBJ+
..\winhand.OBJ+
..\winmain.OBJ+
..\barcore.OBJ+
..\bartool.OBJ+
..\bardlg.OBJ+
..\barstat.OBJ+
..\bardock.OBJ+
..\dockcont.OBJ+
..\dockstat.OBJ+
..\dcprev.OBJ+
..\dcmeta.OBJ+
..\trckrect.OBJ+
..\winctrl1.OBJ+
..\winctrl2.OBJ+
..\winctrl3.OBJ+
..\winctrl4.OBJ+
..\winbtn.OBJ+
..\dlgcore.OBJ+
..\dlgdata.OBJ+
..\dlgfloat.OBJ+
..\dlgprop.OBJ+
..\dlgcomm.OBJ+
..\dlgfile.OBJ+
..\dlgprnt.OBJ+
..\dlgclr.OBJ+
..\dlgfnt.OBJ+
..\dlgfr.OBJ+
..\ccdata.OBJ+
..\dlgtempl.OBJ+
..\wingdi.OBJ+
..\wingdix.OBJ+
..\winstr.OBJ+
..\winmenu.OBJ+
..\auxdata.OBJ+
..\afxcrit.OBJ+
..\afxtrace.OBJ+
..\winutil.OBJ+
..\winocc.OBJ+
..\cmdtarg.OBJ+
..\doccore.OBJ+
..\doctempl.OBJ+
..\docsingl.OBJ+
..\docmulti.OBJ+
..\docmgr.OBJ+
..\viewcore.OBJ+
..\viewprnt.OBJ+
..\winsplit.OBJ+
..\viewscrl.OBJ+
..\viewform.OBJ+
..\viewedit.OBJ+
..\viewprev.OBJ+
..\viewcmn.OBJ+
..\docmapi.OBJ+
..\thrdcore.OBJ+
..\appcore.OBJ+
..\appinit.OBJ+
..\appterm.OBJ+
..\appui.OBJ+
..\appui2.OBJ+
..\appui3.OBJ+
..\appgray.OBJ+
..\appdlg.OBJ+
..\app3d.OBJ+
..\appprnt.OBJ+
..\apphelp.OBJ+
..\apphelpx.OBJ+
..\filelist.OBJ+
..\olelock.OBJ+
..\dllinit.OBJ+
..\inet.OBJ+
..\inetimpl.OBJ+
..\filefind.OBJ
$$SCW$$.DLL
NUL
uuid.lib uuid2.lib SNDD.LIB KERNEL32.LIB GDI32.LIB USER32.LIB 
SFC42UD.def;
<<
!ENDIF

		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)
			-del $(TARGETDIR)\$(PROJ).LIB
			-ren $(TARGETDIR)\$$SCW$$.LIB $(PROJ).LIB



SFCS42UD?PPRJ:
		$(MAKE) -fsfcs42ud.mak "SUB_DEBUG=$(DEBUG)"





movelib?Pbat:
		movelib.bat 



!IF EXIST (SFC42UD.dpd)
!INCLUDE SFC42UD.dpd
!ENDIF



$(OUTPUTDIR)\mfcdll.RES:	..\mfcdll.rc
		$(RC) $(RESFLAGS) $(RCDEFINES) $(INCLUDES) -o$(OUTPUTDIR)\mfcdll.res ..\mfcdll.rc



$(OUTPUTDIR)\objcore.OBJ:	..\objcore.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\objcore.obj ..\objcore.cpp



$(OUTPUTDIR)\except.OBJ:	..\except.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\except.obj ..\except.cpp



$(OUTPUTDIR)\validadd.OBJ:	..\validadd.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\validadd.obj ..\validadd.cpp



$(OUTPUTDIR)\dumpcont.OBJ:	..\dumpcont.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dumpcont.obj ..\dumpcont.cpp



$(OUTPUTDIR)\dumpflt.OBJ:	..\dumpflt.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dumpflt.obj ..\dumpflt.cpp



$(OUTPUTDIR)\arccore.OBJ:	..\arccore.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\arccore.obj ..\arccore.cpp



$(OUTPUTDIR)\arcobj.OBJ:	..\arcobj.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\arcobj.obj ..\arcobj.cpp



$(OUTPUTDIR)\arcex.OBJ:	..\arcex.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\arcex.obj ..\arcex.cpp



$(OUTPUTDIR)\arcstrm.OBJ:	..\arcstrm.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\arcstrm.obj ..\arcstrm.cpp



$(OUTPUTDIR)\dumpinit.OBJ:	..\dumpinit.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dumpinit.obj ..\dumpinit.cpp



$(OUTPUTDIR)\dumpout.OBJ:	..\dumpout.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dumpout.obj ..\dumpout.cpp



$(OUTPUTDIR)\afxasert.OBJ:	..\afxasert.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\afxasert.obj ..\afxasert.cpp



$(OUTPUTDIR)\afxmem.OBJ:	..\afxmem.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\afxmem.obj ..\afxmem.cpp



$(OUTPUTDIR)\afxabort.OBJ:	..\afxabort.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\afxabort.obj ..\afxabort.cpp



$(OUTPUTDIR)\afxinl1.OBJ:	..\afxinl1.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\afxinl1.obj ..\afxinl1.cpp



$(OUTPUTDIR)\afxinl2.OBJ:	..\afxinl2.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\afxinl2.obj ..\afxinl2.cpp



$(OUTPUTDIR)\afxinl3.OBJ:	..\afxinl3.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\afxinl3.obj ..\afxinl3.cpp



$(OUTPUTDIR)\filecore.OBJ:	..\filecore.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\filecore.obj ..\filecore.cpp



$(OUTPUTDIR)\filetxt.OBJ:	..\filetxt.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\filetxt.obj ..\filetxt.cpp



$(OUTPUTDIR)\filemem.OBJ:	..\filemem.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\filemem.obj ..\filemem.cpp



$(OUTPUTDIR)\fileshrd.OBJ:	..\fileshrd.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fileshrd.obj ..\fileshrd.cpp



$(OUTPUTDIR)\filex.OBJ:	..\filex.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\filex.obj ..\filex.cpp



$(OUTPUTDIR)\filest.OBJ:	..\filest.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\filest.obj ..\filest.cpp



$(OUTPUTDIR)\array_b.OBJ:	..\array_b.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\array_b.obj ..\array_b.cpp



$(OUTPUTDIR)\array_d.OBJ:	..\array_d.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\array_d.obj ..\array_d.cpp



$(OUTPUTDIR)\array_p.OBJ:	..\array_p.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\array_p.obj ..\array_p.cpp



$(OUTPUTDIR)\array_o.OBJ:	..\array_o.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\array_o.obj ..\array_o.cpp



$(OUTPUTDIR)\array_s.OBJ:	..\array_s.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\array_s.obj ..\array_s.cpp



$(OUTPUTDIR)\array_u.OBJ:	..\array_u.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\array_u.obj ..\array_u.cpp



$(OUTPUTDIR)\array_w.OBJ:	..\array_w.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\array_w.obj ..\array_w.cpp



$(OUTPUTDIR)\list_o.OBJ:	..\list_o.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\list_o.obj ..\list_o.cpp



$(OUTPUTDIR)\list_p.OBJ:	..\list_p.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\list_p.obj ..\list_p.cpp



$(OUTPUTDIR)\list_s.OBJ:	..\list_s.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\list_s.obj ..\list_s.cpp



$(OUTPUTDIR)\map_pp.OBJ:	..\map_pp.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\map_pp.obj ..\map_pp.cpp



$(OUTPUTDIR)\map_pw.OBJ:	..\map_pw.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\map_pw.obj ..\map_pw.cpp



$(OUTPUTDIR)\map_so.OBJ:	..\map_so.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\map_so.obj ..\map_so.cpp



$(OUTPUTDIR)\map_sp.OBJ:	..\map_sp.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\map_sp.obj ..\map_sp.cpp



$(OUTPUTDIR)\map_ss.OBJ:	..\map_ss.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\map_ss.obj ..\map_ss.cpp



$(OUTPUTDIR)\map_wo.OBJ:	..\map_wo.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\map_wo.obj ..\map_wo.cpp



$(OUTPUTDIR)\map_wp.OBJ:	..\map_wp.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\map_wp.obj ..\map_wp.cpp



$(OUTPUTDIR)\plex.OBJ:	..\plex.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\plex.obj ..\plex.cpp



$(OUTPUTDIR)\strcore.OBJ:	..\strcore.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strcore.obj ..\strcore.cpp



$(OUTPUTDIR)\strex.OBJ:	..\strex.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strex.obj ..\strex.cpp



$(OUTPUTDIR)\timecore.OBJ:	..\timecore.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\timecore.obj ..\timecore.cpp



$(OUTPUTDIR)\afxdbcs.OBJ:	..\afxdbcs.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\afxdbcs.obj ..\afxdbcs.cpp



$(OUTPUTDIR)\afxstate.OBJ:	..\afxstate.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\afxstate.obj ..\afxstate.cpp



$(OUTPUTDIR)\afxtls.OBJ:	..\afxtls.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\afxtls.obj ..\afxtls.cpp



$(OUTPUTDIR)\mtcore.OBJ:	..\mtcore.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\mtcore.obj ..\mtcore.cpp



$(OUTPUTDIR)\mtex.OBJ:	..\mtex.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\mtex.obj ..\mtex.cpp



$(OUTPUTDIR)\wincore.OBJ:	..\wincore.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wincore.obj ..\wincore.cpp



$(OUTPUTDIR)\winfrm.OBJ:	..\winfrm.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\winfrm.obj ..\winfrm.cpp



$(OUTPUTDIR)\winfrm2.OBJ:	..\winfrm2.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\winfrm2.obj ..\winfrm2.cpp



$(OUTPUTDIR)\winfrmx.OBJ:	..\winfrmx.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\winfrmx.obj ..\winfrmx.cpp



$(OUTPUTDIR)\winmdi.OBJ:	..\winmdi.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\winmdi.obj ..\winmdi.cpp



$(OUTPUTDIR)\tooltip.OBJ:	..\tooltip.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\tooltip.obj ..\tooltip.cpp



$(OUTPUTDIR)\winmini.OBJ:	..\winmini.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\winmini.obj ..\winmini.cpp



$(OUTPUTDIR)\winhand.OBJ:	..\winhand.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\winhand.obj ..\winhand.cpp



$(OUTPUTDIR)\winmain.OBJ:	..\winmain.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\winmain.obj ..\winmain.cpp



$(OUTPUTDIR)\barcore.OBJ:	..\barcore.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\barcore.obj ..\barcore.cpp



$(OUTPUTDIR)\bartool.OBJ:	..\bartool.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\bartool.obj ..\bartool.cpp



$(OUTPUTDIR)\bardlg.OBJ:	..\bardlg.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\bardlg.obj ..\bardlg.cpp



$(OUTPUTDIR)\barstat.OBJ:	..\barstat.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\barstat.obj ..\barstat.cpp



$(OUTPUTDIR)\bardock.OBJ:	..\bardock.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\bardock.obj ..\bardock.cpp



$(OUTPUTDIR)\dockcont.OBJ:	..\dockcont.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dockcont.obj ..\dockcont.cpp



$(OUTPUTDIR)\dockstat.OBJ:	..\dockstat.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dockstat.obj ..\dockstat.cpp



$(OUTPUTDIR)\dcprev.OBJ:	..\dcprev.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dcprev.obj ..\dcprev.cpp



$(OUTPUTDIR)\dcmeta.OBJ:	..\dcmeta.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dcmeta.obj ..\dcmeta.cpp



$(OUTPUTDIR)\trckrect.OBJ:	..\trckrect.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\trckrect.obj ..\trckrect.cpp



$(OUTPUTDIR)\winctrl1.OBJ:	..\winctrl1.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\winctrl1.obj ..\winctrl1.cpp



$(OUTPUTDIR)\winctrl2.OBJ:	..\winctrl2.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\winctrl2.obj ..\winctrl2.cpp



$(OUTPUTDIR)\winctrl3.OBJ:	..\winctrl3.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\winctrl3.obj ..\winctrl3.cpp



$(OUTPUTDIR)\winctrl4.OBJ:	..\winctrl4.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\winctrl4.obj ..\winctrl4.cpp



$(OUTPUTDIR)\winbtn.OBJ:	..\winbtn.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\winbtn.obj ..\winbtn.cpp



$(OUTPUTDIR)\dlgcore.OBJ:	..\dlgcore.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dlgcore.obj ..\dlgcore.cpp



$(OUTPUTDIR)\dlgdata.OBJ:	..\dlgdata.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dlgdata.obj ..\dlgdata.cpp



$(OUTPUTDIR)\dlgfloat.OBJ:	..\dlgfloat.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dlgfloat.obj ..\dlgfloat.cpp



$(OUTPUTDIR)\dlgprop.OBJ:	..\dlgprop.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dlgprop.obj ..\dlgprop.cpp



$(OUTPUTDIR)\dlgcomm.OBJ:	..\dlgcomm.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dlgcomm.obj ..\dlgcomm.cpp



$(OUTPUTDIR)\dlgfile.OBJ:	..\dlgfile.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dlgfile.obj ..\dlgfile.cpp



$(OUTPUTDIR)\dlgprnt.OBJ:	..\dlgprnt.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dlgprnt.obj ..\dlgprnt.cpp



$(OUTPUTDIR)\dlgclr.OBJ:	..\dlgclr.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dlgclr.obj ..\dlgclr.cpp



$(OUTPUTDIR)\dlgfnt.OBJ:	..\dlgfnt.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dlgfnt.obj ..\dlgfnt.cpp



$(OUTPUTDIR)\dlgfr.OBJ:	..\dlgfr.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dlgfr.obj ..\dlgfr.cpp



$(OUTPUTDIR)\ccdata.OBJ:	..\ccdata.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ccdata.obj ..\ccdata.cpp



$(OUTPUTDIR)\dlgtempl.OBJ:	..\dlgtempl.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dlgtempl.obj ..\dlgtempl.cpp



$(OUTPUTDIR)\wingdi.OBJ:	..\wingdi.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wingdi.obj ..\wingdi.cpp



$(OUTPUTDIR)\wingdix.OBJ:	..\wingdix.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wingdix.obj ..\wingdix.cpp



$(OUTPUTDIR)\winstr.OBJ:	..\winstr.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\winstr.obj ..\winstr.cpp



$(OUTPUTDIR)\winmenu.OBJ:	..\winmenu.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\winmenu.obj ..\winmenu.cpp



$(OUTPUTDIR)\auxdata.OBJ:	..\auxdata.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\auxdata.obj ..\auxdata.cpp



$(OUTPUTDIR)\afxcrit.OBJ:	..\afxcrit.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\afxcrit.obj ..\afxcrit.cpp



$(OUTPUTDIR)\afxtrace.OBJ:	..\afxtrace.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\afxtrace.obj ..\afxtrace.cpp



$(OUTPUTDIR)\winutil.OBJ:	..\winutil.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\winutil.obj ..\winutil.cpp



$(OUTPUTDIR)\winocc.OBJ:	..\winocc.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\winocc.obj ..\winocc.cpp



$(OUTPUTDIR)\cmdtarg.OBJ:	..\cmdtarg.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\cmdtarg.obj ..\cmdtarg.cpp



$(OUTPUTDIR)\doccore.OBJ:	..\doccore.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\doccore.obj ..\doccore.cpp



$(OUTPUTDIR)\doctempl.OBJ:	..\doctempl.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\doctempl.obj ..\doctempl.cpp



$(OUTPUTDIR)\docsingl.OBJ:	..\docsingl.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\docsingl.obj ..\docsingl.cpp



$(OUTPUTDIR)\docmulti.OBJ:	..\docmulti.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\docmulti.obj ..\docmulti.cpp



$(OUTPUTDIR)\docmgr.OBJ:	..\docmgr.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\docmgr.obj ..\docmgr.cpp



$(OUTPUTDIR)\viewcore.OBJ:	..\viewcore.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\viewcore.obj ..\viewcore.cpp



$(OUTPUTDIR)\viewprnt.OBJ:	..\viewprnt.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\viewprnt.obj ..\viewprnt.cpp



$(OUTPUTDIR)\winsplit.OBJ:	..\winsplit.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\winsplit.obj ..\winsplit.cpp



$(OUTPUTDIR)\viewscrl.OBJ:	..\viewscrl.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\viewscrl.obj ..\viewscrl.cpp



$(OUTPUTDIR)\viewform.OBJ:	..\viewform.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\viewform.obj ..\viewform.cpp



$(OUTPUTDIR)\viewedit.OBJ:	..\viewedit.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\viewedit.obj ..\viewedit.cpp



$(OUTPUTDIR)\viewprev.OBJ:	..\viewprev.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\viewprev.obj ..\viewprev.cpp



$(OUTPUTDIR)\viewcmn.OBJ:	..\viewcmn.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\viewcmn.obj ..\viewcmn.cpp



$(OUTPUTDIR)\docmapi.OBJ:	..\docmapi.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\docmapi.obj ..\docmapi.cpp



$(OUTPUTDIR)\thrdcore.OBJ:	..\thrdcore.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\thrdcore.obj ..\thrdcore.cpp



$(OUTPUTDIR)\appcore.OBJ:	..\appcore.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\appcore.obj ..\appcore.cpp



$(OUTPUTDIR)\appinit.OBJ:	..\appinit.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\appinit.obj ..\appinit.cpp



$(OUTPUTDIR)\appterm.OBJ:	..\appterm.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\appterm.obj ..\appterm.cpp



$(OUTPUTDIR)\appui.OBJ:	..\appui.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\appui.obj ..\appui.cpp



$(OUTPUTDIR)\appui2.OBJ:	..\appui2.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\appui2.obj ..\appui2.cpp



$(OUTPUTDIR)\appui3.OBJ:	..\appui3.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\appui3.obj ..\appui3.cpp



$(OUTPUTDIR)\appgray.OBJ:	..\appgray.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\appgray.obj ..\appgray.cpp



$(OUTPUTDIR)\appdlg.OBJ:	..\appdlg.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\appdlg.obj ..\appdlg.cpp



$(OUTPUTDIR)\app3d.OBJ:	..\app3d.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\app3d.obj ..\app3d.cpp



$(OUTPUTDIR)\appprnt.OBJ:	..\appprnt.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\appprnt.obj ..\appprnt.cpp



$(OUTPUTDIR)\apphelp.OBJ:	..\apphelp.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\apphelp.obj ..\apphelp.cpp



$(OUTPUTDIR)\apphelpx.OBJ:	..\apphelpx.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\apphelpx.obj ..\apphelpx.cpp



$(OUTPUTDIR)\filelist.OBJ:	..\filelist.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\filelist.obj ..\filelist.cpp



$(OUTPUTDIR)\olelock.OBJ:	..\olelock.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olelock.obj ..\olelock.cpp



$(OUTPUTDIR)\dllinit.OBJ:	..\dllinit.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dllinit.obj ..\dllinit.cpp



$(OUTPUTDIR)\inet.OBJ:	..\inet.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\inet.obj ..\inet.cpp



$(OUTPUTDIR)\inetimpl.OBJ:	..\inetimpl.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\inetimpl.obj ..\inetimpl.cpp



$(OUTPUTDIR)\filefind.OBJ:	..\filefind.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\filefind.obj ..\filefind.cpp


$(SYMROOT).SYM: ..\stdafx.h
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF$(SYMROOT).SYM -o$(SYMROOT).PCO ..\stdafx.h



