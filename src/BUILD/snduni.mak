ORIGIN		= Digital Mars C++
ORIGIN_VER	= Version 7.22
VERSION		= RELEASE

!IFDEF SUB_DEBUG
DEBUG		= $(SUB_DEBUG)
NDEBUG		= !$(SUB_DEBUG)
!ELSE
DEBUG		= 0
NDEBUG		= 1
!ENDIF

PROJ		= SNDUNI
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

HEADERS		= ..\..\include\assert.h ..\..\include\share.h ..\..\include\fp.h  \
		..\..\include\fltpnt.h ..\..\include\fcntl.h ..\..\include\stddef.h ..\..\include\math.h  \
		..\..\include\limits.h ..\..\include\heapstat.h ..\..\include\malloc.h ..\include\setlocal.h  \
		..\..\include\locale.h ..\include\lcapi32.h ..\include\semlock.h ..\include\scio.h  \
		..\include\scrtl.h ..\..\include\dos.h ..\include\ehsup.h ..\..\include\fenv.h  \
		..\..\include\fltenv.h ..\..\include\float.h ..\include\mt.h ..\..\include\errno.h  \
		..\include\sctchar.h ..\..\include\time.h ..\..\include\stdio.h ..\..\include\sys\stat.h  \
		..\..\include\process.h ..\..\include\io.h ..\..\include\direct.h ..\..\include\wchar.h  \
		..\..\include\tchar.h 

DEFFILE		= SNDUNI.DEF

!IF $(DEBUG)
OUTPUTDIR	= snduni
!IF EXIST (snduni)
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

SYMROOT		=  snduni\precomp 
SYMS		= $(SYMROOT).SYM 
LIBS		= 

CFLAGS		=  -HIprecomp.h -Ae -Ar -r -mn -NL -ND -5 -a8 -Nc -c -H -HDsnduni -HO- -wx -g 
LFLAGS		=  /CO /NOI /DE /XN /A:512
DEFINES		= -D_MT -DUNICODE -D_UNICODE=1
!ELSE
OUTPUTDIR	= snduni
!IF EXIST (snduni)
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

SYMROOT		=  snduni\precomp 
SYMS		= $(SYMROOT).SYM 
LIBS		= 

CFLAGS		=  -HIprecomp.h -Ae -Ar -r -mn -o+time -NL -ND -5 -a8 -Nc -c -H -HDsnduni -HO- -wx 
LFLAGS		=  /NOI /DE /E /PACKF /XN /A:512
DEFINES		= -D_MT -DUNICODE -D_UNICODE=1
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

INCLUDES	= -I..\include -I..\..\include

INCLUDEDOBJS	= 

