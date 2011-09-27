ORIGIN		= Symantec Café
ORIGIN_VER	= Version 7.22
VERSION		= DEBUG

!IFDEF SUB_DEBUG
DEBUG		= $(SUB_DEBUG)
NDEBUG		= !$(SUB_DEBUG)
!ELSE
DEBUG		= 1
NDEBUG		= 0
!ENDIF

PROJ		= UAFXCWD
APPTYPE		= WIN32 LIB
PROJTYPE	= LIB

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

HEADERS		= ..\stdafx.h ..\..\..\include\afx.h ..\..\..\include\32-bit\afx.h  \
		..\..\..\include\afxver_.h ..\..\..\include\32-bit\afxver_.h ..\..\..\include\afxv_w32.h ..\..\..\include\32-bit\afxv_w32.h  \
		..\..\..\..\..\sdkrtl\750b4\include\windows.h ..\..\..\..\..\sdkrtl\750b4\include\win32\scdefs.h ..\..\..\..\..\sdkrtl\750b4\include\win32\windows.h f:\sc\include\excpt.h  \
		f:\sc\include\stdarg.h ..\..\..\..\..\sdkrtl\750b4\include\windef.h ..\..\..\..\..\sdkrtl\750b4\include\win32\windef.h ..\..\..\..\..\sdkrtl\750b4\include\winnt.h  \
		..\..\..\..\..\sdkrtl\750b4\include\win32\winnt.h f:\sc\include\ctype.h ..\..\..\..\..\sdkrtl\750b4\include\pshpack4.h ..\..\..\..\..\sdkrtl\750b4\include\win32\pshpack4.h  \
		..\..\..\..\..\sdkrtl\750b4\include\poppack.h ..\..\..\..\..\sdkrtl\750b4\include\win32\poppack.h ..\..\..\..\..\sdkrtl\750b4\include\win32\pshpack2.h f:\sc\include\string.h  \
		..\..\..\..\..\sdkrtl\750b4\include\winbase.h ..\..\..\..\..\sdkrtl\750b4\include\win32\winbase.h 

DEFFILE		= uafxcwd.DEF

!IF $(DEBUG)
OUTPUTDIR	= UWD
!IF EXIST (UWD)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $(OUTPUTDIR)\*.* md $(OUTPUTDIR)
!ENDIF
TARGETDIR	= ..\..\..\LIB
!IF EXIST (..\..\..\LIB)
CREATETARGETDIR	=
!ELSE
CREATETARGETDIR	= if not exist $(TARGETDIR)\*.* md $(TARGETDIR)
!ENDIF

SYMROOT		=  UWD\stdafx 
SYMS		= $(SYMROOT).SYM 
LIBS		= 

CFLAGS		=  -cpp -Ae -Ar -mn -C -NL -5 -a8 -c -H -HDUWD -HO- -wx -w- -w2 -w3 -w6 -w7 -w11 -w17 -g 
LFLAGS		=  /CO /NOI /DE /XN /A:512
DEFINES		= -D_X86_ -D_DEBUG -D_MT -D_UNICODE=1
!ELSE
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= 

CFLAGS		=  -mn -o+time -D_WINDOWS -3 -a8 -c 
LFLAGS		=  /NOI /DE /E /PACKF /XN /A:512
DEFINES		= 
!ENDIF

HFLAGS		= $(CFLAGS) 
MFLAGS		= MASTERPROJ=$(PROJ) 
LIBFLAGS	=  /C /P:128 
RESFLAGS	=  -32 
DEBUGGERFLAGS	=  
AFLAGS		= $(CFLAGS) 
HELPFLAGS	= 

MODEL		= N

PAR		= PROJS BATS OBJS

