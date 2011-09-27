ORIGIN		= Digital Mars C++
ORIGIN_VER	= Version 7.22
VERSION		= DEBUG

!IFDEF SUB_DEBUG
DEBUG		= $(SUB_DEBUG)
NDEBUG		= !$(SUB_DEBUG)
!ELSE
DEBUG		= 1
NDEBUG		= 0
!ENDIF

PROJ		= snndasm
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

HEADERS		= 

DEFFILE		= snndasm.DEF

!IF $(DEBUG)
OUTPUTDIR	= snndasm
!IF EXIST (snndasm)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $(OUTPUTDIR)\*.* md $(OUTPUTDIR)
!ENDIF
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		=  snndasm\precomp 
SYMS		= $(SYMROOT).SYM 
LIBS		= 

CFLAGS		=  -HIprecomp.h -Ae -Ar -r -mn -S -NL -5 -a8 -c -H -HDsnndasm -HO- -wx -w- -w12 
LFLAGS		=  /CO /NOI /DE /XN /A:512
DEFINES		= -D_MT
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
DEFINES		= -D_MT
!ENDIF

HFLAGS		= $(CFLAGS) 
MFLAGS		= MASTERPROJ=$(PROJ) 
LIBFLAGS	=  /C /NOI 
RESFLAGS	=  -32 
DEBUGGERFLAGS	=  
AFLAGS		= $(CFLAGS) 
HELPFLAGS	= 

MODEL		= N

PAR		= PROJS BATS OBJS

RCDEFINES	= 

INCLUDES	= -I..\INCLUDE -I..\..\INCLUDE

INCLUDEDOBJS	= 

OBJS		=  $(OUTPUTDIR)\_8087.OBJ  $(OUTPUTDIR)\_exit.OBJ  $(OUTPUTDIR)\_exit2.OBJ  $(OUTPUTDIR)\atoi.OBJ  \
		 $(OUTPUTDIR)\atol.OBJ  $(OUTPUTDIR)\chkstk.OBJ  $(OUTPUTDIR)\cinit.OBJ  $(OUTPUTDIR)\clearbss.OBJ  \
		 $(OUTPUTDIR)\dbllng.OBJ  $(OUTPUTDIR)\double.OBJ  $(OUTPUTDIR)\excptlst.OBJ  $(OUTPUTDIR)\faterr.OBJ  \
		 $(OUTPUTDIR)\floattyp.OBJ  $(OUTPUTDIR)\flt87.OBJ  $(OUTPUTDIR)\fpatan32.OBJ  $(OUTPUTDIR)\fprem32.OBJ  \
		 $(OUTPUTDIR)\fptan32.OBJ  $(OUTPUTDIR)\fputc.OBJ  $(OUTPUTDIR)\ftol.OBJ  \
		 $(OUTPUTDIR)\itoa.OBJ  $(OUTPUTDIR)\ldexp.OBJ  $(OUTPUTDIR)\llmath.OBJ  $(OUTPUTDIR)\ltoa.OBJ  \
		 $(OUTPUTDIR)\math87.OBJ  $(OUTPUTDIR)\mathtrn.OBJ  $(OUTPUTDIR)\mathtrnf.OBJ  $(OUTPUTDIR)\memchr.OBJ  \
		 $(OUTPUTDIR)\memcpy.OBJ  $(OUTPUTDIR)\memicmp.OBJ  $(OUTPUTDIR)\memint.OBJ  $(OUTPUTDIR)\movedata.OBJ  \
		 $(OUTPUTDIR)\page.OBJ  $(OUTPUTDIR)\patch32.OBJ  $(OUTPUTDIR)\port.OBJ  $(OUTPUTDIR)\read.OBJ  \
		 $(OUTPUTDIR)\rot.OBJ  $(OUTPUTDIR)\seg.OBJ  $(OUTPUTDIR)\stpcpy.OBJ  \
		 $(OUTPUTDIR)\strcat.OBJ  $(OUTPUTDIR)\strcmpl.OBJ  $(OUTPUTDIR)\strncat.OBJ  $(OUTPUTDIR)\strncpy.OBJ  \
		 $(OUTPUTDIR)\strtoul.OBJ  $(OUTPUTDIR)\ultoa.OBJ  $(OUTPUTDIR)\write.OBJ 

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
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -osnndasm\$(*B).sym $*.h