OBJS		=  $(OUTPUTDIR)\vwprintf.OBJ  $(OUTPUTDIR)\wasctime.OBJ  $(OUTPUTDIR)\wchdir.OBJ  \
		 $(OUTPUTDIR)\wchmod.OBJ  $(OUTPUTDIR)\wconstar.OBJ  $(OUTPUTDIR)\wcscat.OBJ  $(OUTPUTDIR)\wcscmp.OBJ  \
		 $(OUTPUTDIR)\wcscoll.OBJ  $(OUTPUTDIR)\wcscspn.OBJ  $(OUTPUTDIR)\wcsftime.OBJ  $(OUTPUTDIR)\wcsicmp.OBJ  \
		 $(OUTPUTDIR)\wcsicoll.OBJ  $(OUTPUTDIR)\wcslen.OBJ  $(OUTPUTDIR)\wcslwr.OBJ  $(OUTPUTDIR)\wcsncat.OBJ  \
		 $(OUTPUTDIR)\wcsncmp.OBJ  $(OUTPUTDIR)\wcsncoll.OBJ  $(OUTPUTDIR)\wcsncpy.OBJ  $(OUTPUTDIR)\wcsnicol.OBJ  \
		 $(OUTPUTDIR)\wcsnset.OBJ  $(OUTPUTDIR)\wcspbrk.OBJ  $(OUTPUTDIR)\wcsrchr.OBJ  $(OUTPUTDIR)\wcsrev.OBJ  \
		 $(OUTPUTDIR)\wcsset.OBJ  $(OUTPUTDIR)\wcsspn.OBJ  $(OUTPUTDIR)\wcsstr.OBJ  $(OUTPUTDIR)\wcstod.OBJ  \
		 $(OUTPUTDIR)\wcstok.OBJ  $(OUTPUTDIR)\wcstol.OBJ  $(OUTPUTDIR)\wcstombs.OBJ  $(OUTPUTDIR)\wcsupr.OBJ  \
		 $(OUTPUTDIR)\wcsxfrm.OBJ  $(OUTPUTDIR)\wctime.OBJ  $(OUTPUTDIR)\wctomb.OBJ  $(OUTPUTDIR)\wctype.OBJ  \
		 $(OUTPUTDIR)\wexec2.OBJ  $(OUTPUTDIR)\wexecl.OBJ  $(OUTPUTDIR)\wexecle.OBJ  $(OUTPUTDIR)\wexeclp.OBJ  \
		 $(OUTPUTDIR)\wexeclpe.OBJ  $(OUTPUTDIR)\wexecv.OBJ  $(OUTPUTDIR)\wexecve.OBJ  $(OUTPUTDIR)\wexecvp.OBJ  \
		 $(OUTPUTDIR)\wexecvpe.OBJ  $(OUTPUTDIR)\wfdopen.OBJ  $(OUTPUTDIR)\wfind.OBJ  $(OUTPUTDIR)\wfloatcv.OBJ  \
		 $(OUTPUTDIR)\wfmt.OBJ  $(OUTPUTDIR)\wfopen.OBJ  $(OUTPUTDIR)\wfreopen.OBJ  $(OUTPUTDIR)\wfsopen.OBJ  \
		 $(OUTPUTDIR)\wfullpat.OBJ  $(OUTPUTDIR)\wgetcwd.OBJ  $(OUTPUTDIR)\wgetdcwd.OBJ  $(OUTPUTDIR)\wio.OBJ  \
		 $(OUTPUTDIR)\wmakepat.OBJ  $(OUTPUTDIR)\wmkdir.OBJ  $(OUTPUTDIR)\wmktemp.OBJ  $(OUTPUTDIR)\wperror.OBJ  \
		 $(OUTPUTDIR)\wprintf.OBJ  $(OUTPUTDIR)\wrddir.OBJ  $(OUTPUTDIR)\wscanf.OBJ  $(OUTPUTDIR)\wsearche.OBJ  \
		 $(OUTPUTDIR)\wsetargv.OBJ  $(OUTPUTDIR)\wsetloca.OBJ  $(OUTPUTDIR)\wsopenf.OBJ  $(OUTPUTDIR)\wspawlpe.OBJ  \
		 $(OUTPUTDIR)\wspawn.OBJ  $(OUTPUTDIR)\wspawnl.OBJ  $(OUTPUTDIR)\wspawnle.OBJ  $(OUTPUTDIR)\wspawnlp.OBJ  \
		 $(OUTPUTDIR)\wspawnv.OBJ  $(OUTPUTDIR)\wspawnve.OBJ  $(OUTPUTDIR)\wspawnvp.OBJ  $(OUTPUTDIR)\wspawvpe.OBJ  \
		 $(OUTPUTDIR)\wsplitpa.OBJ  $(OUTPUTDIR)\wstat.OBJ  $(OUTPUTDIR)\wstrdate.OBJ  $(OUTPUTDIR)\wstrtime.OBJ  \
		 $(OUTPUTDIR)\wsystem.OBJ  $(OUTPUTDIR)\wtmpnam.OBJ  $(OUTPUTDIR)\wunlink.OBJ  $(OUTPUTDIR)\wutime.OBJ  \
		 $(OUTPUTDIR)\wwinstar.OBJ  $(OUTPUTDIR)\wtoi.OBJ  $(OUTPUTDIR)\wtol.OBJ  $(OUTPUTDIR)\_wprintf.OBJ  \
		 $(OUTPUTDIR)\_wfopen.OBJ 

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
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -osnduni\$(*B).sym $*.h

.HPP.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -osnduni\$(*B).sym $*.hpp