RCDEFINES	= 

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
		 $(OUTPUTDIR)\dlgprnt.OBJ  $(OUTPUTDIR)\dlgclr.OBJ  $(OUTPUTDIR)\dlgfr.OBJ  $(OUTPUTDIR)\ccdata.OBJ  \
		 $(OUTPUTDIR)\dlgtempl.OBJ  $(OUTPUTDIR)\wingdi.OBJ  $(OUTPUTDIR)\wingdix.OBJ  $(OUTPUTDIR)\winstr.OBJ  \
		 $(OUTPUTDIR)\winmenu.OBJ  $(OUTPUTDIR)\auxdata.OBJ  $(OUTPUTDIR)\afxcrit.OBJ  $(OUTPUTDIR)\afxtrace.OBJ  \
		 $(OUTPUTDIR)\winutil.OBJ  $(OUTPUTDIR)\winocc.OBJ  $(OUTPUTDIR)\cmdtarg.OBJ  $(OUTPUTDIR)\doccore.OBJ  \
		 $(OUTPUTDIR)\doctempl.OBJ  $(OUTPUTDIR)\docsingl.OBJ  $(OUTPUTDIR)\docmulti.OBJ  $(OUTPUTDIR)\docmgr.OBJ  \
		 $(OUTPUTDIR)\viewcore.OBJ  $(OUTPUTDIR)\viewprnt.OBJ  $(OUTPUTDIR)\winsplit.OBJ  $(OUTPUTDIR)\viewscrl.OBJ  \
		 $(OUTPUTDIR)\viewform.OBJ  $(OUTPUTDIR)\viewedit.OBJ  $(OUTPUTDIR)\viewprev.OBJ  $(OUTPUTDIR)\viewcmn.OBJ  \
		 $(OUTPUTDIR)\docmapi.OBJ  $(OUTPUTDIR)\thrdcore.OBJ  $(OUTPUTDIR)\appcore.OBJ  $(OUTPUTDIR)\appinit.OBJ  \
		 $(OUTPUTDIR)\appterm.OBJ  $(OUTPUTDIR)\appui.OBJ  $(OUTPUTDIR)\appui2.OBJ  $(OUTPUTDIR)\appui3.OBJ  \
		 $(OUTPUTDIR)\appgray.OBJ  $(OUTPUTDIR)\appdlg.OBJ  $(OUTPUTDIR)\app3d.OBJ  $(OUTPUTDIR)\appprnt.OBJ  \
		 $(OUTPUTDIR)\apphelp.OBJ  $(OUTPUTDIR)\apphelpx.OBJ  $(OUTPUTDIR)\filelist.OBJ  $(OUTPUTDIR)\app3ds.OBJ  \
		 $(OUTPUTDIR)\nolib.OBJ  $(OUTPUTDIR)\appmodul.OBJ  $(OUTPUTDIR)\dllmodul.OBJ  $(OUTPUTDIR)\oleexp.OBJ  \
		 $(OUTPUTDIR)\sockcore.OBJ  $(OUTPUTDIR)\olelock.OBJ  $(OUTPUTDIR)\oleinit.OBJ  $(OUTPUTDIR)\olebar.OBJ  \
		 $(OUTPUTDIR)\olecnvrt.OBJ  $(OUTPUTDIR)\olemon.OBJ  $(OUTPUTDIR)\oledocvw.OBJ  $(OUTPUTDIR)\oledocob.OBJ  \
		 $(OUTPUTDIR)\oledoctg.OBJ  $(OUTPUTDIR)\oledocip.OBJ  $(OUTPUTDIR)\oleasmon.OBJ  $(OUTPUTDIR)\ctlquick.OBJ  \
		 $(OUTPUTDIR)\ctlnownd.OBJ  $(OUTPUTDIR)\olecli1.OBJ  $(OUTPUTDIR)\olecli2.OBJ  $(OUTPUTDIR)\olecli3.OBJ  \
		 $(OUTPUTDIR)\oledobj1.OBJ  $(OUTPUTDIR)\oledobj2.OBJ  $(OUTPUTDIR)\oledisp1.OBJ  $(OUTPUTDIR)\oledisp2.OBJ  \
		 $(OUTPUTDIR)\oledlgs1.OBJ  $(OUTPUTDIR)\oledlgs2.OBJ  $(OUTPUTDIR)\oledlgs3.OBJ  $(OUTPUTDIR)\oledata.OBJ  \
		 $(OUTPUTDIR)\oledoc1.OBJ  $(OUTPUTDIR)\oledoc2.OBJ  $(OUTPUTDIR)\oledrop1.OBJ  $(OUTPUTDIR)\oledrop2.OBJ  \
		 $(OUTPUTDIR)\olemsgf.OBJ  $(OUTPUTDIR)\oleenum.OBJ  $(OUTPUTDIR)\olefact.OBJ  $(OUTPUTDIR)\oleipfrm.OBJ  \
		 $(OUTPUTDIR)\olelink.OBJ  $(OUTPUTDIR)\olemisc.OBJ  $(OUTPUTDIR)\olestrm.OBJ  $(OUTPUTDIR)\olesvr1.OBJ  \
		 $(OUTPUTDIR)\olesvr2.OBJ  $(OUTPUTDIR)\olereg.OBJ  $(OUTPUTDIR)\oletsvr.OBJ  $(OUTPUTDIR)\oleui1.OBJ  \
		 $(OUTPUTDIR)\oleui2.OBJ  $(OUTPUTDIR)\oleunk.OBJ  $(OUTPUTDIR)\oleverb.OBJ  $(OUTPUTDIR)\olecall.OBJ  \
		 $(OUTPUTDIR)\olevar.OBJ  $(OUTPUTDIR)\viewrich.OBJ  $(OUTPUTDIR)\oledll.OBJ  $(OUTPUTDIR)\oletyplb.OBJ  \
		 $(OUTPUTDIR)\occmgr.OBJ  $(OUTPUTDIR)\occevent.OBJ  $(OUTPUTDIR)\occcont.OBJ  $(OUTPUTDIR)\occsite.OBJ  \
		 $(OUTPUTDIR)\occlock.OBJ  $(OUTPUTDIR)\occddx.OBJ  $(OUTPUTDIR)\occddxf.OBJ  $(OUTPUTDIR)\occdlg.OBJ  \
		 $(OUTPUTDIR)\dbcore.OBJ  $(OUTPUTDIR)\dbrfx.OBJ  $(OUTPUTDIR)\dbview.OBJ  $(OUTPUTDIR)\dbflt.OBJ  \
		 $(OUTPUTDIR)\dblong.OBJ  $(OUTPUTDIR)\dbvar.OBJ  $(OUTPUTDIR)\dlgfnt.OBJ  $(OUTPUTDIR)\daocore.OBJ  \
		 $(OUTPUTDIR)\daodfx.OBJ  $(OUTPUTDIR)\daoview.OBJ  $(OUTPUTDIR)\inet.OBJ  $(OUTPUTDIR)\inetimpl.OBJ  \
		 $(OUTPUTDIR)\filefind.OBJ 

RCFILES		= 

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
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -oUWD\$(*B).sym $*.h

.HPP.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -oUWD\$(*B).sym $*.hpp