.HPP.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -osnndasm\$(*B).sym $*.hpp

.HXX.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -osnndasm\$(*B).sym $*.hxx

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
snndasm\precomp.PCO+
snndasm\_8087.OBJ+
snndasm\_exit.OBJ+
snndasm\_exit2.OBJ+
snndasm\atoi.OBJ+
snndasm\atol.OBJ+
snndasm\chkstk.OBJ+
snndasm\cinit.OBJ+
snndasm\clearbss.OBJ+
snndasm\dbllng.OBJ+
snndasm\double.OBJ+
snndasm\excptlst.OBJ+
snndasm\faterr.OBJ+
snndasm\floattyp.OBJ+
snndasm\flt87.OBJ+
snndasm\fpatan32.OBJ+
snndasm\fprem32.OBJ+
snndasm\fptan32.OBJ+
snndasm\fputc.OBJ+
snndasm\ftol.OBJ+
snndasm\itoa.OBJ+
snndasm\ldexp.OBJ+
snndasm\llmath.OBJ+
snndasm\ltoa.OBJ+
snndasm\math87.OBJ+
snndasm\mathtrn.OBJ+
snndasm\mathtrnf.OBJ+
snndasm\memchr.OBJ+
snndasm\memcpy.OBJ+
snndasm\memicmp.OBJ+
snndasm\memint.OBJ+
snndasm\movedata.OBJ+
snndasm\page.OBJ+
snndasm\patch32.OBJ+
snndasm\port.OBJ+
snndasm\read.OBJ+
snndasm\rot.OBJ+
snndasm\seg.OBJ+
snndasm\stpcpy.OBJ+
snndasm\strcat.OBJ+
snndasm\strcmpl.OBJ+
snndasm\strncat.OBJ+
snndasm\strncpy.OBJ+
snndasm\strtoul.OBJ+
snndasm\ultoa.OBJ+
snndasm\write.OBJ
$$SCW$$.LIB
NUL

snndasm.DEF;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
..\win32\_8087.OBJ+
..\core32\_exit.OBJ+
..\core32\_exit2.OBJ+
..\core32\atoi.OBJ+
..\core32\atol.OBJ+
..\win32\chkstk.OBJ+
..\core32\cinit.OBJ+
..\win32\clearbss.OBJ+
..\core32\dbllng.OBJ+
..\core32\double.OBJ+
..\win32\excptlst.OBJ+
..\core32\faterr.OBJ+
..\core32\floattyp.OBJ+
..\core32\flt87.OBJ+
..\core32\fpatan32.OBJ+
..\core32\fprem32.OBJ+
..\core32\fptan32.OBJ+
..\core32\fputc.OBJ+
..\win32\ftol.OBJ+
..\core32\itoa.OBJ+
..\core32\ldexp.OBJ+
..\core32\llmath.OBJ+
..\core32\ltoa.OBJ+
..\core32\math87.OBJ+
..\core32\mathtrn.OBJ+
..\core32\mathtrnf.OBJ+
..\core32\memchr.OBJ+
..\core32\memcpy.OBJ+
..\core32\memicmp.OBJ+
..\core32\memint.OBJ+
..\core32\movedata.OBJ+
..\core32\page.OBJ+
..\core32\patch32.OBJ+
..\core32\port.OBJ+
..\core32\read.OBJ+
..\core32\rot.OBJ+
..\core32\seg.OBJ+
..\core32\stpcpy.OBJ+
..\core32\strcat.OBJ+
..\core32\strcmpl.OBJ+
..\core32\strncat.OBJ+
..\core32\strncpy.OBJ+
..\core32\strtoul.OBJ+
..\core32\ultoa.OBJ+
..\core32\write.OBJ
$$SCW$$.LIB
NUL