.HXX.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -osnduni\$(*B).sym $*.hxx

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
snduni\precomp.PCO+
snduni\vwprintf.OBJ+
snduni\wasctime.OBJ+
snduni\wchdir.OBJ+
snduni\wchmod.OBJ+
snduni\wconstar.OBJ+
snduni\wcscat.OBJ+
snduni\wcscmp.OBJ+
snduni\wcscoll.OBJ+
snduni\wcscspn.OBJ+
snduni\wcsftime.OBJ+
snduni\wcsicmp.OBJ+
snduni\wcsicoll.OBJ+
snduni\wcslen.OBJ+
snduni\wcslwr.OBJ+
snduni\wcsncat.OBJ+
snduni\wcsncmp.OBJ+
snduni\wcsncoll.OBJ+
snduni\wcsncpy.OBJ+
snduni\wcsnicol.OBJ+
snduni\wcsnset.OBJ+
snduni\wcspbrk.OBJ+
snduni\wcsrchr.OBJ+
snduni\wcsrev.OBJ+
snduni\wcsset.OBJ+
snduni\wcsspn.OBJ+
snduni\wcsstr.OBJ+
snduni\wcstod.OBJ+
snduni\wcstok.OBJ+
snduni\wcstol.OBJ+
snduni\wcstombs.OBJ+
snduni\wcsupr.OBJ+
snduni\wcsxfrm.OBJ+
snduni\wctime.OBJ+
snduni\wctomb.OBJ+
snduni\wctype.OBJ+
snduni\wexec2.OBJ+
snduni\wexecl.OBJ+
snduni\wexecle.OBJ+
snduni\wexeclp.OBJ+
snduni\wexeclpe.OBJ+
snduni\wexecv.OBJ+
snduni\wexecve.OBJ+
snduni\wexecvp.OBJ+
snduni\wexecvpe.OBJ+
snduni\wfdopen.OBJ+
snduni\wfind.OBJ+
snduni\wfloatcv.OBJ+
snduni\wfmt.OBJ+
snduni\wfopen.OBJ+
snduni\wfreopen.OBJ+
snduni\wfsopen.OBJ+
snduni\wfullpat.OBJ+
snduni\wgetcwd.OBJ+
snduni\wgetdcwd.OBJ+
snduni\wio.OBJ+
snduni\wmakepat.OBJ+
snduni\wmkdir.OBJ+
snduni\wmktemp.OBJ+
snduni\wperror.OBJ+
snduni\wprintf.OBJ+
snduni\wrddir.OBJ+
snduni\wscanf.OBJ+
snduni\wsearche.OBJ+
snduni\wsetargv.OBJ+
snduni\wsetloca.OBJ+
snduni\wsopenf.OBJ+
snduni\wspawlpe.OBJ+
snduni\wspawn.OBJ+
snduni\wspawnl.OBJ+
snduni\wspawnle.OBJ+
snduni\wspawnlp.OBJ+
snduni\wspawnv.OBJ+
snduni\wspawnve.OBJ+
snduni\wspawnvp.OBJ+
snduni\wspawvpe.OBJ+
snduni\wsplitpa.OBJ+
snduni\wstat.OBJ+
snduni\wstrdate.OBJ+
snduni\wstrtime.OBJ+
snduni\wsystem.OBJ+
snduni\wtmpnam.OBJ+
snduni\wunlink.OBJ+
snduni\wutime.OBJ+
snduni\wwinstar.OBJ+
snduni\wtoi.OBJ+
snduni\wtol.OBJ+
snduni\_wprintf.OBJ+
snduni\_wfopen.OBJ
.\$$SCW$$.LIB
NUL

SNDUNI.DEF;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
snduni\precomp.PCO+
snduni\vwprintf.obj+
snduni\wasctime.OBJ+
snduni\wchdir.OBJ+
snduni\wchmod.OBJ+
snduni\wconstar.OBJ+
snduni\wcscat.OBJ+
snduni\wcscmp.OBJ+
snduni\wcscoll.OBJ+
snduni\wcscspn.OBJ+
snduni\wcsftime.OBJ+
snduni\wcsicmp.OBJ+
snduni\wcsicoll.OBJ+
snduni\wcslen.OBJ+
snduni\wcslwr.OBJ+
snduni\wcsncat.OBJ+
snduni\wcsncmp.OBJ+
snduni\wcsncoll.OBJ+
snduni\wcsncpy.OBJ+
snduni\wcsnicol.OBJ+
snduni\wcsnset.OBJ+
snduni\wcspbrk.OBJ+
snduni\wcsrchr.OBJ+
snduni\wcsrev.OBJ+
snduni\wcsset.OBJ+
snduni\wcsspn.OBJ+
snduni\wcsstr.OBJ+
snduni\wcstod.OBJ+
snduni\wcstok.OBJ+
snduni\wcstol.OBJ+
snduni\wcstombs.OBJ+
snduni\wcsupr.OBJ+
snduni\wcsxfrm.OBJ+
snduni\wctime.OBJ+
snduni\wctomb.OBJ+
snduni\wctype.OBJ+
snduni\wexec2.OBJ+
snduni\wexecl.OBJ+
snduni\wexecle.OBJ+
snduni\wexeclp.OBJ+
snduni\wexeclpe.OBJ+
snduni\wexecv.OBJ+
snduni\wexecve.OBJ+
snduni\wexecvp.OBJ+
snduni\wexecvpe.OBJ+
snduni\wfdopen.OBJ+
snduni\wfind.OBJ+
snduni\wfloatcv.OBJ+
snduni\wfmt.OBJ+
snduni\wfopen.OBJ+
snduni\wfreopen.OBJ+
snduni\wfsopen.OBJ+
snduni\wfullpat.OBJ+
snduni\wgetcwd.OBJ+
snduni\wgetdcwd.OBJ+
snduni\wio.OBJ+
snduni\wmakepat.OBJ+
snduni\wmkdir.OBJ+
snduni\wmktemp.OBJ+
snduni\wperror.OBJ+
snduni\wprintf.OBJ+
snduni\wrddir.OBJ+
snduni\wscanf.OBJ+
snduni\wsearche.OBJ+
snduni\wsetargv.OBJ+
snduni\wsetloca.OBJ+
snduni\wsopenf.OBJ+
snduni\wspawlpe.OBJ+
snduni\wspawn.OBJ+
snduni\wspawnl.OBJ+
snduni\wspawnle.OBJ+
snduni\wspawnlp.OBJ+
snduni\wspawnv.OBJ+
snduni\wspawnve.OBJ+
snduni\wspawnvp.OBJ+
snduni\wspawvpe.OBJ+
snduni\wsplitpa.OBJ+
snduni\wstat.OBJ+
snduni\wstrdate.OBJ+
snduni\wstrtime.OBJ+
snduni\wsystem.OBJ+
snduni\wtmpnam.OBJ+
snduni\wunlink.OBJ+
snduni\wutime.OBJ+
snduni\wwinstar.OBJ+
snduni\wtoi.OBJ+
snduni\wtol.OBJ+
snduni\_wprintf.OBJ+
snduni\_wfopen.OBJ
.\$$SCW$$.LIB
NUL