.HXX.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -oUWD\$(*B).sym $*.hxx

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
UWD\stdafx.PCO+
UWD\objcore.OBJ+
UWD\except.OBJ+
UWD\validadd.OBJ+
UWD\dumpcont.OBJ+
UWD\dumpflt.OBJ+
UWD\arccore.OBJ+
UWD\arcobj.OBJ+
UWD\arcex.OBJ+
UWD\arcstrm.OBJ+
UWD\dumpinit.OBJ+
UWD\dumpout.OBJ+
UWD\afxasert.OBJ+
UWD\afxmem.OBJ+
UWD\afxabort.OBJ+
UWD\afxinl1.OBJ+
UWD\afxinl2.OBJ+
UWD\afxinl3.OBJ+
UWD\filecore.OBJ+
UWD\filetxt.OBJ+
UWD\filemem.OBJ+
UWD\fileshrd.OBJ+
UWD\filex.OBJ+
UWD\filest.OBJ+
UWD\array_b.OBJ+
UWD\array_d.OBJ+
UWD\array_p.OBJ+
UWD\array_o.OBJ+
UWD\array_s.OBJ+
UWD\array_u.OBJ+
UWD\array_w.OBJ+
UWD\list_o.OBJ+
UWD\list_p.OBJ+
UWD\list_s.OBJ+
UWD\map_pp.OBJ+
UWD\map_pw.OBJ+
UWD\map_so.OBJ+
UWD\map_sp.OBJ+
UWD\map_ss.OBJ+
UWD\map_wo.OBJ+
UWD\map_wp.OBJ+
UWD\plex.OBJ+
UWD\strcore.OBJ+
UWD\strex.OBJ+
UWD\timecore.OBJ+
UWD\afxdbcs.OBJ+
UWD\afxstate.OBJ+
UWD\afxtls.OBJ+
UWD\mtcore.OBJ+
UWD\mtex.OBJ+
UWD\wincore.OBJ+
UWD\winfrm.OBJ+
UWD\winfrm2.OBJ+
UWD\winfrmx.OBJ+
UWD\winmdi.OBJ+
UWD\tooltip.OBJ+
UWD\winmini.OBJ+
UWD\winhand.OBJ+
UWD\winmain.OBJ+
UWD\barcore.OBJ+
UWD\bartool.OBJ+
UWD\bardlg.OBJ+
UWD\barstat.OBJ+
UWD\bardock.OBJ+
UWD\dockcont.OBJ+
UWD\dockstat.OBJ+
UWD\dcprev.OBJ+
UWD\dcmeta.OBJ+
UWD\trckrect.OBJ+
UWD\winctrl1.OBJ+
UWD\winctrl2.OBJ+
UWD\winctrl3.OBJ+
UWD\winctrl4.OBJ+
UWD\winbtn.OBJ+
UWD\dlgcore.OBJ+
UWD\dlgdata.OBJ+
UWD\dlgfloat.OBJ+
UWD\dlgprop.OBJ+
UWD\dlgcomm.OBJ+
UWD\dlgfile.OBJ+
UWD\dlgprnt.OBJ+
UWD\dlgclr.OBJ+
UWD\dlgfr.OBJ+
UWD\ccdata.OBJ+
UWD\dlgtempl.OBJ+
UWD\wingdi.OBJ+
UWD\wingdix.OBJ+
UWD\winstr.OBJ+
UWD\winmenu.OBJ+
UWD\auxdata.OBJ+
UWD\afxcrit.OBJ+
UWD\afxtrace.OBJ+
UWD\winutil.OBJ+
UWD\winocc.OBJ+
UWD\cmdtarg.OBJ+
UWD\doccore.OBJ+
UWD\doctempl.OBJ+
UWD\docsingl.OBJ+
UWD\docmulti.OBJ+
UWD\docmgr.OBJ+
UWD\viewcore.OBJ+
UWD\viewprnt.OBJ+
UWD\winsplit.OBJ+
UWD\viewscrl.OBJ+
UWD\viewform.OBJ+
UWD\viewedit.OBJ+
UWD\viewprev.OBJ+
UWD\viewcmn.OBJ+
UWD\docmapi.OBJ+
UWD\thrdcore.OBJ+
UWD\appcore.OBJ+
UWD\appinit.OBJ+
UWD\appterm.OBJ+
UWD\appui.OBJ+
UWD\appui2.OBJ+
UWD\appui3.OBJ+
UWD\appgray.OBJ+
UWD\appdlg.OBJ+
UWD\app3d.OBJ+
UWD\appprnt.OBJ+
UWD\apphelp.OBJ+
UWD\apphelpx.OBJ+
UWD\filelist.OBJ+
UWD\app3ds.OBJ+
UWD\nolib.OBJ+
UWD\appmodul.OBJ+
UWD\dllmodul.OBJ+
UWD\oleexp.OBJ+
UWD\sockcore.OBJ+
UWD\olelock.OBJ+
UWD\oleinit.OBJ+
UWD\olebar.OBJ+
UWD\olecnvrt.OBJ+
UWD\olemon.OBJ+
UWD\oledocvw.OBJ+
UWD\oledocob.OBJ+
UWD\oledoctg.OBJ+
UWD\oledocip.OBJ+
UWD\oleasmon.OBJ+
UWD\ctlquick.OBJ+
UWD\ctlnownd.OBJ+
UWD\olecli1.OBJ+
UWD\olecli2.OBJ+
UWD\olecli3.OBJ+
UWD\oledobj1.OBJ+
UWD\oledobj2.OBJ+
UWD\oledisp1.OBJ+
UWD\oledisp2.OBJ+
UWD\oledlgs1.OBJ+
UWD\oledlgs2.OBJ+
UWD\oledlgs3.OBJ+
UWD\oledata.OBJ+
UWD\oledoc1.OBJ+
UWD\oledoc2.OBJ+
UWD\oledrop1.OBJ+
UWD\oledrop2.OBJ+
UWD\olemsgf.OBJ+
UWD\oleenum.OBJ+
UWD\olefact.OBJ+
UWD\oleipfrm.OBJ+
UWD\olelink.OBJ+
UWD\olemisc.OBJ+
UWD\olestrm.OBJ+
UWD\olesvr1.OBJ+
UWD\olesvr2.OBJ+
UWD\olereg.OBJ+
UWD\oletsvr.OBJ+
UWD\oleui1.OBJ+
UWD\oleui2.OBJ+
UWD\oleunk.OBJ+
UWD\oleverb.OBJ+
UWD\olecall.OBJ+
UWD\olevar.OBJ+
UWD\viewrich.OBJ+
UWD\oledll.OBJ+
UWD\oletyplb.OBJ+
UWD\occmgr.OBJ+
UWD\occevent.OBJ+
UWD\occcont.OBJ+
UWD\occsite.OBJ+
UWD\occlock.OBJ+
UWD\occddx.OBJ+
UWD\occddxf.OBJ+
UWD\occdlg.OBJ+
UWD\dbcore.OBJ+
UWD\dbrfx.OBJ+
UWD\dbview.OBJ+
UWD\dbflt.OBJ+
UWD\dblong.OBJ+
UWD\dbvar.OBJ+
UWD\dlgfnt.OBJ+
UWD\daocore.OBJ+
UWD\daodfx.OBJ+
UWD\daoview.OBJ+
UWD\inet.OBJ+
UWD\inetimpl.OBJ+
UWD\filefind.OBJ
..\..\..\LIB\$$SCW$$.LIB
NUL

uafxcwd.DEF;
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
..\app3ds.OBJ+
..\nolib.OBJ+
..\appmodul.OBJ+
..\dllmodul.OBJ+
..\oleexp.OBJ+
..\sockcore.OBJ+
..\olelock.OBJ+
..\oleinit.OBJ+
..\olebar.OBJ+
..\olecnvrt.OBJ+
..\olemon.OBJ+
..\oledocvw.OBJ+
..\oledocob.OBJ+
..\oledoctg.OBJ+
..\oledocip.OBJ+
..\oleasmon.OBJ+
..\ctlquick.OBJ+
..\ctlnownd.OBJ+
..\olecli1.OBJ+
..\olecli2.OBJ+
..\olecli3.OBJ+
..\oledobj1.OBJ+
..\oledobj2.OBJ+
..\oledisp1.OBJ+
..\oledisp2.OBJ+
..\oledlgs1.OBJ+
..\oledlgs2.OBJ+
..\oledlgs3.OBJ+
..\oledata.OBJ+
..\oledoc1.OBJ+
..\oledoc2.OBJ+
..\oledrop1.OBJ+
..\oledrop2.OBJ+
..\olemsgf.OBJ+
..\oleenum.OBJ+
..\olefact.OBJ+
..\oleipfrm.OBJ+
..\olelink.OBJ+
..\olemisc.OBJ+
..\olestrm.OBJ+
..\olesvr1.OBJ+
..\olesvr2.OBJ+
..\olereg.OBJ+
..\oletsvr.OBJ+
..\oleui1.OBJ+
..\oleui2.OBJ+
..\oleunk.OBJ+
..\oleverb.OBJ+
..\olecall.OBJ+
..\olevar.OBJ+
..\viewrich.OBJ+
..\oledll.OBJ+
..\oletyplb.OBJ+
..\occmgr.OBJ+
..\occevent.OBJ+
..\occcont.OBJ+
..\occsite.OBJ+
..\occlock.OBJ+
..\occddx.OBJ+
..\occddxf.OBJ+
..\occdlg.OBJ+
..\dbcore.OBJ+
..\dbrfx.OBJ+
..\dbview.OBJ+
..\dbflt.OBJ+
..\dblong.OBJ+
..\dbvar.OBJ+
..\dlgfnt.OBJ+
..\daocore.OBJ+
..\daodfx.OBJ+
..\daoview.OBJ+
..\inet.OBJ+
..\inetimpl.OBJ+
..\filefind.OBJ
$$SCW$$.LIB
NUL