snndasm.DEF;
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
 + $(OUTPUTDIR)\_8087.OBJ + $(OUTPUTDIR)\_exit.OBJ + $(OUTPUTDIR)\_exit2.OBJ + $(OUTPUTDIR)\atoi.OBJ &
		 + $(OUTPUTDIR)\atol.OBJ + $(OUTPUTDIR)\chkstk.OBJ + $(OUTPUTDIR)\cinit.OBJ + $(OUTPUTDIR)\clearbss.OBJ &
		 + $(OUTPUTDIR)\dbllng.OBJ + $(OUTPUTDIR)\double.OBJ + $(OUTPUTDIR)\excptlst.OBJ + $(OUTPUTDIR)\faterr.OBJ &
		 + $(OUTPUTDIR)\floattyp.OBJ + $(OUTPUTDIR)\flt87.OBJ + $(OUTPUTDIR)\fpatan32.OBJ + $(OUTPUTDIR)\fprem32.OBJ &
		 + $(OUTPUTDIR)\fptan32.OBJ + $(OUTPUTDIR)\fputc.OBJ + $(OUTPUTDIR)\ftol.OBJ &
		 + $(OUTPUTDIR)\itoa.OBJ + $(OUTPUTDIR)\ldexp.OBJ + $(OUTPUTDIR)\llmath.OBJ + $(OUTPUTDIR)\ltoa.OBJ &
		 + $(OUTPUTDIR)\math87.OBJ + $(OUTPUTDIR)\mathtrn.OBJ + $(OUTPUTDIR)\mathtrnf.OBJ + $(OUTPUTDIR)\memchr.OBJ &
		 + $(OUTPUTDIR)\memcpy.OBJ + $(OUTPUTDIR)\memicmp.OBJ + $(OUTPUTDIR)\memint.OBJ + $(OUTPUTDIR)\movedata.OBJ &
		 + $(OUTPUTDIR)\page.OBJ + $(OUTPUTDIR)\patch32.OBJ + $(OUTPUTDIR)\port.OBJ + $(OUTPUTDIR)\read.OBJ &
		 + $(OUTPUTDIR)\rot.OBJ + $(OUTPUTDIR)\seg.OBJ + $(OUTPUTDIR)\stpcpy.OBJ &
		 + $(OUTPUTDIR)\strcat.OBJ + $(OUTPUTDIR)\strcmpl.OBJ + $(OUTPUTDIR)\strncat.OBJ + $(OUTPUTDIR)\strncpy.OBJ &
		 + $(OUTPUTDIR)\strtoul.OBJ + $(OUTPUTDIR)\ultoa.OBJ + $(OUTPUTDIR)\write.OBJ
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
		-del snndasm.dpd
		-del $(OBJS)
		-del $(SYMROOT).SYM
		-del $(SYMROOT).PCO

cleanres:

res:		cleanres $(RCFILES) all