SNDUNI.DEF;
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
 +$(OUTPUTDIR)\vwprintf.OBJ +$(OUTPUTDIR)\wasctime.OBJ + $(OUTPUTDIR)\wchdir.OBJ &
		 + $(OUTPUTDIR)\wchmod.OBJ + $(OUTPUTDIR)\wconstar.OBJ + $(OUTPUTDIR)\wcscat.OBJ + $(OUTPUTDIR)\wcscmp.OBJ &
		 + $(OUTPUTDIR)\wcscoll.OBJ + $(OUTPUTDIR)\wcscspn.OBJ + $(OUTPUTDIR)\wcsftime.OBJ + $(OUTPUTDIR)\wcsicmp.OBJ &
		 + $(OUTPUTDIR)\wcsicoll.OBJ + $(OUTPUTDIR)\wcslen.OBJ + $(OUTPUTDIR)\wcslwr.OBJ + $(OUTPUTDIR)\wcsncat.OBJ &
		 + $(OUTPUTDIR)\wcsncmp.OBJ + $(OUTPUTDIR)\wcsncoll.OBJ + $(OUTPUTDIR)\wcsncpy.OBJ + $(OUTPUTDIR)\wcsnicol.OBJ &
		 + $(OUTPUTDIR)\wcsnset.OBJ + $(OUTPUTDIR)\wcspbrk.OBJ + $(OUTPUTDIR)\wcsrchr.OBJ + $(OUTPUTDIR)\wcsrev.OBJ &
		 + $(OUTPUTDIR)\wcsset.OBJ + $(OUTPUTDIR)\wcsspn.OBJ + $(OUTPUTDIR)\wcsstr.OBJ + $(OUTPUTDIR)\wcstod.OBJ &
		 + $(OUTPUTDIR)\wcstok.OBJ + $(OUTPUTDIR)\wcstol.OBJ + $(OUTPUTDIR)\wcstombs.OBJ + $(OUTPUTDIR)\wcsupr.OBJ &
		 + $(OUTPUTDIR)\wcsxfrm.OBJ + $(OUTPUTDIR)\wctime.OBJ + $(OUTPUTDIR)\wctomb.OBJ + $(OUTPUTDIR)\wctype.OBJ &
		 + $(OUTPUTDIR)\wexec2.OBJ + $(OUTPUTDIR)\wexecl.OBJ + $(OUTPUTDIR)\wexecle.OBJ + $(OUTPUTDIR)\wexeclp.OBJ &
		 + $(OUTPUTDIR)\wexeclpe.OBJ + $(OUTPUTDIR)\wexecv.OBJ + $(OUTPUTDIR)\wexecve.OBJ + $(OUTPUTDIR)\wexecvp.OBJ &
		 + $(OUTPUTDIR)\wexecvpe.OBJ + $(OUTPUTDIR)\wfdopen.OBJ + $(OUTPUTDIR)\wfind.OBJ + $(OUTPUTDIR)\wfloatcv.OBJ &
		 + $(OUTPUTDIR)\wfmt.OBJ + $(OUTPUTDIR)\wfopen.OBJ + $(OUTPUTDIR)\wfreopen.OBJ + $(OUTPUTDIR)\wfsopen.OBJ &
		 + $(OUTPUTDIR)\wfullpat.OBJ + $(OUTPUTDIR)\wgetcwd.OBJ + $(OUTPUTDIR)\wgetdcwd.OBJ + $(OUTPUTDIR)\wio.OBJ &
		 + $(OUTPUTDIR)\wmakepat.OBJ + $(OUTPUTDIR)\wmkdir.OBJ + $(OUTPUTDIR)\wmktemp.OBJ + $(OUTPUTDIR)\wperror.OBJ &
		 + $(OUTPUTDIR)\wprintf.OBJ + $(OUTPUTDIR)\wrddir.OBJ + $(OUTPUTDIR)\wscanf.OBJ + $(OUTPUTDIR)\wsearche.OBJ &
		 + $(OUTPUTDIR)\wsetargv.OBJ + $(OUTPUTDIR)\wsetloca.OBJ + $(OUTPUTDIR)\wsopenf.OBJ + $(OUTPUTDIR)\wspawlpe.OBJ &
		 + $(OUTPUTDIR)\wspawn.OBJ + $(OUTPUTDIR)\wspawnl.OBJ + $(OUTPUTDIR)\wspawnle.OBJ + $(OUTPUTDIR)\wspawnlp.OBJ &
		 + $(OUTPUTDIR)\wspawnv.OBJ + $(OUTPUTDIR)\wspawnve.OBJ + $(OUTPUTDIR)\wspawnvp.OBJ + $(OUTPUTDIR)\wspawvpe.OBJ &
		 + $(OUTPUTDIR)\wsplitpa.OBJ + $(OUTPUTDIR)\wstat.OBJ + $(OUTPUTDIR)\wstrdate.OBJ + $(OUTPUTDIR)\wstrtime.OBJ &
		 + $(OUTPUTDIR)\wsystem.OBJ + $(OUTPUTDIR)\wtmpnam.OBJ + $(OUTPUTDIR)\wunlink.OBJ + $(OUTPUTDIR)\wutime.OBJ &
		 + $(OUTPUTDIR)\wwinstar.OBJ + $(OUTPUTDIR)\wtoi.OBJ + $(OUTPUTDIR)\wtol.OBJ + $(OUTPUTDIR)\_wprintf.OBJ &
		 + $(OUTPUTDIR)\_wfopen.OBJ
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
		-del SNDUNI.dpd
		-del $(OBJS)
		-del $(SYMROOT).SYM
		-del $(SYMROOT).PCO