uafxcwd.DEF;
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
	
$(TARGETDIR)\$(PROJ).$(PROJTYPE): $(SYMS) $(OBJS) $(INCLUDEDOBJS) $(RCFILES) $(RESFILES) $(HELPFILES) 
			-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		LIB $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(LIBFLAGS)\
				@<<
 + $(OUTPUTDIR)\objcore.OBJ + $(OUTPUTDIR)\except.OBJ + $(OUTPUTDIR)\validadd.OBJ &
		 + $(OUTPUTDIR)\dumpcont.OBJ + $(OUTPUTDIR)\dumpflt.OBJ + $(OUTPUTDIR)\arccore.OBJ + $(OUTPUTDIR)\arcobj.OBJ &
		 + $(OUTPUTDIR)\arcex.OBJ + $(OUTPUTDIR)\arcstrm.OBJ + $(OUTPUTDIR)\dumpinit.OBJ + $(OUTPUTDIR)\dumpout.OBJ &
		 + $(OUTPUTDIR)\afxasert.OBJ + $(OUTPUTDIR)\afxmem.OBJ + $(OUTPUTDIR)\afxabort.OBJ + $(OUTPUTDIR)\afxinl1.OBJ &
		 + $(OUTPUTDIR)\afxinl2.OBJ + $(OUTPUTDIR)\afxinl3.OBJ + $(OUTPUTDIR)\filecore.OBJ + $(OUTPUTDIR)\filetxt.OBJ &
		 + $(OUTPUTDIR)\filemem.OBJ + $(OUTPUTDIR)\fileshrd.OBJ + $(OUTPUTDIR)\filex.OBJ + $(OUTPUTDIR)\filest.OBJ &
		 + $(OUTPUTDIR)\array_b.OBJ + $(OUTPUTDIR)\array_d.OBJ + $(OUTPUTDIR)\array_p.OBJ + $(OUTPUTDIR)\array_o.OBJ &
		 + $(OUTPUTDIR)\array_s.OBJ + $(OUTPUTDIR)\array_u.OBJ + $(OUTPUTDIR)\array_w.OBJ + $(OUTPUTDIR)\list_o.OBJ &
		 + $(OUTPUTDIR)\list_p.OBJ + $(OUTPUTDIR)\list_s.OBJ + $(OUTPUTDIR)\map_pp.OBJ + $(OUTPUTDIR)\map_pw.OBJ &
		 + $(OUTPUTDIR)\map_so.OBJ + $(OUTPUTDIR)\map_sp.OBJ + $(OUTPUTDIR)\map_ss.OBJ + $(OUTPUTDIR)\map_wo.OBJ &
		 + $(OUTPUTDIR)\map_wp.OBJ + $(OUTPUTDIR)\plex.OBJ + $(OUTPUTDIR)\strcore.OBJ + $(OUTPUTDIR)\strex.OBJ &
		 + $(OUTPUTDIR)\timecore.OBJ + $(OUTPUTDIR)\afxdbcs.OBJ + $(OUTPUTDIR)\afxstate.OBJ + $(OUTPUTDIR)\afxtls.OBJ &
		 + $(OUTPUTDIR)\mtcore.OBJ + $(OUTPUTDIR)\mtex.OBJ + $(OUTPUTDIR)\wincore.OBJ + $(OUTPUTDIR)\winfrm.OBJ &
		 + $(OUTPUTDIR)\winfrm2.OBJ + $(OUTPUTDIR)\winfrmx.OBJ + $(OUTPUTDIR)\winmdi.OBJ + $(OUTPUTDIR)\tooltip.OBJ &
		 + $(OUTPUTDIR)\winmini.OBJ + $(OUTPUTDIR)\winhand.OBJ + $(OUTPUTDIR)\winmain.OBJ + $(OUTPUTDIR)\barcore.OBJ &
		 + $(OUTPUTDIR)\bartool.OBJ + $(OUTPUTDIR)\bardlg.OBJ + $(OUTPUTDIR)\barstat.OBJ + $(OUTPUTDIR)\bardock.OBJ &
		 + $(OUTPUTDIR)\dockcont.OBJ + $(OUTPUTDIR)\dockstat.OBJ + $(OUTPUTDIR)\dcprev.OBJ + $(OUTPUTDIR)\dcmeta.OBJ &
		 + $(OUTPUTDIR)\trckrect.OBJ + $(OUTPUTDIR)\winctrl1.OBJ + $(OUTPUTDIR)\winctrl2.OBJ + $(OUTPUTDIR)\winctrl3.OBJ &
		 + $(OUTPUTDIR)\winctrl4.OBJ + $(OUTPUTDIR)\winbtn.OBJ + $(OUTPUTDIR)\dlgcore.OBJ + $(OUTPUTDIR)\dlgdata.OBJ &
		 + $(OUTPUTDIR)\dlgfloat.OBJ + $(OUTPUTDIR)\dlgprop.OBJ + $(OUTPUTDIR)\dlgcomm.OBJ + $(OUTPUTDIR)\dlgfile.OBJ &
		 + $(OUTPUTDIR)\dlgprnt.OBJ + $(OUTPUTDIR)\dlgclr.OBJ + $(OUTPUTDIR)\dlgfr.OBJ + $(OUTPUTDIR)\ccdata.OBJ &
		 + $(OUTPUTDIR)\dlgtempl.OBJ + $(OUTPUTDIR)\wingdi.OBJ + $(OUTPUTDIR)\wingdix.OBJ + $(OUTPUTDIR)\winstr.OBJ &
		 + $(OUTPUTDIR)\winmenu.OBJ + $(OUTPUTDIR)\auxdata.OBJ + $(OUTPUTDIR)\afxcrit.OBJ + $(OUTPUTDIR)\afxtrace.OBJ &
		 + $(OUTPUTDIR)\winutil.OBJ + $(OUTPUTDIR)\winocc.OBJ + $(OUTPUTDIR)\cmdtarg.OBJ + $(OUTPUTDIR)\doccore.OBJ &
		 + $(OUTPUTDIR)\doctempl.OBJ + $(OUTPUTDIR)\docsingl.OBJ + $(OUTPUTDIR)\docmulti.OBJ + $(OUTPUTDIR)\docmgr.OBJ &
		 + $(OUTPUTDIR)\viewcore.OBJ + $(OUTPUTDIR)\viewprnt.OBJ + $(OUTPUTDIR)\winsplit.OBJ + $(OUTPUTDIR)\viewscrl.OBJ &
		 + $(OUTPUTDIR)\viewform.OBJ + $(OUTPUTDIR)\viewedit.OBJ + $(OUTPUTDIR)\viewprev.OBJ + $(OUTPUTDIR)\viewcmn.OBJ &
		 + $(OUTPUTDIR)\docmapi.OBJ + $(OUTPUTDIR)\thrdcore.OBJ + $(OUTPUTDIR)\appcore.OBJ + $(OUTPUTDIR)\appinit.OBJ &
		 + $(OUTPUTDIR)\appterm.OBJ + $(OUTPUTDIR)\appui.OBJ + $(OUTPUTDIR)\appui2.OBJ + $(OUTPUTDIR)\appui3.OBJ &
		 + $(OUTPUTDIR)\appgray.OBJ + $(OUTPUTDIR)\appdlg.OBJ + $(OUTPUTDIR)\app3d.OBJ + $(OUTPUTDIR)\appprnt.OBJ &
		 + $(OUTPUTDIR)\apphelp.OBJ + $(OUTPUTDIR)\apphelpx.OBJ + $(OUTPUTDIR)\filelist.OBJ + $(OUTPUTDIR)\app3ds.OBJ &
		 + $(OUTPUTDIR)\nolib.OBJ + $(OUTPUTDIR)\appmodul.OBJ + $(OUTPUTDIR)\dllmodul.OBJ + $(OUTPUTDIR)\oleexp.OBJ &
		 + $(OUTPUTDIR)\sockcore.OBJ + $(OUTPUTDIR)\olelock.OBJ + $(OUTPUTDIR)\oleinit.OBJ + $(OUTPUTDIR)\olebar.OBJ &
		 + $(OUTPUTDIR)\olecnvrt.OBJ + $(OUTPUTDIR)\olemon.OBJ + $(OUTPUTDIR)\oledocvw.OBJ + $(OUTPUTDIR)\oledocob.OBJ &
		 + $(OUTPUTDIR)\oledoctg.OBJ + $(OUTPUTDIR)\oledocip.OBJ + $(OUTPUTDIR)\oleasmon.OBJ + $(OUTPUTDIR)\ctlquick.OBJ &
		 + $(OUTPUTDIR)\ctlnownd.OBJ + $(OUTPUTDIR)\olecli1.OBJ + $(OUTPUTDIR)\olecli2.OBJ + $(OUTPUTDIR)\olecli3.OBJ &
		 + $(OUTPUTDIR)\oledobj1.OBJ + $(OUTPUTDIR)\oledobj2.OBJ + $(OUTPUTDIR)\oledisp1.OBJ + $(OUTPUTDIR)\oledisp2.OBJ &
		 + $(OUTPUTDIR)\oledlgs1.OBJ + $(OUTPUTDIR)\oledlgs2.OBJ + $(OUTPUTDIR)\oledlgs3.OBJ + $(OUTPUTDIR)\oledata.OBJ &
		 + $(OUTPUTDIR)\oledoc1.OBJ + $(OUTPUTDIR)\oledoc2.OBJ + $(OUTPUTDIR)\oledrop1.OBJ + $(OUTPUTDIR)\oledrop2.OBJ &
		 + $(OUTPUTDIR)\olemsgf.OBJ + $(OUTPUTDIR)\oleenum.OBJ + $(OUTPUTDIR)\olefact.OBJ + $(OUTPUTDIR)\oleipfrm.OBJ &
		 + $(OUTPUTDIR)\olelink.OBJ + $(OUTPUTDIR)\olemisc.OBJ + $(OUTPUTDIR)\olestrm.OBJ + $(OUTPUTDIR)\olesvr1.OBJ &
		 + $(OUTPUTDIR)\olesvr2.OBJ + $(OUTPUTDIR)\olereg.OBJ + $(OUTPUTDIR)\oletsvr.OBJ + $(OUTPUTDIR)\oleui1.OBJ &
		 + $(OUTPUTDIR)\oleui2.OBJ + $(OUTPUTDIR)\oleunk.OBJ + $(OUTPUTDIR)\oleverb.OBJ + $(OUTPUTDIR)\olecall.OBJ &
		 + $(OUTPUTDIR)\olevar.OBJ + $(OUTPUTDIR)\viewrich.OBJ + $(OUTPUTDIR)\oledll.OBJ + $(OUTPUTDIR)\oletyplb.OBJ &
		 + $(OUTPUTDIR)\occmgr.OBJ + $(OUTPUTDIR)\occevent.OBJ + $(OUTPUTDIR)\occcont.OBJ + $(OUTPUTDIR)\occsite.OBJ &
		 + $(OUTPUTDIR)\occlock.OBJ + $(OUTPUTDIR)\occddx.OBJ + $(OUTPUTDIR)\occddxf.OBJ + $(OUTPUTDIR)\occdlg.OBJ &
		 + $(OUTPUTDIR)\dbcore.OBJ + $(OUTPUTDIR)\dbrfx.OBJ + $(OUTPUTDIR)\dbview.OBJ + $(OUTPUTDIR)\dbflt.OBJ &
		 + $(OUTPUTDIR)\dblong.OBJ + $(OUTPUTDIR)\dbvar.OBJ + $(OUTPUTDIR)\dlgfnt.OBJ + $(OUTPUTDIR)\daocore.OBJ &
		 + $(OUTPUTDIR)\daodfx.OBJ + $(OUTPUTDIR)\daoview.OBJ + $(OUTPUTDIR)\inet.OBJ + $(OUTPUTDIR)\inetimpl.OBJ &
		 + $(OUTPUTDIR)\filefind.OBJ