link:
		LIB $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(LIBFLAGS)\
				@<<
 + $(OUTPUTDIR)\_8087.OBJ + $(OUTPUTDIR)\_exit.OBJ + $(OUTPUTDIR)\_exit2.OBJ + $(OUTPUTDIR)\atoi.OBJ &
		 + $(OUTPUTDIR)\atol.OBJ + $(OUTPUTDIR)\chkstk.OBJ + $(OUTPUTDIR)\cinit.OBJ + $(OUTPUTDIR)\clearbss.OBJ &
		 + $(OUTPUTDIR)\dbllng.OBJ + $(OUTPUTDIR)\double.OBJ + $(OUTPUTDIR)\excptlst.OBJ + $(OUTPUTDIR)\faterr.OBJ &
		 + $(OUTPUTDIR)\floattyp.OBJ + $(OUTPUTDIR)\flt87.OBJ + $(OUTPUTDIR)\fpatan32.OBJ + $(OUTPUTDIR)\fprem32.OBJ &
		 + $(OUTPUTDIR)\fptan32.OBJ + $(OUTPUTDIR)\fputc.OBJ + $(OUTPUTDIR)\ftol.OBJ &
		 + $(OUTPUTDIR)\itoa.OBJ + $(OUTPUTDIR)\ldexp.OBJ + $(OUTPUTDIR)\llmath.OBJ + $(OUTPUTDIR)\ltoa.OBJ &
		 + $(OUTPUTDIR)\math87.OBJ + $(OUTPUTDIR)\mathtrn.OBJ + $(OUTPUTDIR)\mathtrnf.OBJ + $(OUTPUTDIR)\memchr.OBJ &
		 + $(OUTPUTDIR)\memcpy.OBJ + $(OUTPUTDIR)\memicmp.OBJ + $(OUTPUTDIR)\memint.OBJ + $(OUTPUTDIR)\movedata.OBJ &
		 + $(OUTPUTDIR)\page.OBJ + $(OUTPUTDIR)\patch32.OBJ + $(OUTPUTDIR)\port.OBJ + $(OUTPUTDIR)\read.OBJ &
		 + $(OUTPUTDIR)\rot.OBJ + $(OUTPUTDIR)\seg.OBJ + $(OUTPUTDIR)\stpcpy.OBJ &
		 + $(OUTPUTDIR)\strcat.OBJ + $(OUTPUTDIR)\strcmpl.OBJ + $(OUTPUTDIR)\strncat.OBJ + $(OUTPUTDIR)\strncpy.OBJ &
		 + $(OUTPUTDIR)\strtoul.OBJ + $(OUTPUTDIR)\ultoa.OBJ + $(OUTPUTDIR)\write.OBJ
<<
		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)




!IF EXIST (snndasm.dpd)
!INCLUDE snndasm.dpd
!ENDIF



$(OUTPUTDIR)\_8087.OBJ:	..\win32\_8087.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_8087.obj ..\win32\_8087.asm



$(OUTPUTDIR)\_exit.OBJ:	..\core32\_exit.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_exit.obj ..\core32\_exit.c



$(OUTPUTDIR)\_exit2.OBJ:	..\core32\_exit2.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_exit2.obj ..\core32\_exit2.c



$(OUTPUTDIR)\atoi.OBJ:	..\core32\atoi.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\atoi.obj ..\core32\atoi.asm



$(OUTPUTDIR)\atol.OBJ:	..\core32\atol.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\atol.obj ..\core32\atol.asm



$(OUTPUTDIR)\chkstk.OBJ:	..\win32\chkstk.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\chkstk.obj ..\win32\chkstk.asm



$(OUTPUTDIR)\cinit.OBJ:	..\core32\cinit.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\cinit.obj ..\core32\cinit.asm



$(OUTPUTDIR)\clearbss.OBJ:	..\win32\clearbss.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\clearbss.obj ..\win32\clearbss.asm



$(OUTPUTDIR)\dbllng.OBJ:	..\core32\dbllng.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dbllng.obj ..\core32\dbllng.asm



$(OUTPUTDIR)\double.OBJ:	..\core32\double.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\double.obj ..\core32\double.asm



$(OUTPUTDIR)\excptlst.OBJ:	..\win32\excptlst.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\excptlst.obj ..\win32\excptlst.asm



$(OUTPUTDIR)\faterr.OBJ:	..\core32\faterr.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\faterr.obj ..\core32\faterr.asm



$(OUTPUTDIR)\floattyp.OBJ:	..\core32\floattyp.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\floattyp.obj ..\core32\floattyp.asm



$(OUTPUTDIR)\flt87.OBJ:	..\core32\flt87.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\flt87.obj ..\core32\flt87.asm



$(OUTPUTDIR)\fpatan32.OBJ:	..\core32\fpatan32.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fpatan32.obj ..\core32\fpatan32.asm



$(OUTPUTDIR)\fprem32.OBJ:	..\core32\fprem32.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fprem32.obj ..\core32\fprem32.asm