cleanres:

res:		cleanres $(RCFILES) all


link:
		LIB $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(LIBFLAGS)\
				@<<
 +$(OUTPUTDIR)\vwprintf.OBJ +$(OUTPUTDIR)\wasctime.OBJ + $(OUTPUTDIR)\wchdir.OBJ &
		 + $(OUTPUTDIR)\wchmod.OBJ + $(OUTPUTDIR)\wconstar.OBJ + $(OUTPUTDIR)\wcscat.OBJ + $(OUTPUTDIR)\wcscmp.OBJ &
		 + $(OUTPUTDIR)\wcscoll.OBJ + $(OUTPUTDIR)\wcscspn.OBJ + $(OUTPUTDIR)\wcsftime.OBJ + $(OUTPUTDIR)\wcsicmp.OBJ &
		 + $(OUTPUTDIR)\wcsicoll.OBJ + $(OUTPUTDIR)\wcslen.OBJ + $(OUTPUTDIR)\wcslwr.OBJ + $(OUTPUTDIR)\wcsncat.OBJ &
		 + $(OUTPUTDIR)\wcsncmp.OBJ + $(OUTPUTDIR)\wcsncoll.OBJ + $(OUTPUTDIR)\wcsncpy.OBJ + $(OUTPUTDIR)\wcsnicol.OBJ &
		 + $(OUTPUTDIR)\wcsnset.OBJ + $(OUTPUTDIR)\wcspbrk.OBJ + $(OUTPUTDIR)\wcsrchr.OBJ + $(OUTPUTDIR)\wcsrev.OBJ &
		 + $(OUTPUTDIR)\wcsset.OBJ + $(OUTPUTDIR)\wcsspn.OBJ + $(OUTPUTDIR)\wcsstr.OBJ + $(OUTPUTDIR)\wcstod.OBJ &
		 + $(OUTPUTDIR)\wcstok.OBJ + $(OUTPUTDIR)\wcstol.OBJ + $(OUTPUTDIR)\wcstombs.OBJ + $(OUTPUTDIR)\wcsupr.OBJ &
		 + $(OUTPUTDIR)\wcsxfrm.OBJ + $(OUTPUTDIR)\wctime.OBJ + $(OUTPUTDIR)\wctomb.OBJ + $(OUTPUTDIR)\wctype.OBJ &
		 + $(OUTPUTDIR)\wexec2.OBJ + $(OUTPUTDIR)\wexecl.OBJ + $(OUTPUTDIR)\wexecle.OBJ + $(OUTPUTDIR)\wexeclp.OBJ &
		 + $(OUTPUTDIR)\wexeclpe.OBJ + $(OUTPUTDIR)\wexecv.OBJ + $(OUTPUTDIR)\wexecve.OBJ + $(OUTPUTDIR)\wexecvp.OBJ &
		 + $(OUTPUTDIR)\wexecvpe.OBJ + $(OUTPUTDIR)\wfdopen.OBJ + $(OUTPUTDIR)\wfind.OBJ + $(OUTPUTDIR)\wfloatcv.OBJ &
		 + $(OUTPUTDIR)\wfmt.OBJ + $(OUTPUTDIR)\wfopen.OBJ + $(OUTPUTDIR)\wfreopen.OBJ + $(OUTPUTDIR)\wfsopen.OBJ &
		 + $(OUTPUTDIR)\wfullpat.OBJ + $(OUTPUTDIR)\wgetcwd.OBJ + $(OUTPUTDIR)\wgetdcwd.OBJ + $(OUTPUTDIR)\wio.OBJ &
		 + $(OUTPUTDIR)\wmakepat.OBJ + $(OUTPUTDIR)\wmkdir.OBJ + $(OUTPUTDIR)\wmktemp.OBJ + $(OUTPUTDIR)\wperror.OBJ &
		 + $(OUTPUTDIR)\wprintf.OBJ + $(OUTPUTDIR)\wrddir.OBJ + $(OUTPUTDIR)\wscanf.OBJ + $(OUTPUTDIR)\wsearche.OBJ &
		 + $(OUTPUTDIR)\wsetargv.OBJ + $(OUTPUTDIR)\wsetloca.OBJ + $(OUTPUTDIR)\wsopenf.OBJ + $(OUTPUTDIR)\wspawlpe.OBJ &
		 + $(OUTPUTDIR)\wspawn.OBJ + $(OUTPUTDIR)\wspawnl.OBJ + $(OUTPUTDIR)\wspawnle.OBJ + $(OUTPUTDIR)\wspawnlp.OBJ &
		 + $(OUTPUTDIR)\wspawnv.OBJ + $(OUTPUTDIR)\wspawnve.OBJ + $(OUTPUTDIR)\wspawnvp.OBJ + $(OUTPUTDIR)\wspawvpe.OBJ &
		 + $(OUTPUTDIR)\wsplitpa.OBJ + $(OUTPUTDIR)\wstat.OBJ + $(OUTPUTDIR)\wstrdate.OBJ + $(OUTPUTDIR)\wstrtime.OBJ &
		 + $(OUTPUTDIR)\wsystem.OBJ + $(OUTPUTDIR)\wtmpnam.OBJ + $(OUTPUTDIR)\wunlink.OBJ + $(OUTPUTDIR)\wutime.OBJ &
		 + $(OUTPUTDIR)\wwinstar.OBJ + $(OUTPUTDIR)\wtoi.OBJ + $(OUTPUTDIR)\wtol.OBJ + $(OUTPUTDIR)\_wprintf.OBJ &
		 + $(OUTPUTDIR)\_wfopen.OBJ