<<
			-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)
			-echo $(TARGETDIR)\$(PROJ).$(PROJTYPE) built

_done:
		REM  Project is up to date

buildall:	clean	all


clean:
		-del $(TARGETDIR)\$$SCW$$.$(PROJTYPE)
		-del $(TARGETDIR)\$(PROJ).CLE
		-del $(OUTPUTDIR)\SCPH.SYM
		-del UAFXCWD.dpd
		-del $(OBJS)
		-del $(SYMROOT).SYM
		-del $(SYMROOT).PCO

cleanres:

res:		cleanres $(RCFILES) all


link:
		LIB $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(LIBFLAGS)\
				@<<
 + $(OUTPUTDIR)\objcore.OBJ + $(OUTPUTDIR)\except.OBJ + $(OUTPUTDIR)\validadd.OBJ &
		 + $(OUTPUTDIR)\dumpcont.OBJ + $(OUTPUTDIR)\dumpflt.OBJ + $(OUTPUTDIR)\arccore.OBJ + $(OUTPUTDIR)\arcobj.OBJ &
		 + $(OUTPUTDIR)\arcex.OBJ + $(OUTPUTDIR)\arcstrm.OBJ + $(OUTPUTDIR)\dumpinit.OBJ + $(OUTPUTDIR)\dumpout.OBJ &
		 + $(OUTPUTDIR)\afxasert.OBJ + $(OUTPUTDIR)\afxmem.OBJ + $(OUTPUTDIR)\afxabort.OBJ + $(OUTPUTDIR)\afxinl1.OBJ &
		 + $(OUTPUTDIR)\afxinl2.OBJ + $(OUTPUTDIR)\afxinl3.OBJ + $(OUTPUTDIR)\filecore.OBJ + $(OUTPUTDIR)\filetxt.OBJ &
		 + $(OUTPUTDIR)\filemem.OBJ + $(OUTPUTDIR)\fileshrd.OBJ + $(OUTPUTDIR)\filex.OBJ + $(OUTPUTDIR)\filest.OBJ &
		 + $(OUTPUTDIR)\array_b.OBJ + $(OUTPUTDIR)\array_d.OBJ + $(OUTPUTDIR)\array_p.OBJ + $(OUTPUTDIR)\array_o.OBJ &
		 + $(OUTPUTDIR)\array_s.OBJ + $(OUTPUTDIR)\array_u.OBJ + $(OUTPUTDIR)\array_w.OBJ + $(OUTPUTDIR)\list_o.OBJ &
		 + $(OUTPUTDIR)\list_p.OBJ + $(OUTPUTDIR)\list_s.OBJ + $(OUTPUTDIR)\map_pp.OBJ + $(OUTPUTDIR)\map_pw.OBJ &
		 + $(OUTPUTDIR)\map_so.OBJ + $(OUTPUTDIR)\map_sp.OBJ + $(OUTPUTDIR)\map_ss.OBJ + $(OUTPUTDIR)\map_wo.OBJ &
		 + $(OUTPUTDIR)\map_wp.OBJ + $(OUTPUTDIR)\plex.OBJ + $(OUTPUTDIR)\strcore.OBJ + $(OUTPUTDIR)\strex.OBJ &
		 + $(OUTPUTDIR)\timecore.OBJ + $(OUTPUTDIR)\afxdbcs.OBJ + $(OUTPUTDIR)\afxstate.OBJ + $(OUTPUTDIR)\afxtls.OBJ &
		 + $(OUTPUTDIR)\mtcore.OBJ + $(OUTPUTDIR)\mtex.OBJ + $(OUTPUTDIR)\wincore.OBJ + $(OUTPUTDIR)\winfrm.OBJ &
		 + $(OUTPUTDIR)\winfrm2.OBJ + $(OUTPUTDIR)\winfrmx.OBJ + $(OUTPUTDIR)\winmdi.OBJ + $(OUTPUTDIR)\tooltip.OBJ &
		 + $(OUTPUTDIR)\winmini.OBJ + $(OUTPUTDIR)\winhand.OBJ + $(OUTPUTDIR)\winmain.OBJ + $(OUTPUTDIR)\barcore.OBJ &
		 + $(OUTPUTDIR)\bartool.OBJ + $(OUTPUTDIR)\bardlg.OBJ + $(OUTPUTDIR)\barstat.OBJ + $(OUTPUTDIR)\bardock.OBJ &
		 + $(OUTPUTDIR)\dockcont.OBJ + $(OUTPUTDIR)\dockstat.OBJ + $(OUTPUTDIR)\dcprev.OBJ + $(OUTPUTDIR)\dcmeta.OBJ &
		 + $(OUTPUTDIR)\trckrect.OBJ + $(OUTPUTDIR)\winctrl1.OBJ + $(OUTPUTDIR)\winctrl2.OBJ + $(OUTPUTDIR)\winctrl3.OBJ &
		 + $(OUTPUTDIR)\winctrl4.OBJ + $(OUTPUTDIR)\winbtn.OBJ + $(OUTPUTDIR)\dlgcore.OBJ + $(OUTPUTDIR)\dlgdata.OBJ &
		 + $(OUTPUTDIR)\dlgfloat.OBJ + $(OUTPUTDIR)\dlgprop.OBJ + $(OUTPUTDIR)\dlgcomm.OBJ + $(OUTPUTDIR)\dlgfile.OBJ &
		 + $(OUTPUTDIR)\dlgprnt.OBJ + $(OUTPUTDIR)\dlgclr.OBJ + $(OUTPUTDIR)\dlgfr.OBJ + $(OUTPUTDIR)\ccdata.OBJ &
		 + $(OUTPUTDIR)\dlgtempl.OBJ + $(OUTPUTDIR)\wingdi.OBJ + $(OUTPUTDIR)\wingdix.OBJ + $(OUTPUTDIR)\winstr.OBJ &
		 + $(OUTPUTDIR)\winmenu.OBJ + $(OUTPUTDIR)\auxdata.OBJ + $(OUTPUTDIR)\afxcrit.OBJ + $(OUTPUTDIR)\afxtrace.OBJ &
		 + $(OUTPUTDIR)\winutil.OBJ + $(OUTPUTDIR)\winocc.OBJ + $(OUTPUTDIR)\cmdtarg.OBJ + $(OUTPUTDIR)\doccore.OBJ &
		 + $(OUTPUTDIR)\doctempl.OBJ + $(OUTPUTDIR)\docsingl.OBJ + $(OUTPUTDIR)\docmulti.OBJ + $(OUTPUTDIR)\docmgr.OBJ &
		 + $(OUTPUTDIR)\viewcore.OBJ + $(OUTPUTDIR)\viewprnt.OBJ + $(OUTPUTDIR)\winsplit.OBJ + $(OUTPUTDIR)\viewscrl.OBJ &
		 + $(OUTPUTDIR)\viewform.OBJ + $(OUTPUTDIR)\viewedit.OBJ + $(OUTPUTDIR)\viewprev.OBJ + $(OUTPUTDIR)\viewcmn.OBJ &
		 + $(OUTPUTDIR)\docmapi.OBJ + $(OUTPUTDIR)\thrdcore.OBJ + $(OUTPUTDIR)\appcore.OBJ + $(OUTPUTDIR)\appinit.OBJ &
		 + $(OUTPUTDIR)\appterm.OBJ + $(OUTPUTDIR)\appui.OBJ + $(OUTPUTDIR)\appui2.OBJ + $(OUTPUTDIR)\appui3.OBJ &
		 + $(OUTPUTDIR)\appgray.OBJ + $(OUTPUTDIR)\appdlg.OBJ + $(OUTPUTDIR)\app3d.OBJ + $(OUTPUTDIR)\appprnt.OBJ &
		 + $(OUTPUTDIR)\apphelp.OBJ + $(OUTPUTDIR)\apphelpx.OBJ + $(OUTPUTDIR)\filelist.OBJ + $(OUTPUTDIR)\app3ds.OBJ &
		 + $(OUTPUTDIR)\nolib.OBJ + $(OUTPUTDIR)\appmodul.OBJ + $(OUTPUTDIR)\dllmodul.OBJ + $(OUTPUTDIR)\oleexp.OBJ &
		 + $(OUTPUTDIR)\sockcore.OBJ + $(OUTPUTDIR)\olelock.OBJ + $(OUTPUTDIR)\oleinit.OBJ + $(OUTPUTDIR)\olebar.OBJ &
		 + $(OUTPUTDIR)\olecnvrt.OBJ + $(OUTPUTDIR)\olemon.OBJ + $(OUTPUTDIR)\oledocvw.OBJ + $(OUTPUTDIR)\oledocob.OBJ &
		 + $(OUTPUTDIR)\oledoctg.OBJ + $(OUTPUTDIR)\oledocip.OBJ + $(OUTPUTDIR)\oleasmon.OBJ + $(OUTPUTDIR)\ctlquick.OBJ &
		 + $(OUTPUTDIR)\ctlnownd.OBJ + $(OUTPUTDIR)\olecli1.OBJ + $(OUTPUTDIR)\olecli2.OBJ + $(OUTPUTDIR)\olecli3.OBJ &
		 + $(OUTPUTDIR)\oledobj1.OBJ + $(OUTPUTDIR)\oledobj2.OBJ + $(OUTPUTDIR)\oledisp1.OBJ + $(OUTPUTDIR)\oledisp2.OBJ &
		 + $(OUTPUTDIR)\oledlgs1.OBJ + $(OUTPUTDIR)\oledlgs2.OBJ + $(OUTPUTDIR)\oledlgs3.OBJ + $(OUTPUTDIR)\oledata.OBJ &
		 + $(OUTPUTDIR)\oledoc1.OBJ + $(OUTPUTDIR)\oledoc2.OBJ + $(OUTPUTDIR)\oledrop1.OBJ + $(OUTPUTDIR)\oledrop2.OBJ &
		 + $(OUTPUTDIR)\olemsgf.OBJ + $(OUTPUTDIR)\oleenum.OBJ + $(OUTPUTDIR)\olefact.OBJ + $(OUTPUTDIR)\oleipfrm.OBJ &
		 + $(OUTPUTDIR)\olelink.OBJ + $(OUTPUTDIR)\olemisc.OBJ + $(OUTPUTDIR)\olestrm.OBJ + $(OUTPUTDIR)\olesvr1.OBJ &
		 + $(OUTPUTDIR)\olesvr2.OBJ + $(OUTPUTDIR)\olereg.OBJ + $(OUTPUTDIR)\oletsvr.OBJ + $(OUTPUTDIR)\oleui1.OBJ &
		 + $(OUTPUTDIR)\oleui2.OBJ + $(OUTPUTDIR)\oleunk.OBJ + $(OUTPUTDIR)\oleverb.OBJ + $(OUTPUTDIR)\olecall.OBJ &
		 + $(OUTPUTDIR)\olevar.OBJ + $(OUTPUTDIR)\viewrich.OBJ + $(OUTPUTDIR)\oledll.OBJ + $(OUTPUTDIR)\oletyplb.OBJ &
		 + $(OUTPUTDIR)\occmgr.OBJ + $(OUTPUTDIR)\occevent.OBJ + $(OUTPUTDIR)\occcont.OBJ + $(OUTPUTDIR)\occsite.OBJ &
		 + $(OUTPUTDIR)\occlock.OBJ + $(OUTPUTDIR)\occddx.OBJ + $(OUTPUTDIR)\occddxf.OBJ + $(OUTPUTDIR)\occdlg.OBJ &
		 + $(OUTPUTDIR)\dbcore.OBJ + $(OUTPUTDIR)\dbrfx.OBJ + $(OUTPUTDIR)\dbview.OBJ + $(OUTPUTDIR)\dbflt.OBJ &
		 + $(OUTPUTDIR)\dblong.OBJ + $(OUTPUTDIR)\dbvar.OBJ + $(OUTPUTDIR)\dlgfnt.OBJ + $(OUTPUTDIR)\daocore.OBJ &
		 + $(OUTPUTDIR)\daodfx.OBJ + $(OUTPUTDIR)\daoview.OBJ + $(OUTPUTDIR)\inet.OBJ + $(OUTPUTDIR)\inetimpl.OBJ &
		 + $(OUTPUTDIR)\filefind.OBJ