$(OUTPUTDIR)\fptan32.OBJ:	..\core32\fptan32.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fptan32.obj ..\core32\fptan32.asm



$(OUTPUTDIR)\fputc.OBJ:	..\core32\fputc.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fputc.obj ..\core32\fputc.asm



$(OUTPUTDIR)\ftol.OBJ:	..\win32\ftol.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ftol.obj ..\win32\ftol.asm



$(OUTPUTDIR)\itoa.OBJ:	..\core32\itoa.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\itoa.obj ..\core32\itoa.asm



$(OUTPUTDIR)\ldexp.OBJ:	..\core32\ldexp.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ldexp.obj ..\core32\ldexp.asm



$(OUTPUTDIR)\llmath.OBJ:	..\core32\llmath.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\llmath.obj ..\core32\llmath.asm



$(OUTPUTDIR)\ltoa.OBJ:	..\core32\ltoa.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ltoa.obj ..\core32\ltoa.asm



$(OUTPUTDIR)\math87.OBJ:	..\core32\math87.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\math87.obj ..\core32\math87.asm



$(OUTPUTDIR)\mathtrn.OBJ:	..\core32\mathtrn.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\mathtrn.obj ..\core32\mathtrn.asm



$(OUTPUTDIR)\mathtrnf.OBJ:	..\core32\mathtrnf.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\mathtrnf.obj ..\core32\mathtrnf.asm



$(OUTPUTDIR)\memchr.OBJ:	..\core32\memchr.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\memchr.obj ..\core32\memchr.asm



$(OUTPUTDIR)\memcpy.OBJ:	..\core32\memcpy.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\memcpy.obj ..\core32\memcpy.asm



$(OUTPUTDIR)\memicmp.OBJ:	..\core32\memicmp.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\memicmp.obj ..\core32\memicmp.asm



$(OUTPUTDIR)\memint.OBJ:	..\core32\memint.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\memint.obj ..\core32\memint.asm



$(OUTPUTDIR)\movedata.OBJ:	..\core32\movedata.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\movedata.obj ..\core32\movedata.asm



$(OUTPUTDIR)\page.OBJ:	..\core32\page.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\page.obj ..\core32\page.asm



$(OUTPUTDIR)\patch32.OBJ:	..\core32\patch32.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\patch32.obj ..\core32\patch32.asm



$(OUTPUTDIR)\port.OBJ:	..\core32\port.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\port.obj ..\core32\port.asm



$(OUTPUTDIR)\read.OBJ:	..\core32\read.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\read.obj ..\core32\read.asm



$(OUTPUTDIR)\rot.OBJ:	..\core32\rot.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\rot.obj ..\core32\rot.asm



$(OUTPUTDIR)\seg.OBJ:	..\core32\seg.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\seg.obj ..\core32\seg.asm



$(OUTPUTDIR)\stpcpy.OBJ:	..\core32\stpcpy.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\stpcpy.obj ..\core32\stpcpy.asm



$(OUTPUTDIR)\strcat.OBJ:	..\core32\strcat.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strcat.obj ..\core32\strcat.asm



$(OUTPUTDIR)\strcmpl.OBJ:	..\core32\strcmpl.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strcmpl.obj ..\core32\strcmpl.asm



$(OUTPUTDIR)\strncat.OBJ:	..\core32\strncat.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strncat.obj ..\core32\strncat.asm



$(OUTPUTDIR)\strncpy.OBJ:	..\core32\strncpy.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strncpy.obj ..\core32\strncpy.asm



$(OUTPUTDIR)\strtoul.OBJ:	..\core32\strtoul.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strtoul.obj ..\core32\strtoul.asm



$(OUTPUTDIR)\ultoa.OBJ:	..\core32\ultoa.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ultoa.obj ..\core32\ultoa.asm



$(OUTPUTDIR)\write.OBJ:	..\core32\write.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\write.obj ..\core32\write.asm


$(SYMROOT).SYM: ..\include\precomp.h
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF$(SYMROOT).SYM -o$(SYMROOT).PCO ..\include\precomp.h