<<
		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)




!IF EXIST (SNDUNI.dpd)
!INCLUDE SNDUNI.dpd
!ENDIF



$(OUTPUTDIR)\vwprintf.OBJ:	..\core\vwprintf.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\vwprintf.obj ..\core\vwprintf.c

$(OUTPUTDIR)\wasctime.OBJ:	..\core\wasctime.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wasctime.obj ..\core\wasctime.c



$(OUTPUTDIR)\wchdir.OBJ:	..\win32\wchdir.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wchdir.obj ..\win32\wchdir.c



$(OUTPUTDIR)\wchmod.OBJ:	..\win32\wchmod.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wchmod.obj ..\win32\wchmod.c



$(OUTPUTDIR)\wconstar.OBJ:	..\win32\wconstar.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wconstar.obj ..\win32\wconstar.c



$(OUTPUTDIR)\wcscat.OBJ:	..\core\wcscat.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcscat.obj ..\core\wcscat.c



$(OUTPUTDIR)\wcscmp.OBJ:	..\core\wcscmp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcscmp.obj ..\core\wcscmp.c



$(OUTPUTDIR)\wcscoll.OBJ:	..\core\wcscoll.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcscoll.obj ..\core\wcscoll.c



$(OUTPUTDIR)\wcscspn.OBJ:	..\core\wcscspn.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcscspn.obj ..\core\wcscspn.c



$(OUTPUTDIR)\wcsftime.OBJ:	..\core\wcsftime.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcsftime.obj ..\core\wcsftime.c



$(OUTPUTDIR)\wcsicmp.OBJ:	..\core\wcsicmp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcsicmp.obj ..\core\wcsicmp.c



$(OUTPUTDIR)\wcsicoll.OBJ:	..\core\wcsicoll.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcsicoll.obj ..\core\wcsicoll.c



$(OUTPUTDIR)\wcslen.OBJ:	..\core\wcslen.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcslen.obj ..\core\wcslen.c



$(OUTPUTDIR)\wcslwr.OBJ:	..\core\wcslwr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcslwr.obj ..\core\wcslwr.c



$(OUTPUTDIR)\wcsncat.OBJ:	..\core\wcsncat.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcsncat.obj ..\core\wcsncat.c



$(OUTPUTDIR)\wcsncmp.OBJ:	..\core\wcsncmp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcsncmp.obj ..\core\wcsncmp.c



$(OUTPUTDIR)\wcsncoll.OBJ:	..\core\wcsncoll.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcsncoll.obj ..\core\wcsncoll.c



$(OUTPUTDIR)\wcsncpy.OBJ:	..\core\wcsncpy.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcsncpy.obj ..\core\wcsncpy.c



$(OUTPUTDIR)\wcsnicol.OBJ:	..\core\wcsnicol.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcsnicol.obj ..\core\wcsnicol.c