<<
		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)




!IF EXIST (UAFXCWD.dpd)
!INCLUDE UAFXCWD.dpd
!ENDIF



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



$(OUTPUTDIR)\app3ds.OBJ:	..\app3ds.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\app3ds.obj ..\app3ds.cpp



$(OUTPUTDIR)\nolib.OBJ:	..\nolib.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\nolib.obj ..\nolib.cpp



$(OUTPUTDIR)\appmodul.OBJ:	..\appmodul.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\appmodul.obj ..\appmodul.cpp



$(OUTPUTDIR)\dllmodul.OBJ:	..\dllmodul.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dllmodul.obj ..\dllmodul.cpp



$(OUTPUTDIR)\oleexp.OBJ:	..\oleexp.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oleexp.obj ..\oleexp.cpp



$(OUTPUTDIR)\sockcore.OBJ:	..\sockcore.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\sockcore.obj ..\sockcore.cpp



$(OUTPUTDIR)\olelock.OBJ:	..\olelock.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olelock.obj ..\olelock.cpp



$(OUTPUTDIR)\oleinit.OBJ:	..\oleinit.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oleinit.obj ..\oleinit.cpp



$(OUTPUTDIR)\olebar.OBJ:	..\olebar.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olebar.obj ..\olebar.cpp



$(OUTPUTDIR)\olecnvrt.OBJ:	..\olecnvrt.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olecnvrt.obj ..\olecnvrt.cpp



$(OUTPUTDIR)\olemon.OBJ:	..\olemon.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olemon.obj ..\olemon.cpp



$(OUTPUTDIR)\oledocvw.OBJ:	..\oledocvw.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledocvw.obj ..\oledocvw.cpp



$(OUTPUTDIR)\oledocob.OBJ:	..\oledocob.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledocob.obj ..\oledocob.cpp



$(OUTPUTDIR)\oledoctg.OBJ:	..\oledoctg.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledoctg.obj ..\oledoctg.cpp



$(OUTPUTDIR)\oledocip.OBJ:	..\oledocip.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledocip.obj ..\oledocip.cpp



$(OUTPUTDIR)\oleasmon.OBJ:	..\oleasmon.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oleasmon.obj ..\oleasmon.cpp



$(OUTPUTDIR)\ctlquick.OBJ:	..\ctlquick.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctlquick.obj ..\ctlquick.cpp



$(OUTPUTDIR)\ctlnownd.OBJ:	..\ctlnownd.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctlnownd.obj ..\ctlnownd.cpp



$(OUTPUTDIR)\olecli1.OBJ:	..\olecli1.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olecli1.obj ..\olecli1.cpp



$(OUTPUTDIR)\olecli2.OBJ:	..\olecli2.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olecli2.obj ..\olecli2.cpp



$(OUTPUTDIR)\olecli3.OBJ:	..\olecli3.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olecli3.obj ..\olecli3.cpp



$(OUTPUTDIR)\oledobj1.OBJ:	..\oledobj1.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledobj1.obj ..\oledobj1.cpp