$(OUTPUTDIR)\wcsnset.OBJ:	..\core\wcsnset.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcsnset.obj ..\core\wcsnset.c



$(OUTPUTDIR)\wcspbrk.OBJ:	..\core\wcspbrk.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcspbrk.obj ..\core\wcspbrk.c



$(OUTPUTDIR)\wcsrchr.OBJ:	..\core\wcsrchr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcsrchr.obj ..\core\wcsrchr.c



$(OUTPUTDIR)\wcsrev.OBJ:	..\core\wcsrev.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcsrev.obj ..\core\wcsrev.c



$(OUTPUTDIR)\wcsset.OBJ:	..\core\wcsset.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcsset.obj ..\core\wcsset.c



$(OUTPUTDIR)\wcsspn.OBJ:	..\core\wcsspn.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcsspn.obj ..\core\wcsspn.c



$(OUTPUTDIR)\wcsstr.OBJ:	..\core\wcsstr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcsstr.obj ..\core\wcsstr.c



$(OUTPUTDIR)\wcstod.OBJ:	..\core\wcstod.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcstod.obj ..\core\wcstod.c



$(OUTPUTDIR)\wcstok.OBJ:	..\core\wcstok.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcstok.obj ..\core\wcstok.c



$(OUTPUTDIR)\wcstol.OBJ:	..\core\wcstol.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcstol.obj ..\core\wcstol.c



$(OUTPUTDIR)\wcstombs.OBJ:	..\core\wcstombs.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcstombs.obj ..\core\wcstombs.c



$(OUTPUTDIR)\wcsupr.OBJ:	..\core\wcsupr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcsupr.obj ..\core\wcsupr.c



$(OUTPUTDIR)\wcsxfrm.OBJ:	..\core\wcsxfrm.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcsxfrm.obj ..\core\wcsxfrm.c



$(OUTPUTDIR)\wctime.OBJ:	..\core\wctime.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wctime.obj ..\core\wctime.c



$(OUTPUTDIR)\wctomb.OBJ:	..\core\wctomb.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wctomb.obj ..\core\wctomb.c



$(OUTPUTDIR)\wctype.OBJ:	..\core\wctype.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wctype.obj ..\core\wctype.c



$(OUTPUTDIR)\wexec2.OBJ:	..\win32\wexec2.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wexec2.obj ..\win32\wexec2.c



$(OUTPUTDIR)\wexecl.OBJ:	..\core\wexecl.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wexecl.obj ..\core\wexecl.c



$(OUTPUTDIR)\wexecle.OBJ:	..\core\wexecle.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wexecle.obj ..\core\wexecle.c



$(OUTPUTDIR)\wexeclp.OBJ:	..\core\wexeclp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wexeclp.obj ..\core\wexeclp.c



$(OUTPUTDIR)\wexeclpe.OBJ:	..\core\wexeclpe.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wexeclpe.obj ..\core\wexeclpe.c



$(OUTPUTDIR)\wexecv.OBJ:	..\core\wexecv.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wexecv.obj ..\core\wexecv.c



$(OUTPUTDIR)\wexecve.OBJ:	..\core\wexecve.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wexecve.obj ..\core\wexecve.c



$(OUTPUTDIR)\wexecvp.OBJ:	..\core\wexecvp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wexecvp.obj ..\core\wexecvp.c



$(OUTPUTDIR)\wexecvpe.OBJ:	..\core\wexecvpe.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wexecvpe.obj ..\core\wexecvpe.c



$(OUTPUTDIR)\wfdopen.OBJ:	..\core\wfdopen.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wfdopen.obj ..\core\wfdopen.c



$(OUTPUTDIR)\wfind.OBJ:	..\win32\wfind.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wfind.obj ..\win32\wfind.c



$(OUTPUTDIR)\wfloatcv.OBJ:	..\core\wfloatcv.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wfloatcv.obj ..\core\wfloatcv.c



$(OUTPUTDIR)\wfmt.OBJ:	..\core\wfmt.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wfmt.obj ..\core\wfmt.c



$(OUTPUTDIR)\wfopen.OBJ:	..\core\wfopen.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wfopen.obj ..\core\wfopen.c



$(OUTPUTDIR)\wfreopen.OBJ:	..\core\wfreopen.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wfreopen.obj ..\core\wfreopen.c



$(OUTPUTDIR)\wfsopen.OBJ:	..\core\wfsopen.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wfsopen.obj ..\core\wfsopen.c



$(OUTPUTDIR)\wfullpat.OBJ:	..\core\wfullpat.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wfullpat.obj ..\core\wfullpat.c



$(OUTPUTDIR)\wgetcwd.OBJ:	..\win32\wgetcwd.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wgetcwd.obj ..\win32\wgetcwd.c



$(OUTPUTDIR)\wgetdcwd.OBJ:	..\core\wgetdcwd.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wgetdcwd.obj ..\core\wgetdcwd.c