$(OUTPUTDIR)\oledobj2.OBJ:	..\oledobj2.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledobj2.obj ..\oledobj2.cpp



$(OUTPUTDIR)\oledisp1.OBJ:	..\oledisp1.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledisp1.obj ..\oledisp1.cpp



$(OUTPUTDIR)\oledisp2.OBJ:	..\oledisp2.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledisp2.obj ..\oledisp2.cpp



$(OUTPUTDIR)\oledlgs1.OBJ:	..\oledlgs1.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledlgs1.obj ..\oledlgs1.cpp



$(OUTPUTDIR)\oledlgs2.OBJ:	..\oledlgs2.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledlgs2.obj ..\oledlgs2.cpp



$(OUTPUTDIR)\oledlgs3.OBJ:	..\oledlgs3.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledlgs3.obj ..\oledlgs3.cpp



$(OUTPUTDIR)\oledata.OBJ:	..\oledata.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledata.obj ..\oledata.cpp



$(OUTPUTDIR)\oledoc1.OBJ:	..\oledoc1.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledoc1.obj ..\oledoc1.cpp



$(OUTPUTDIR)\oledoc2.OBJ:	..\oledoc2.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledoc2.obj ..\oledoc2.cpp



$(OUTPUTDIR)\oledrop1.OBJ:	..\oledrop1.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledrop1.obj ..\oledrop1.cpp



$(OUTPUTDIR)\oledrop2.OBJ:	..\oledrop2.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledrop2.obj ..\oledrop2.cpp



$(OUTPUTDIR)\olemsgf.OBJ:	..\olemsgf.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olemsgf.obj ..\olemsgf.cpp



$(OUTPUTDIR)\oleenum.OBJ:	..\oleenum.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oleenum.obj ..\oleenum.cpp



$(OUTPUTDIR)\olefact.OBJ:	..\olefact.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olefact.obj ..\olefact.cpp



$(OUTPUTDIR)\oleipfrm.OBJ:	..\oleipfrm.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oleipfrm.obj ..\oleipfrm.cpp



$(OUTPUTDIR)\olelink.OBJ:	..\olelink.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olelink.obj ..\olelink.cpp



$(OUTPUTDIR)\olemisc.OBJ:	..\olemisc.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olemisc.obj ..\olemisc.cpp



$(OUTPUTDIR)\olestrm.OBJ:	..\olestrm.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olestrm.obj ..\olestrm.cpp



$(OUTPUTDIR)\olesvr1.OBJ:	..\olesvr1.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olesvr1.obj ..\olesvr1.cpp



$(OUTPUTDIR)\olesvr2.OBJ:	..\olesvr2.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olesvr2.obj ..\olesvr2.cpp



$(OUTPUTDIR)\olereg.OBJ:	..\olereg.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olereg.obj ..\olereg.cpp



$(OUTPUTDIR)\oletsvr.OBJ:	..\oletsvr.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oletsvr.obj ..\oletsvr.cpp



$(OUTPUTDIR)\oleui1.OBJ:	..\oleui1.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oleui1.obj ..\oleui1.cpp



$(OUTPUTDIR)\oleui2.OBJ:	..\oleui2.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oleui2.obj ..\oleui2.cpp



$(OUTPUTDIR)\oleunk.OBJ:	..\oleunk.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oleunk.obj ..\oleunk.cpp



$(OUTPUTDIR)\oleverb.OBJ:	..\oleverb.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oleverb.obj ..\oleverb.cpp



$(OUTPUTDIR)\olecall.OBJ:	..\olecall.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olecall.obj ..\olecall.cpp



$(OUTPUTDIR)\olevar.OBJ:	..\olevar.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\olevar.obj ..\olevar.cpp



$(OUTPUTDIR)\viewrich.OBJ:	..\viewrich.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\viewrich.obj ..\viewrich.cpp



$(OUTPUTDIR)\oledll.OBJ:	..\oledll.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oledll.obj ..\oledll.cpp



$(OUTPUTDIR)\oletyplb.OBJ:	..\oletyplb.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\oletyplb.obj ..\oletyplb.cpp



$(OUTPUTDIR)\occmgr.OBJ:	..\occmgr.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\occmgr.obj ..\occmgr.cpp



$(OUTPUTDIR)\occevent.OBJ:	..\occevent.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\occevent.obj ..\occevent.cpp



$(OUTPUTDIR)\occcont.OBJ:	..\occcont.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\occcont.obj ..\occcont.cpp



$(OUTPUTDIR)\occsite.OBJ:	..\occsite.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\occsite.obj ..\occsite.cpp



$(OUTPUTDIR)\occlock.OBJ:	..\occlock.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\occlock.obj ..\occlock.cpp



$(OUTPUTDIR)\occddx.OBJ:	..\occddx.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\occddx.obj ..\occddx.cpp



$(OUTPUTDIR)\occddxf.OBJ:	..\occddxf.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\occddxf.obj ..\occddxf.cpp



$(OUTPUTDIR)\occdlg.OBJ:	..\occdlg.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\occdlg.obj ..\occdlg.cpp



$(OUTPUTDIR)\dbcore.OBJ:	..\dbcore.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dbcore.obj ..\dbcore.cpp



$(OUTPUTDIR)\dbrfx.OBJ:	..\dbrfx.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dbrfx.obj ..\dbrfx.cpp



$(OUTPUTDIR)\dbview.OBJ:	..\dbview.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dbview.obj ..\dbview.cpp



$(OUTPUTDIR)\dbflt.OBJ:	..\dbflt.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dbflt.obj ..\dbflt.cpp



$(OUTPUTDIR)\dblong.OBJ:	..\dblong.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dblong.obj ..\dblong.cpp



$(OUTPUTDIR)\dbvar.OBJ:	..\dbvar.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dbvar.obj ..\dbvar.cpp



$(OUTPUTDIR)\dlgfnt.OBJ:	..\dlgfnt.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dlgfnt.obj ..\dlgfnt.cpp



$(OUTPUTDIR)\daocore.OBJ:	..\daocore.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\daocore.obj ..\daocore.cpp



$(OUTPUTDIR)\daodfx.OBJ:	..\daodfx.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\daodfx.obj ..\daodfx.cpp



$(OUTPUTDIR)\daoview.OBJ:	..\daoview.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\daoview.obj ..\daoview.cpp



$(OUTPUTDIR)\inet.OBJ:	..\inet.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\inet.obj ..\inet.cpp



$(OUTPUTDIR)\inetimpl.OBJ:	..\inetimpl.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\inetimpl.obj ..\inetimpl.cpp



$(OUTPUTDIR)\filefind.OBJ:	..\filefind.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\filefind.obj ..\filefind.cpp


$(SYMROOT).SYM: ..\stdafx.h
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF$(SYMROOT).SYM -o$(SYMROOT).PCO ..\stdafx.h