$(OUTPUTDIR)\wio.OBJ:	..\win32\wio.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wio.obj ..\win32\wio.c



$(OUTPUTDIR)\wmakepat.OBJ:	..\core\wmakepat.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wmakepat.obj ..\core\wmakepat.c



$(OUTPUTDIR)\wmkdir.OBJ:	..\win32\wmkdir.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wmkdir.obj ..\win32\wmkdir.c



$(OUTPUTDIR)\wmktemp.OBJ:	..\core\wmktemp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wmktemp.obj ..\core\wmktemp.c



$(OUTPUTDIR)\wperror.OBJ:	..\core\wperror.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wperror.obj ..\core\wperror.c



$(OUTPUTDIR)\wprintf.OBJ:	..\core\wprintf.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wprintf.obj ..\core\wprintf.c



$(OUTPUTDIR)\wrddir.OBJ:	..\win32\wrddir.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wrddir.obj ..\win32\wrddir.c



$(OUTPUTDIR)\wscanf.OBJ:	..\core\wscanf.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wscanf.obj ..\core\wscanf.c



$(OUTPUTDIR)\wsearche.OBJ:	..\core\wsearche.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wsearche.obj ..\core\wsearche.c



$(OUTPUTDIR)\wsetargv.OBJ:	..\win32\wsetargv.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wsetargv.obj ..\win32\wsetargv.c



$(OUTPUTDIR)\wsetloca.OBJ:	..\core\wsetloca.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wsetloca.obj ..\core\wsetloca.c



$(OUTPUTDIR)\wsopenf.OBJ:	..\core\wsopenf.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wsopenf.obj ..\core\wsopenf.c



$(OUTPUTDIR)\wspawlpe.OBJ:	..\core\wspawlpe.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wspawlpe.obj ..\core\wspawlpe.c



$(OUTPUTDIR)\wspawn.OBJ:	..\core\wspawn.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wspawn.obj ..\core\wspawn.c



$(OUTPUTDIR)\wspawnl.OBJ:	..\core\wspawnl.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wspawnl.obj ..\core\wspawnl.c



$(OUTPUTDIR)\wspawnle.OBJ:	..\core\wspawnle.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wspawnle.obj ..\core\wspawnle.c



$(OUTPUTDIR)\wspawnlp.OBJ:	..\core\wspawnlp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wspawnlp.obj ..\core\wspawnlp.c



$(OUTPUTDIR)\wspawnv.OBJ:	..\core\wspawnv.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wspawnv.obj ..\core\wspawnv.c



$(OUTPUTDIR)\wspawnve.OBJ:	..\core\wspawnve.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wspawnve.obj ..\core\wspawnve.c



$(OUTPUTDIR)\wspawnvp.OBJ:	..\core\wspawnvp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wspawnvp.obj ..\core\wspawnvp.c



$(OUTPUTDIR)\wspawvpe.OBJ:	..\core\wspawvpe.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wspawvpe.obj ..\core\wspawvpe.c



$(OUTPUTDIR)\wsplitpa.OBJ:	..\core\wsplitpa.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wsplitpa.obj ..\core\wsplitpa.c



$(OUTPUTDIR)\wstat.OBJ:	..\core\wstat.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wstat.obj ..\core\wstat.c



$(OUTPUTDIR)\wstrdate.OBJ:	..\core\wstrdate.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wstrdate.obj ..\core\wstrdate.c



$(OUTPUTDIR)\wstrtime.OBJ:	..\core\wstrtime.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wstrtime.obj ..\core\wstrtime.c



$(OUTPUTDIR)\wsystem.OBJ:	..\core\wsystem.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wsystem.obj ..\core\wsystem.c



$(OUTPUTDIR)\wtmpnam.OBJ:	..\core\wtmpnam.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wtmpnam.obj ..\core\wtmpnam.c



$(OUTPUTDIR)\wunlink.OBJ:	..\win32\wunlink.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wunlink.obj ..\win32\wunlink.c



$(OUTPUTDIR)\wutime.OBJ:	..\win32\wutime.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wutime.obj ..\win32\wutime.c



$(OUTPUTDIR)\wwinstar.OBJ:	..\win32\wwinstar.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wwinstar.obj ..\win32\wwinstar.c



$(OUTPUTDIR)\wtoi.OBJ:	..\core32\wtoi.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wtoi.obj ..\core32\wtoi.c



$(OUTPUTDIR)\wtol.OBJ:	..\core32\wtol.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wtol.obj ..\core32\wtol.c



$(OUTPUTDIR)\_wprintf.OBJ:	..\core\_wprintf.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_wprintf.obj ..\core\_wprintf.c



$(OUTPUTDIR)\_wfopen.OBJ:	..\core\_wfopen.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_wfopen.obj ..\core\_wfopen.c


$(SYMROOT).SYM: ..\include\precomp.h
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF$(SYMROOT).SYM -o$(SYMROOT).PCO ..\include\precomp.h



