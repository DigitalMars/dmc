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

PROJ		= sccrt70
APPTYPE		= WIN32 DLL

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

DEFFILE		= sccrt70.DEF
O		= sccrt70

!IF EXIST (sccrt70)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $O\*.* md $O
!ENDIF

TARGETDIR	= ..\..\redist32
!IF EXIST (..\..\redist32)
CREATETARGETDIR	=
!ELSE
CREATETARGETDIR	= if not exist $(TARGETDIR)\*.* md $(TARGETDIR)
!ENDIF

OBJS1 = $O\inttypes.OBJ $O\strtoll.OBJ $O\wcstoll.OBJ $O\wcstoull.OBJ $O\hypot.OBJ $O\typeinfo.obj
POBJS1= $O\inttypes.OBJ+$O\strtoll.OBJ+$O\wcstoll.OBJ+$O\wcstoull.OBJ+$O\hypot.OBJ+$O\typeinfo.obj

SYMROOT		=  sccrt70\precomp 
SYMS		= $(SYMROOT).SYM 
LIBS		= dlluni.LIB KERNEL32.LIB GDI32.LIB USER32.LIB 

!IF $(DEBUG)

CFLAGS		=  -HIprecomp.h -Ae -Ar -r -C -D_WINDLL -NL -5 -Nc -c -H -HDsccrt70 -HO- -wx -g 
DEFINES		= -D_MT -D_MBCS=1
CFLAGS_1	=  -cpp -Ae -Ar -r -C -D_WINDLL -NL -5 -Nc -c -wx -g
DEFINES_1	= -D_MT -D_MBCS
CFLAGS_2	=  -cpp -Ae -Ar -r -C -D_WINDLL -NL -5 -Nc -c -wx -g
DEFINES_2	= -D_MT -D_MBCS
CFLAGS_3	=  -cpp -Ae -Ar -r -C -D_WINDLL -NL -5 -Nc -c -wx -g
DEFINES_3	= -D_MT -D_MBCS
CFLAGS_4	=  -cpp -Ae -Ar -r -C -D_WINDLL -NL -5 -Nc -c -wx -g
DEFINES_4	= -D_MT -D_MBCS
CFLAGS_5	=  -Ae -Ar -Aa -r -C -D_WINDLL -NL -5 -Nc -c -wx -g
DEFINES_5	= -D_MT
CFLAGS_6	=  -Ae -Ar -Aa -r -C -D_WINDLL -NL -5 -Nc -c -wx -g
DEFINES_6	= -D_MT
CFLAGS_7	=  -Ae -Ar -Aa -r -C -D_WINDLL -NL -5 -Nc -c -wx -g
DEFINES_7	= -D_MT
CFLAGS_8	=  -Ae -Ar -r -C -D_WINDLL -f -NL -5 -Nc -c -wx -g
DEFINES_8	= -D_MT -D_MBCS
CFLAGS_9	=  -Ae -Ar -r -C -D_WINDLL -NL -5 -Nc -c -wx -g
DEFINES_9	= -D_MT -D_MBCS -DTRACK_ALLOCS -DDUMP_POOLS
CFLAGS_10	=  -Ae -Ar -r -C -D_WINDLL -NL -5 -Nc -c -wx -g
DEFINES_10	= -D_MT -D_MBCS -DTRACK_ALLOCS -DDUMP_POOLS
CFLAGS_11	=  -Ae -Ar -r -C -D_WINDLL -NL -5 -Nc -c -wx -g
DEFINES_11	= -D_MT -D_MBCS -DTRACK_ALLOCS -DDUMP_POOLS
CFLAGS_12	=  -Ae -Ar -r -C -D_WINDLL -NL -5 -Nc -c -wx -g
DEFINES_12	= -D_MT -D_MBCS -DTRACK_ALLOCS -DDUMP_POOLS
CFLAGS_13	=  -Ae -Ar -r -C -D_WINDLL -NL -5 -Nc -c -wx -g
DEFINES_13	= -D_MT -D_MBCS -DTRACK_ALLOCS -DDUMP_POOLS
CFLAGS_14	=  -Ae -Ar -r -C -D_WINDLL -NL -5 -Nc -c -wx -g
DEFINES_14	= -D_MT -D_MBCS -DTRACK_ALLOCS -DDUMP_POOLS
CFLAGS_15	=  -Ae -Ar -r -C -D_WINDLL -NL -5 -Nc -c -wx -g
DEFINES_15	= -D_MT -D_MBCS -DTRACK_ALLOCS -DDUMP_POOLS
CFLAGS_16	=  -Ae -Ar -r -C -D_WINDLL -NL -5 -Nc -c -wx -g
DEFINES_16	= -D_MT -D_MBCS -DTRACK_ALLOCS -DDUMP_POOLS
CFLAGS_17	=  -Ae -Ar -r -C -D_WINDLL -NL -5 -Nc -c -wx -g
DEFINES_17	= -D_MT -D_MBCS -DTRACK_ALLOCS -DDUMP_POOLS
CFLAGS_18	=  -Ae -Ar -r -C -D_WINDLL -NL -5 -Nc -c -wx -g
DEFINES_18	= -D_MT -D_MBCS -DTRACK_ALLOCS -DDUMP_POOLS
CFLAGS_19	=  -Ae -Ar -r -C -D_WINDLL -NL -5 -Nc -c -wx -g
DEFINES_19	= -D_MT -D_MBCS -DTRACK_ALLOCS -DDUMP_POOLS
CFLAGS_20	=  -Ae -Ar -r -C -D_WINDLL -NL -5 -Nc -c -wx -g
DEFINES_20	= -D_MT -D_MBCS -DTRACK_ALLOCS -DDUMP_POOLS
LFLAGS		=  /CO /NOD /NOI /DO /DE /NOPACKF /XN /NT /ENTRY:__CRT_INIT@12 /VERS:1.0 /BAS:268435456 /A:512 /IMPL:$(TARGETDIR)\$$SCW$$.LIB

!ELSE

CFLAGS		=  -HIprecomp.h -Ae -Ar -o+time -D_WINDLL -NL -5 -Nc -c -H -HDsccrt70 -HO- -wx -w- -w12 
DEFINES		= -D_MT -D_MBCS=1
CFLAGS_1	=  -cpp -Ae -Ar -o+time -D_WINDLL -NL -5 -Nc -c -wx -w- -w12
DEFINES_1	= -D_MT -D_MBCS
CFLAGS_2	=  -cpp -Ae -Ar -o+time -D_WINDLL -NL -5 -Nc -c -wx -w- -w12
DEFINES_2	= -D_MT -D_MBCS
CFLAGS_3	=  -cpp -Ae -Ar -o+time -D_WINDLL -NL -5 -Nc -c -wx -w- -w12
DEFINES_3	= -D_MT -D_MBCS
CFLAGS_4	=  -cpp -Ae -Ar -o+time -D_WINDLL -NL -5 -Nc -c -wx -w- -w12
DEFINES_4	= -D_MT -D_MBCS
CFLAGS_5	=  -Ae -Ar -Aa -o+time -D_WINDLL -NL -5 -Nc -c -wx -w- -w12
DEFINES_5	= -D_MT
CFLAGS_6	=  -Ae -Ar -Aa -o+time -D_WINDLL -NL -5 -Nc -c -wx -w- -w12
DEFINES_6	= -D_MT
CFLAGS_7	=  -Ae -Ar -Aa -o+time -D_WINDLL -NL -5 -Nc -c -wx -w- -w12
DEFINES_7	= -D_MT
CFLAGS_8	=  -Ae -Ar -o+time -D_WINDLL -f -NL -5 -Nc -c -wx -w- -w12
DEFINES_8	= -D_MT -D_MBCS
CFLAGS_9	=  -Ae -Ar -o+time -D_WINDLL -NL -5 -Nc -c -wx -w- -w12
DEFINES_9	= -D_MT -D_MBCS
CFLAGS_10	=  -Ae -Ar -o+time -D_WINDLL -NL -5 -Nc -c -wx -w- -w12
DEFINES_10	= -D_MT -D_MBCS
CFLAGS_11	=  -Ae -Ar -o+time -D_WINDLL -NL -5 -Nc -c -wx -w- -w12
DEFINES_11	= -D_MT -D_MBCS
CFLAGS_12	=  -Ae -Ar -o+time -D_WINDLL -NL -5 -Nc -c -wx -w- -w12
DEFINES_12	= -D_MT -D_MBCS
CFLAGS_13	=  -Ae -Ar -o+time -D_WINDLL -NL -5 -Nc -c -wx -w- -w12
DEFINES_13	= -D_MT -D_MBCS
CFLAGS_14	=  -Ae -Ar -o+time -D_WINDLL -NL -5 -Nc -c -wx -w- -w12
DEFINES_14	= -D_MT -D_MBCS
CFLAGS_15	=  -Ae -Ar -o+time -D_WINDLL -NL -5 -Nc -c -wx -w- -w12
DEFINES_15	= -D_MT -D_MBCS
CFLAGS_16	=  -Ae -Ar -o+time -D_WINDLL -NL -5 -Nc -c -wx -w- -w12
DEFINES_16	= -D_MT -D_MBCS
CFLAGS_17	=  -Ae -Ar -o+time -D_WINDLL -NL -5 -Nc -c -wx -w- -w12
DEFINES_17	= -D_MT -D_MBCS
CFLAGS_18	=  -Ae -Ar -o+time -D_WINDLL -NL -5 -Nc -c -wx -w- -w12
DEFINES_18	= -D_MT -D_MBCS
CFLAGS_19	=  -Ae -Ar -o+time -D_WINDLL -NL -5 -Nc -c -wx -w- -w12
DEFINES_19	= -D_MT -D_MBCS
CFLAGS_20	=  -Ae -Ar -o+time -D_WINDLL -NL -5 -Nc -c -wx -w- -w12
DEFINES_20	= -D_MT -D_MBCS
LFLAGS		=  /L /M :GROUP /X /NOD /NOI /DO /DE /DET /PACKF /XN /NT /ENTRY:__CRT_INIT@12 /VERS:1.0 /BAS:268435456 /A:512 /IMPL:$(TARGETDIR)\$$SCW$$.LIB
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

INCLUDES	= -I..\include -I..\..\include

INCLUDEDOBJS	= 

OBJS		=  $O\CALLOC.OBJ  $O\FREE.OBJ  $O\HEAP32.OBJ  \
		 $O\MALLOC.OBJ  $O\MSIZE.OBJ  $O\PTRCHK.OBJ  $O\REALLOC.OBJ  \
		 $O\SBRK.OBJ  $O\HEAP.OBJ  $O\HEAPINIT.OBJ  $O\MULTPOOL.OBJ  \
		 $O\POOL.OBJ  $O\CRTINIT.OBJ  $O\SEMLOCK.OBJ  $O\_delete.OBJ  $O\_delete2.OBJ  $O\_delete3.OBJ  \
		 $O\_new.OBJ  $O\_new2.OBJ  $O\_new3.OBJ  \
		 $O\adelete.OBJ  $O\adelete2.OBJ  $O\adelete3.OBJ  \
		 $O\nothrow.OBJ  $O\anew.OBJ  \
		 $O\anew2.OBJ  $O\anew3.OBJ  $O\CORENEW.OBJ  $O\_HANDLER.OBJ  $O\CLEARBSS.OBJ  \
		 $O\MEMCPY.OBJ  $O\MEMSET.OBJ  $O\ITOA.OBJ  $O\STRCAT.OBJ  \
		 $O\STRFUNC.OBJ  $O\EXCPTLST.OBJ  $O\EHINIT.OBJ  $O\EHSUP.OBJ  \
		 $O\EHSUP2.OBJ  $O\EHSUP3.OBJ  $O\except.OBJ  $O\rtti.OBJ  \
		 $O\UNMANGLE.OBJ  $O\strdup.OBJ  $O\CTYPE.OBJ  $O\PRINTF.OBJ  \
		 $O\SIGNAL.OBJ  $O\FATEXIT.OBJ  $O\FLTUSED.OBJ  $O\FPSIG.OBJ  \
		 $O\fmt.OBJ  $O\FATERR.OBJ  $O\LLMATH.OBJ  $O\SETERRNO.OBJ  \
		 $O\SETNTERR.OBJ  $O\_EXIT.OBJ  $O\CINIT.OBJ  \
		 $O\SETENVP.OBJ  $O\ATOF.OBJ  $O\STRTOD.OBJ  $O\TOLOWER.OBJ  \
		 $O\FLOATCVT.OBJ  $O\DCLASS.OBJ  $O\_8087.OBJ  $O\FLT87.OBJ  \
		 $O\LDEXP.OBJ  $O\MEMICMP.OBJ  $O\FLTENV.OBJ  $O\DOUBLE.OBJ  \
		 $O\FDIVP.OBJ  $O\PATCH32.OBJ  $O\_ptrchk.OBJ  $O\EXIT.OBJ  \
		 $O\thread.OBJ  $O\threadex.OBJ  $O\CRITICAL.OBJ  $O\THSUP.OBJ  \
		 $O\nlsdata.OBJ  $O\ISCTYPE.OBJ  $O\WCSLEN.OBJ  $O\WCTOMB.OBJ  \
		 $O\WFMT.OBJ  $O\decdata.OBJ  $O\MBTOWC.OBJ  $O\WCSTOD.OBJ  \
		 $O\wcstombs.OBJ  $O\setchars.OBJ  $O\WFLOATCV.OBJ  $O\MBSTOWCS.OBJ  \
		 $O\WPRINTF.OBJ  $O\iswctype.OBJ  $O\lcapi32.OBJ  $O\ehdata.OBJ  \
		 $O\STRNCPY.OBJ  $O\fplock.OBJ  $O\IOB.OBJ  $O\FCLOSE.OBJ  \
		 $O\io.OBJ  $O\FFLUSH.OBJ  $O\write.OBJ  $O\chkstk.OBJ  \
		 $O\flushall.OBJ  $O\isatty.OBJ  $O\umask.OBJ  $O\flush.OBJ  \
		 $O\read.OBJ  $O\fputc.OBJ  $O\fputwc.OBJ  $O\FLUSHWBU.OBJ  \
		 $O\scver.OBJ  $O\ismbdigi.OBJ  $O\initmbcp.OBJ  $O\setmbcp.OBJ  \
		 $O\mbcpinfo.OBJ  $O\_fopen.OBJ  $O\fopen.OBJ  $O\LOCKING.OBJ  \
		 $O\OSFHAND.OBJ  $O\_DUP.OBJ  $O\DOSCOMP.OBJ  $O\DOSCREAT.OBJ  \
		 $O\DOSFIL.OBJ  $O\DOSLOCK.OBJ  $O\DOSSEEK.OBJ  $O\DUP.OBJ  \
		 $O\STAT.OBJ  $O\GETDOS.OBJ  $O\GETCWD.OBJ  $O\MBTOUPPE.OBJ  \
		 $O\MBSICMP.OBJ  $O\MBSPBRK.OBJ  $O\MBSRCHR.OBJ  $O\TIMESUP.OBJ  \
		 $O\STRCMPL.OBJ  $O\STRPBRK.OBJ  $O\STRRCHR.OBJ  $O\_TZSET.OBJ  \
		 $O\LOCTIME.OBJ  $O\CONIO.OBJ  $O\ATOI.OBJ $O\ultoa.obj  $O\GETENV.OBJ  \
		 $O\TOUPPER.OBJ  $O\ATOL.OBJ  $O\STRNICMP.OBJ  $O\BUILDENV.OBJ  \
		 $O\CHSIZE.OBJ  $O\putenv.OBJ  $O\towlower.OBJ  $O\MBSCHR.OBJ  \
		 $O\MBSNBICM.OBJ  $O\w32fater.OBJ  $O\putch.OBJ  $O\cputs.OBJ  \
		 $O\cgets.OBJ  $O\ftime.OBJ  $O\time.OBJ  $O\argcargv.OBJ \
		 $O\strtold.OBJ  $O\strtof.OBJ  $O\_alloca.OBJ $O\ldclass.OBJ \
		 $(OBJS1)

RCFILES		= 

RESFILES	= 

HELPFILES	= 

PROJS		= dlluni.PRJ 

BATS		= 

PRECOMPILE	=  dlluni?PPRJ 

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
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -osccrt70\$(*B).sym $*.h

.HPP.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -osccrt70\$(*B).sym $*.hpp

.HXX.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -osccrt70\$(*B).sym $*.hxx

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



all:	noteout createdir $(PRECOMPILE) $(SYMS) $(OBJS) $(INCLUDEDOBJS) $(POSTCOMPILE) $(TARGETDIR)\$(PROJ).DLL $(POSTLINK) _done


all2:	createdir $(PRECOMPILE) $(SYMS) $(OBJS) $(INCLUDEDOBJS) $(POSTCOMPILE) $(TARGETDIR)\$(PROJ).DLL $(POSTLINK) _done

noteout:
	REM Output to $O

createdir:
	$(CREATEOUTPUTDIR)
	$(CREATETARGETDIR)
	
$(TARGETDIR)\$(PROJ).DLL: $(SYMS) $(OBJS) $(INCLUDEDOBJS) $(RCFILES) $(RESFILES) $(HELPFILES) $(DEFFILE)
			-del $(TARGETDIR)\$(PROJ).DLL
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
sccrt70\precomp.PCO+
sccrt70\CALLOC.OBJ+
sccrt70\FREE.OBJ+
sccrt70\HEAP32.OBJ+
sccrt70\MALLOC.OBJ+
sccrt70\MSIZE.OBJ+
sccrt70\PTRCHK.OBJ+
sccrt70\REALLOC.OBJ+
sccrt70\SBRK.OBJ+
sccrt70\HEAP.OBJ+
sccrt70\HEAPINIT.OBJ+
sccrt70\MULTPOOL.OBJ+
sccrt70\POOL.OBJ+
sccrt70\CRTINIT.OBJ+
sccrt70\SEMLOCK.OBJ+
sccrt70\_delete.OBJ+
sccrt70\_delete2.OBJ+
sccrt70\_delete3.OBJ+
sccrt70\_new.OBJ+
sccrt70\_new2.OBJ+
sccrt70\_new3.OBJ+
sccrt70\adelete.OBJ+
sccrt70\adelete2.OBJ+
sccrt70\adelete3.OBJ+
sccrt70\nothrow.OBJ+
sccrt70\anew.OBJ+
sccrt70\anew2.OBJ+
sccrt70\anew3.OBJ+
sccrt70\CORENEW.OBJ+
sccrt70\_HANDLER.OBJ+
sccrt70\CLEARBSS.OBJ+
sccrt70\MEMCPY.OBJ+
sccrt70\MEMSET.OBJ+
sccrt70\ITOA.OBJ+
sccrt70\STRCAT.OBJ+
sccrt70\STRFUNC.OBJ+
sccrt70\EXCPTLST.OBJ+
sccrt70\EHINIT.OBJ+
sccrt70\EHSUP.OBJ+
sccrt70\EHSUP2.OBJ+
sccrt70\EHSUP3.OBJ+
sccrt70\except.OBJ+
sccrt70\rtti.OBJ+
sccrt70\UNMANGLE.OBJ+
sccrt70\strdup.OBJ+
sccrt70\CTYPE.OBJ+
sccrt70\PRINTF.OBJ+
sccrt70\SIGNAL.OBJ+
sccrt70\FATEXIT.OBJ+
sccrt70\FLTUSED.OBJ+
sccrt70\FPSIG.OBJ+
sccrt70\fmt.OBJ+
sccrt70\FATERR.OBJ+
sccrt70\LLMATH.OBJ+
sccrt70\SETERRNO.OBJ+
sccrt70\SETNTERR.OBJ+
sccrt70\_EXIT.OBJ+
sccrt70\CINIT.OBJ+
sccrt70\SETENVP.OBJ+
sccrt70\ATOF.OBJ+
sccrt70\STRTOD.OBJ+
sccrt70\TOLOWER.OBJ+
sccrt70\FLOATCVT.OBJ+
sccrt70\DCLASS.OBJ+
sccrt70\_8087.OBJ+
sccrt70\FLT87.OBJ+
sccrt70\LDEXP.OBJ+
sccrt70\MEMICMP.OBJ+
sccrt70\FLTENV.OBJ+
sccrt70\DOUBLE.OBJ+
sccrt70\FDIVP.OBJ+
sccrt70\PATCH32.OBJ+
sccrt70\_ptrchk.OBJ+
sccrt70\EXIT.OBJ+
sccrt70\thread.OBJ+
sccrt70\threadex.OBJ+
sccrt70\CRITICAL.OBJ+
sccrt70\THSUP.OBJ+
sccrt70\nlsdata.OBJ+
sccrt70\ISCTYPE.OBJ+
sccrt70\WCSLEN.OBJ+
sccrt70\WCTOMB.OBJ+
sccrt70\WFMT.OBJ+
sccrt70\decdata.OBJ+
sccrt70\MBTOWC.OBJ+
sccrt70\WCSTOD.OBJ+
sccrt70\wcstombs.OBJ+
sccrt70\setchars.OBJ+
sccrt70\WFLOATCV.OBJ+
sccrt70\MBSTOWCS.OBJ+
sccrt70\WPRINTF.OBJ+
sccrt70\iswctype.OBJ+
sccrt70\lcapi32.OBJ+
sccrt70\ehdata.OBJ+
sccrt70\STRNCPY.OBJ+
sccrt70\fplock.OBJ+
sccrt70\IOB.OBJ+
sccrt70\FCLOSE.OBJ+
sccrt70\io.OBJ+
sccrt70\FFLUSH.OBJ+
sccrt70\write.OBJ+
sccrt70\chkstk.OBJ+
sccrt70\flushall.OBJ+
sccrt70\isatty.OBJ+
sccrt70\umask.OBJ+
sccrt70\flush.OBJ+
sccrt70\read.OBJ+
sccrt70\fputc.OBJ+
sccrt70\fputwc.OBJ+
sccrt70\FLUSHWBU.OBJ+
sccrt70\scver.OBJ+
sccrt70\ismbdigi.OBJ+
sccrt70\initmbcp.OBJ+
sccrt70\setmbcp.OBJ+
sccrt70\mbcpinfo.OBJ+
sccrt70\_fopen.OBJ+
sccrt70\fopen.OBJ+
sccrt70\LOCKING.OBJ+
sccrt70\OSFHAND.OBJ+
sccrt70\_DUP.OBJ+
sccrt70\DOSCOMP.OBJ+
sccrt70\DOSCREAT.OBJ+
sccrt70\DOSFIL.OBJ+
sccrt70\DOSLOCK.OBJ+
sccrt70\DOSSEEK.OBJ+
sccrt70\DUP.OBJ+
sccrt70\STAT.OBJ+
sccrt70\GETDOS.OBJ+
sccrt70\GETCWD.OBJ+
sccrt70\MBTOUPPE.OBJ+
sccrt70\MBSICMP.OBJ+
sccrt70\MBSPBRK.OBJ+
sccrt70\MBSRCHR.OBJ+
sccrt70\TIMESUP.OBJ+
sccrt70\STRCMPL.OBJ+
sccrt70\STRPBRK.OBJ+
sccrt70\STRRCHR.OBJ+
sccrt70\_TZSET.OBJ+
sccrt70\LOCTIME.OBJ+
sccrt70\CONIO.OBJ+
sccrt70\ATOI.OBJ+
sccrt70\ultoa.OBJ+
sccrt70\GETENV.OBJ+
sccrt70\TOUPPER.OBJ+
sccrt70\ATOL.OBJ+
sccrt70\STRNICMP.OBJ+
sccrt70\BUILDENV.OBJ+
sccrt70\CHSIZE.OBJ+
sccrt70\putenv.OBJ+
sccrt70\towlower.OBJ+
sccrt70\MBSCHR.OBJ+
sccrt70\MBSNBICM.OBJ+
sccrt70\w32fater.OBJ+
sccrt70\putch.OBJ+
sccrt70\cputs.OBJ+
sccrt70\cgets.OBJ+
sccrt70\ftime.OBJ+
sccrt70\time.OBJ+
sccrt70\strtold.OBJ+
sccrt70\strtof.OBJ+
sccrt70\_alloca.OBJ+
sccrt70\ldclass.OBJ+
$(POBJS1)+
sccrt70\argcargv.OBJ
..\..\redist32\$$SCW$$.DLL
NUL
dlluni.LIB KERNEL32.LIB GDI32.LIB USER32.LIB 
sccrt70.DEF;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
sccrt70\precomp.PCO+
sccrt70\CALLOC.OBJ+
sccrt70\FREE.OBJ+
sccrt70\HEAP32.OBJ+
sccrt70\MALLOC.OBJ+
sccrt70\MSIZE.OBJ+
sccrt70\PTRCHK.OBJ+
sccrt70\REALLOC.OBJ+
sccrt70\SBRK.OBJ+
sccrt70\HEAP.OBJ+
sccrt70\HEAPINIT.OBJ+
sccrt70\MULTPOOL.OBJ+
sccrt70\POOL.OBJ+
sccrt70\CRTINIT.OBJ+
sccrt70\SEMLOCK.OBJ+
sccrt70\_delete.OBJ+
sccrt70\_delete2.OBJ+
sccrt70\_delete3.OBJ+
sccrt70\_new.OBJ+
sccrt70\_new2.OBJ+
sccrt70\_new3.OBJ+
sccrt70\adelete.OBJ+
sccrt70\adelete2.OBJ+
sccrt70\adelete3.OBJ+
sccrt70\nothrow.OBJ+
sccrt70\anew.OBJ+
sccrt70\anew2.OBJ+
sccrt70\anew3.OBJ+
sccrt70\CORENEW.OBJ+
sccrt70\_HANDLER.OBJ+
sccrt70\CLEARBSS.OBJ+
sccrt70\MEMCPY.OBJ+
sccrt70\MEMSET.OBJ+
sccrt70\ITOA.OBJ+
sccrt70\STRCAT.OBJ+
sccrt70\STRFUNC.OBJ+
sccrt70\EXCPTLST.OBJ+
sccrt70\EHINIT.OBJ+
sccrt70\EHSUP.OBJ+
sccrt70\EHSUP2.OBJ+
sccrt70\EHSUP3.OBJ+
sccrt70\except.OBJ+
sccrt70\rtti.OBJ+
sccrt70\UNMANGLE.OBJ+
sccrt70\strdup.OBJ+
sccrt70\CTYPE.OBJ+
sccrt70\PRINTF.OBJ+
sccrt70\SIGNAL.OBJ+
sccrt70\FATEXIT.OBJ+
sccrt70\FLTUSED.OBJ+
sccrt70\FPSIG.OBJ+
sccrt70\fmt.OBJ+
sccrt70\FATERR.OBJ+
sccrt70\LLMATH.OBJ+
sccrt70\SETERRNO.OBJ+
sccrt70\SETNTERR.OBJ+
sccrt70\_EXIT.OBJ+
sccrt70\CINIT.OBJ+
sccrt70\SETENVP.OBJ+
sccrt70\ATOF.OBJ+
sccrt70\STRTOD.OBJ+
sccrt70\TOLOWER.OBJ+
sccrt70\FLOATCVT.OBJ+
sccrt70\DCLASS.OBJ+
sccrt70\_8087.OBJ+
sccrt70\FLT87.OBJ+
sccrt70\LDEXP.OBJ+
sccrt70\MEMICMP.OBJ+
sccrt70\FLTENV.OBJ+
sccrt70\DOUBLE.OBJ+
sccrt70\FDIVP.OBJ+
sccrt70\PATCH32.OBJ+
sccrt70\_ptrchk.OBJ+
sccrt70\EXIT.OBJ+
sccrt70\thread.OBJ+
sccrt70\threadex.OBJ+
sccrt70\CRITICAL.OBJ+
sccrt70\THSUP.OBJ+
sccrt70\nlsdata.OBJ+
sccrt70\ISCTYPE.OBJ+
sccrt70\WCSLEN.OBJ+
sccrt70\WCTOMB.OBJ+
sccrt70\WFMT.OBJ+
sccrt70\decdata.OBJ+
sccrt70\MBTOWC.OBJ+
sccrt70\WCSTOD.OBJ+
sccrt70\wcstombs.OBJ+
sccrt70\setchars.OBJ+
sccrt70\WFLOATCV.OBJ+
sccrt70\MBSTOWCS.OBJ+
sccrt70\WPRINTF.OBJ+
sccrt70\iswctype.OBJ+
sccrt70\lcapi32.OBJ+
sccrt70\ehdata.OBJ+
sccrt70\STRNCPY.OBJ+
sccrt70\fplock.OBJ+
sccrt70\IOB.OBJ+
sccrt70\FCLOSE.OBJ+
sccrt70\io.OBJ+
sccrt70\FFLUSH.OBJ+
sccrt70\write.OBJ+
sccrt70\chkstk.OBJ+
sccrt70\flushall.OBJ+
sccrt70\isatty.OBJ+
sccrt70\umask.OBJ+
sccrt70\flush.OBJ+
sccrt70\read.OBJ+
sccrt70\fputc.OBJ+
sccrt70\fputwc.OBJ+
sccrt70\FLUSHWBU.OBJ+
sccrt70\scver.OBJ+
sccrt70\ismbdigi.OBJ+
sccrt70\initmbcp.OBJ+
sccrt70\setmbcp.OBJ+
sccrt70\mbcpinfo.OBJ+
sccrt70\_fopen.OBJ+
sccrt70\fopen.OBJ+
sccrt70\LOCKING.OBJ+
sccrt70\OSFHAND.OBJ+
sccrt70\_DUP.OBJ+
sccrt70\DOSCOMP.OBJ+
sccrt70\DOSCREAT.OBJ+
sccrt70\DOSFIL.OBJ+
sccrt70\DOSLOCK.OBJ+
sccrt70\DOSSEEK.OBJ+
sccrt70\DUP.OBJ+
sccrt70\STAT.OBJ+
sccrt70\GETDOS.OBJ+
sccrt70\GETCWD.OBJ+
sccrt70\MBTOUPPE.OBJ+
sccrt70\MBSICMP.OBJ+
sccrt70\MBSPBRK.OBJ+
sccrt70\MBSRCHR.OBJ+
sccrt70\TIMESUP.OBJ+
sccrt70\STRCMPL.OBJ+
sccrt70\STRPBRK.OBJ+
sccrt70\STRRCHR.OBJ+
sccrt70\_TZSET.OBJ+
sccrt70\LOCTIME.OBJ+
sccrt70\CONIO.OBJ+
sccrt70\ATOI.OBJ+
sccrt70\ultoa.OBJ+
sccrt70\GETENV.OBJ+
sccrt70\TOUPPER.OBJ+
sccrt70\ATOL.OBJ+
sccrt70\STRNICMP.OBJ+
sccrt70\BUILDENV.OBJ+
sccrt70\CHSIZE.OBJ+
sccrt70\putenv.OBJ+
sccrt70\towlower.OBJ+
sccrt70\MBSCHR.OBJ+
sccrt70\MBSNBICM.OBJ+
sccrt70\w32fater.OBJ+
sccrt70\putch.OBJ+
sccrt70\cputs.OBJ+
sccrt70\cgets.OBJ+
sccrt70\ftime.OBJ+
sccrt70\time.OBJ+
sccrt70\strtold.OBJ+
sccrt70\strtof.OBJ+
sccrt70\_alloca.OBJ+
sccrt70\ldclass.OBJ+
$(POBJS1)+
sccrt70\argcargv.OBJ
..\..\redist32\$$SCW$$.DLL
sccrt70.MAP
dlluni.LIB KERNEL32.LIB GDI32.LIB USER32.LIB 
sccrt70.DEF;
<<
!ENDIF

			-ren $(TARGETDIR)\$$SCW$$.DLL $(PROJ).DLL
			-del $(TARGETDIR)\$(PROJ).LIB
			-ren $(TARGETDIR)\$$SCW$$.LIB $(PROJ).LIB
			-echo $(TARGETDIR)\$(PROJ).DLL built

_done:
		REM  Project is up to date

buildall:	clean	all


clean:
		-del $(TARGETDIR)\$$SCW$$.DLL
		-del $(PROJ).CLE
		-del $O\SCPH.SYM
		-del sccrt70.dpd
		-del $(OBJS)
		-del $(SYMROOT).SYM
		-del $(SYMROOT).PCO
		$(MAKE) -fdlluni.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@

cleanres:

res:		cleanres $(RCFILES) all



dlluni?PPRJ:
		$(MAKE) -fdlluni.mak "SUB_DEBUG=$(DEBUG)"





!IF EXIST (sccrt70.dpd)
!INCLUDE sccrt70.dpd
!ENDIF



$O\CALLOC.OBJ:	..\heap32\CALLOC.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\CALLOC.obj ..\heap32\CALLOC.C



$O\FREE.OBJ:	..\heap32\FREE.C
		$(CC) $(CFLAGS_1) $(DEFINES_1) $(INCLUDES) -o$O\FREE.obj ..\heap32\FREE.C



$O\HEAP32.OBJ:	..\heap32\HEAP32.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\HEAP32.obj ..\heap32\HEAP32.C



$O\MALLOC.OBJ:	..\heap32\MALLOC.C
		$(CC) $(CFLAGS_2) $(DEFINES_2) $(INCLUDES) -o$O\MALLOC.obj ..\heap32\MALLOC.C



$O\MSIZE.OBJ:	..\heap32\MSIZE.C
		$(CC) $(CFLAGS_3) $(DEFINES_3) $(INCLUDES) -o$O\MSIZE.obj ..\heap32\MSIZE.C



$O\PTRCHK.OBJ:	..\heap32\PTRCHK.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\PTRCHK.obj ..\heap32\PTRCHK.C



$O\REALLOC.OBJ:	..\heap32\REALLOC.C
		$(CC) $(CFLAGS_4) $(DEFINES_4) $(INCLUDES) -o$O\REALLOC.obj ..\heap32\REALLOC.C



$O\SBRK.OBJ:	..\heap32\SBRK.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\SBRK.obj ..\heap32\SBRK.C



$O\HEAP.OBJ:	..\heap32\HEAP.CPP
		$(CC) $(CFLAGS_16) $(DEFINES_16) $(INCLUDES) -o$O\HEAP.obj ..\heap32\HEAP.CPP



$O\HEAPINIT.OBJ:	..\heap32\HEAPINIT.CPP
		$(CC) $(CFLAGS_17) $(DEFINES_17) $(INCLUDES) -o$O\HEAPINIT.obj ..\heap32\HEAPINIT.CPP



$O\MULTPOOL.OBJ:	..\heap32\MULTPOOL.CPP
		$(CC) $(CFLAGS_18) $(DEFINES_18) $(INCLUDES) -o$O\MULTPOOL.obj ..\heap32\MULTPOOL.CPP



$O\POOL.OBJ:	..\heap32\POOL.CPP
		$(CC) $(CFLAGS_19) $(DEFINES_19) $(INCLUDES) -o$O\POOL.obj ..\heap32\POOL.CPP



$O\CRTINIT.OBJ:	..\win32\CRTINIT.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\CRTINIT.obj ..\win32\CRTINIT.C



$O\SEMLOCK.OBJ:	..\win32\SEMLOCK.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\SEMLOCK.obj ..\win32\SEMLOCK.C



$O\_delete.OBJ:	..\core\_delete.CPP
		$(CC) $(CFLAGS_9) $(DEFINES_9) $(INCLUDES) -o$O\_delete.obj ..\core\_delete.CPP



$O\_delete2.OBJ:	..\core\_delete2.CPP
		$(CC) $(CFLAGS_9) $(DEFINES_9) $(INCLUDES) -o$O\_delete2.obj ..\core\_delete2.CPP



$O\_delete3.OBJ:	..\core\_delete3.CPP
		$(CC) $(CFLAGS_9) $(DEFINES_9) $(INCLUDES) -o$O\_delete3.obj ..\core\_delete3.CPP



$O\_new.OBJ:	..\core\_new.CPP
		$(CC) $(CFLAGS_11) $(DEFINES_11) $(INCLUDES) -o$O\_new.obj ..\core\_new.CPP



$O\_new2.OBJ:	..\core\_new2.CPP
		$(CC) $(CFLAGS_12) $(DEFINES_12) $(INCLUDES) -o$O\_new2.obj ..\core\_new2.CPP



$O\_new3.OBJ:	..\core\_new3.CPP
		$(CC) $(CFLAGS_12) $(DEFINES_12) $(INCLUDES) -o$O\_new3.obj ..\core\_new3.CPP



$O\adelete.OBJ:	..\core\adelete.CPP
		$(CC) $(CFLAGS_7) $(DEFINES_7) $(INCLUDES) -o$O\adelete.obj ..\core\adelete.CPP



$O\adelete2.OBJ:	..\core\adelete2.CPP
		$(CC) $(CFLAGS_7) $(DEFINES_7) $(INCLUDES) -o$O\adelete2.obj ..\core\adelete2.CPP



$O\adelete3.OBJ:	..\core\adelete3.CPP
		$(CC) $(CFLAGS_7) $(DEFINES_7) $(INCLUDES) -o$O\adelete3.obj ..\core\adelete3.CPP



$O\nothrow.OBJ:	..\core\nothrow.CPP
		$(CC) $(CFLAGS_7) $(DEFINES_7) $(INCLUDES) -o$O\nothrow.obj ..\core\nothrow.CPP



$O\anew.OBJ:	..\core\anew.CPP
		$(CC) $(CFLAGS_6) $(DEFINES_6) $(INCLUDES) -o$O\anew.obj ..\core\anew.CPP



$O\anew2.OBJ:	..\core\anew2.CPP
		$(CC) $(CFLAGS_5) $(DEFINES_5) $(INCLUDES) -o$O\anew2.obj ..\core\anew2.CPP



$O\anew3.OBJ:	..\core\anew3.CPP
		$(CC) $(CFLAGS_5) $(DEFINES_5) $(INCLUDES) -o$O\anew3.obj ..\core\anew3.CPP



$O\CORENEW.OBJ:	..\core\CORENEW.CPP
		$(CC) $(CFLAGS_13) $(DEFINES_13) $(INCLUDES) -o$O\CORENEW.obj ..\core\CORENEW.CPP



$O\_HANDLER.OBJ:	..\core\_HANDLER.CPP
		$(CC) $(CFLAGS_10) $(DEFINES_10) $(INCLUDES) -o$O\_HANDLER.obj ..\core\_HANDLER.CPP



$O\CLEARBSS.OBJ:	..\win32\CLEARBSS.ASM
		$(CC) $(CFLAGS) $(DEFINES) -I..\include -o$O\CLEARBSS.obj ..\win32\CLEARBSS.ASM



$O\MEMCPY.OBJ:	..\core32\MEMCPY.ASM
		$(CC) $(CFLAGS) $(DEFINES) -I..\include -o$O\MEMCPY.obj ..\core32\MEMCPY.ASM



$O\MEMSET.OBJ:	..\core32\MEMSET.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MEMSET.obj ..\core32\MEMSET.C



$O\ITOA.OBJ:	..\core32\ITOA.ASM
		$(CC) $(CFLAGS) $(DEFINES) -I..\include -o$O\ITOA.obj ..\core32\ITOA.ASM



$O\STRCAT.OBJ:	..\core32\STRCAT.ASM
		$(CC) $(CFLAGS) $(DEFINES) -I..\include -o$O\STRCAT.obj ..\core32\STRCAT.ASM



$O\STRFUNC.OBJ:	..\core32\STRFUNC.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\STRFUNC.obj ..\core32\STRFUNC.C



$O\EXCPTLST.OBJ:	..\win32\EXCPTLST.ASM
		$(CC) $(CFLAGS) $(DEFINES) -I..\include -o$O\EXCPTLST.obj ..\win32\EXCPTLST.ASM



$O\EHINIT.OBJ:	..\win32\EHINIT.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\EHINIT.obj ..\win32\EHINIT.C



$O\EHSUP.OBJ:	..\win32\EHSUP.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\EHSUP.obj ..\win32\EHSUP.C



$O\EHSUP2.OBJ:	..\win32\EHSUP2.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\EHSUP2.obj ..\win32\EHSUP2.C



$O\EHSUP3.OBJ:	..\win32\EHSUP3.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\EHSUP3.obj ..\win32\EHSUP3.C



$O\except.OBJ:	..\core\except.CPP
		$(CC) $(CFLAGS_15) $(DEFINES_15) $(INCLUDES) -o$O\except.obj ..\core\except.CPP



$O\typeinfo.OBJ:	..\core\typeinfo.CPP
		$(CC) $(CFLAGS_15) $(DEFINES_15) $(INCLUDES) -o$O\typeinfo.obj ..\core\typeinfo.CPP



$O\rtti.OBJ:	..\core\rtti.cpp
		$(CC) $(CFLAGS_20) $(DEFINES_20) $(INCLUDES) -o$O\rtti.obj ..\core\rtti.cpp



$O\UNMANGLE.OBJ:	..\core\UNMANGLE.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\UNMANGLE.obj ..\core\UNMANGLE.C



$O\strdup.OBJ:	..\core\strdup.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strdup.obj ..\core\strdup.c



$O\CTYPE.OBJ:	..\core\CTYPE.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\CTYPE.obj ..\core\CTYPE.C



$O\PRINTF.OBJ:	..\core\PRINTF.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\PRINTF.obj ..\core\PRINTF.C



$O\SIGNAL.OBJ:	..\core\SIGNAL.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\SIGNAL.obj ..\core\SIGNAL.C



$O\FATEXIT.OBJ:	..\core\FATEXIT.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\FATEXIT.obj ..\core\FATEXIT.C



$O\FLTUSED.OBJ:	..\core\FLTUSED.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\FLTUSED.obj ..\core\FLTUSED.C



$O\FPSIG.OBJ:	..\core\FPSIG.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\FPSIG.obj ..\core\FPSIG.C



$O\fmt.OBJ:	..\core\fmt.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fmt.obj ..\core\fmt.c



$O\FATERR.OBJ:	..\core32\FATERR.ASM
		$(CC) $(CFLAGS) $(DEFINES) -I..\include -o$O\FATERR.obj ..\core32\FATERR.ASM



$O\LLMATH.OBJ:	..\core32\LLMATH.ASM
		$(CC) $(CFLAGS) $(DEFINES) -I..\include -o$O\LLMATH.obj ..\core32\LLMATH.ASM



$O\SETERRNO.OBJ:	..\core\SETERRNO.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\SETERRNO.obj ..\core\SETERRNO.C



$O\SETNTERR.OBJ:	..\win32\SETNTERR.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\SETNTERR.obj ..\win32\SETNTERR.C



$O\_exit.obj:	..\core32\_exit.c
		$(CC) $(CFLAGS_5) $(DEFINES) -I..\include -o$O\_exit.obj ..\core32\_exit.c



$O\_exit2.obj:	..\core32\_exit2.c
		$(CC) $(CFLAGS_5) $(DEFINES) -I..\include -o$O\_exit2.obj ..\core32\_exit2.c



$O\CINIT.OBJ:	..\core32\CINIT.ASM
		$(CC) $(CFLAGS) $(DEFINES) -I..\include -o$O\CINIT.obj ..\core32\CINIT.ASM



$O\SETENVP.OBJ:	..\core\SETENVP.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\SETENVP.obj ..\core\SETENVP.C



$O\ATOF.OBJ:	..\core\ATOF.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ATOF.obj ..\core\ATOF.C



$O\STRTOD.OBJ:	..\core\STRTOD.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\STRTOD.obj ..\core\STRTOD.C



$O\TOLOWER.OBJ:	..\core\TOLOWER.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\TOLOWER.obj ..\core\TOLOWER.C



$O\FLOATCVT.OBJ:	..\core\FLOATCVT.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\FLOATCVT.obj ..\core\FLOATCVT.C



$O\DCLASS.OBJ:	..\core32\DCLASS.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\DCLASS.obj ..\core32\DCLASS.C



$O\_8087.OBJ:	..\win32\_8087.ASM
		$(CC) $(CFLAGS) $(DEFINES) -I..\include -o$O\_8087.obj ..\win32\_8087.ASM



$O\FLT87.OBJ:	..\core32\FLT87.ASM
		$(CC) $(CFLAGS) $(DEFINES) -I..\include -o$O\FLT87.obj ..\core32\FLT87.ASM



$O\LDEXP.OBJ:	..\core32\LDEXP.ASM
		$(CC) $(CFLAGS) $(DEFINES) -I..\include -o$O\LDEXP.obj ..\core32\LDEXP.ASM



$O\MEMICMP.OBJ:	..\core32\MEMICMP.ASM
		$(CC) $(CFLAGS) $(DEFINES) -I..\include -o$O\MEMICMP.obj ..\core32\MEMICMP.ASM



$O\FLTENV.OBJ:	..\core\FLTENV.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\FLTENV.obj ..\core\FLTENV.C



$O\DOUBLE.OBJ:	..\core32\DOUBLE.ASM
		$(CC) $(CFLAGS) $(DEFINES) -I..\include -o$O\DOUBLE.obj ..\core32\DOUBLE.ASM



$O\FDIVP.OBJ:	..\core\FDIVP.C
		$(CC) $(CFLAGS_8) $(DEFINES_8) $(INCLUDES) -o$O\FDIVP.obj ..\core\FDIVP.C



$O\PATCH32.OBJ:	..\core32\PATCH32.ASM
		$(CC) $(CFLAGS) $(DEFINES) -I..\include -o$O\PATCH32.obj ..\core32\PATCH32.ASM



$O\_ptrchk.OBJ:	..\core32\_ptrchk.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_ptrchk.obj ..\core32\_ptrchk.c



$O\EXIT.OBJ:	..\CORE\EXIT.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\EXIT.obj ..\CORE\EXIT.C



$O\thread.OBJ:	..\win32\thread.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\thread.obj ..\win32\thread.c



$O\threadex.OBJ:	..\win32\threadex.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\threadex.obj ..\win32\threadex.c



$O\CRITICAL.OBJ:	..\win32\CRITICAL.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\CRITICAL.obj ..\win32\CRITICAL.C



$O\THSUP.OBJ:	..\win32\THSUP.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\THSUP.obj ..\win32\THSUP.C



$O\nlsdata.OBJ:	..\core\nlsdata.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\nlsdata.obj ..\core\nlsdata.c



$O\ISCTYPE.OBJ:	..\core\ISCTYPE.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ISCTYPE.obj ..\core\ISCTYPE.C



$O\WCSLEN.OBJ:	..\core\WCSLEN.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\WCSLEN.obj ..\core\WCSLEN.C



$O\WCTOMB.OBJ:	..\core\WCTOMB.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\WCTOMB.obj ..\core\WCTOMB.C



$O\WFMT.OBJ:	..\core\WFMT.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\WFMT.obj ..\core\WFMT.C



$O\decdata.OBJ:	..\core\decdata.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\decdata.obj ..\core\decdata.c



$O\MBTOWC.OBJ:	..\core\MBTOWC.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBTOWC.obj ..\core\MBTOWC.C



$O\WCSTOD.OBJ:	..\core\WCSTOD.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\WCSTOD.obj ..\core\WCSTOD.C



$O\wcstombs.OBJ:	..\core\wcstombs.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\wcstombs.obj ..\core\wcstombs.c



$O\setchars.OBJ:	..\core\setchars.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\setchars.obj ..\core\setchars.c



$O\WFLOATCV.OBJ:	..\core\WFLOATCV.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\WFLOATCV.obj ..\core\WFLOATCV.C



$O\MBSTOWCS.OBJ:	..\core\MBSTOWCS.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSTOWCS.obj ..\core\MBSTOWCS.C



$O\WPRINTF.OBJ:	..\core\WPRINTF.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\WPRINTF.obj ..\core\WPRINTF.C



$O\iswctype.OBJ:	..\core\iswctype.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\iswctype.obj ..\core\iswctype.c



$O\lcapi32.OBJ:	..\core\lcapi32.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\lcapi32.obj ..\core\lcapi32.c



$O\ehdata.OBJ:	..\core\ehdata.cpp
		$(CC) $(CFLAGS_14) $(DEFINES_14) $(INCLUDES) -o$O\ehdata.obj ..\core\ehdata.cpp



$O\STRNCPY.OBJ:	..\core32\STRNCPY.ASM
		$(CC) $(CFLAGS) $(DEFINES) -I..\include -o$O\STRNCPY.obj ..\core32\STRNCPY.ASM



$O\fplock.OBJ:	..\win32\fplock.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fplock.obj ..\win32\fplock.c



$O\IOB.OBJ:	..\core\IOB.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\IOB.obj ..\core\IOB.C



$O\FCLOSE.OBJ:	..\core\FCLOSE.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\FCLOSE.obj ..\core\FCLOSE.C



$O\io.OBJ:	..\win32\io.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\io.obj ..\win32\io.c



$O\FFLUSH.OBJ:	..\core\FFLUSH.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\FFLUSH.obj ..\core\FFLUSH.C



$O\write.OBJ:	..\core32\write.asm
		$(CC) $(CFLAGS) $(DEFINES) -I..\include -o$O\write.obj ..\core32\write.asm



$O\chkstk.OBJ:	..\win32\chkstk.asm
		$(CC) $(CFLAGS) $(DEFINES) -I..\include -o$O\chkstk.obj ..\win32\chkstk.asm



$O\flushall.OBJ:	..\core\flushall.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\flushall.obj ..\core\flushall.c



$O\isatty.OBJ:	..\win32\isatty.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\isatty.obj ..\win32\isatty.c



$O\umask.OBJ:	..\core\umask.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\umask.obj ..\core\umask.c



$O\flush.OBJ:	..\core\flush.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\flush.obj ..\core\flush.c



$O\read.OBJ:	..\core32\read.asm
		$(CC) $(CFLAGS) $(DEFINES) -I..\include -o$O\read.obj ..\core32\read.asm



$O\fputc.OBJ:	..\core32\fputc.asm
		$(CC) $(CFLAGS) $(DEFINES) -I..\include -o$O\fputc.obj ..\core32\fputc.asm



$O\fputwc.OBJ:	..\core\fputwc.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fputwc.obj ..\core\fputwc.c



$O\FLUSHWBU.OBJ:	..\core\FLUSHWBU.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\FLUSHWBU.obj ..\core\FLUSHWBU.C



$O\scver.OBJ:	..\core\scver.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\scver.obj ..\core\scver.c



$O\ismbdigi.OBJ:	..\core\ismbdigi.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ismbdigi.obj ..\core\ismbdigi.c



$O\initmbcp.OBJ:	..\core\initmbcp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\initmbcp.obj ..\core\initmbcp.c



$O\setmbcp.OBJ:	..\core\setmbcp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\setmbcp.obj ..\core\setmbcp.c



$O\mbcpinfo.OBJ:	..\core\mbcpinfo.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\mbcpinfo.obj ..\core\mbcpinfo.c



$O\_fopen.OBJ:	..\core\_fopen.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_fopen.obj ..\core\_fopen.c



$O\fopen.OBJ:	..\core\fopen.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fopen.obj ..\core\fopen.c



$O\LOCKING.OBJ:	..\WIN32\LOCKING.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\LOCKING.obj ..\WIN32\LOCKING.C



$O\OSFHAND.OBJ:	..\WIN32\OSFHAND.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\OSFHAND.obj ..\WIN32\OSFHAND.C



$O\_DUP.OBJ:	..\CORE\_DUP.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_DUP.obj ..\CORE\_DUP.C



$O\DOSCOMP.OBJ:	..\CORE\DOSCOMP.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\DOSCOMP.obj ..\CORE\DOSCOMP.C



$O\DOSCREAT.OBJ:	..\WIN32\DOSCREAT.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\DOSCREAT.obj ..\WIN32\DOSCREAT.C



$O\DOSFIL.OBJ:	..\WIN32\DOSFIL.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\DOSFIL.obj ..\WIN32\DOSFIL.C



$O\DOSLOCK.OBJ:	..\CORE\DOSLOCK.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\DOSLOCK.obj ..\CORE\DOSLOCK.C



$O\DOSSEEK.OBJ:	..\CORE\DOSSEEK.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\DOSSEEK.obj ..\CORE\DOSSEEK.C



$O\DUP.OBJ:	..\WIN32\DUP.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\DUP.obj ..\WIN32\DUP.C



$O\STAT.OBJ:	..\CORE\STAT.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\STAT.obj ..\CORE\STAT.C



$O\GETDOS.OBJ:	..\WIN32\GETDOS.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\GETDOS.obj ..\WIN32\GETDOS.C



$O\GETCWD.OBJ:	..\WIN32\GETCWD.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\GETCWD.obj ..\WIN32\GETCWD.C



$O\MBTOUPPE.OBJ:	..\CORE\MBTOUPPE.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBTOUPPE.obj ..\CORE\MBTOUPPE.C



$O\MBSICMP.OBJ:	..\CORE\MBSICMP.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSICMP.obj ..\CORE\MBSICMP.C



$O\MBSPBRK.OBJ:	..\CORE\MBSPBRK.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSPBRK.obj ..\CORE\MBSPBRK.C



$O\MBSRCHR.OBJ:	..\CORE\MBSRCHR.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSRCHR.obj ..\CORE\MBSRCHR.C



$O\TIMESUP.OBJ:	..\CORE\TIMESUP.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\TIMESUP.obj ..\CORE\TIMESUP.C



$O\STRCMPL.OBJ:	..\CORE32\STRCMPL.ASM
		$(CC) $(CFLAGS) $(DEFINES) -I..\include -o$O\STRCMPL.obj ..\CORE32\STRCMPL.ASM



$O\STRPBRK.OBJ:	..\CORE\STRPBRK.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\STRPBRK.obj ..\CORE\STRPBRK.C



$O\STRRCHR.OBJ:	..\CORE\STRRCHR.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\STRRCHR.obj ..\CORE\STRRCHR.C



$O\_TZSET.OBJ:	..\CORE\_TZSET.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_TZSET.obj ..\CORE\_TZSET.C



$O\LOCTIME.OBJ:	..\CORE\LOCTIME.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\LOCTIME.obj ..\CORE\LOCTIME.C



$O\CONIO.OBJ:	..\WIN32\CONIO.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\CONIO.obj ..\WIN32\CONIO.C



$O\ATOI.OBJ:	..\CORE32\ATOI.ASM
		$(CC) $(CFLAGS) $(DEFINES) -I..\include -o$O\ATOI.obj ..\CORE32\ATOI.ASM



$O\ultoa.OBJ:	..\CORE32\ultoa.ASM
		$(CC) $(CFLAGS) $(DEFINES) -I..\include -o$O\ultoa.obj ..\CORE32\ultoa.ASM



$O\GETENV.OBJ:	..\WIN32\GETENV.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\GETENV.obj ..\WIN32\GETENV.C



$O\TOUPPER.OBJ:	..\CORE\TOUPPER.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\TOUPPER.obj ..\CORE\TOUPPER.C



$O\ATOL.OBJ:	..\CORE32\ATOL.ASM
		$(CC) $(CFLAGS) $(DEFINES) -I..\include -o$O\ATOL.obj ..\CORE32\ATOL.ASM



$O\STRNICMP.OBJ:	..\CORE\STRNICMP.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\STRNICMP.obj ..\CORE\STRNICMP.C



$O\BUILDENV.OBJ:	..\CORE\BUILDENV.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\BUILDENV.obj ..\CORE\BUILDENV.C



$O\CHSIZE.OBJ:	..\WIN32\CHSIZE.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\CHSIZE.obj ..\WIN32\CHSIZE.C



$O\putenv.OBJ:	..\core\putenv.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\putenv.obj ..\core\putenv.c



$O\towlower.OBJ:	..\core\towlower.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\towlower.obj ..\core\towlower.c



$O\MBSCHR.OBJ:	..\core\MBSCHR.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSCHR.obj ..\core\MBSCHR.C



$O\MBSNBICM.OBJ:	..\core\MBSNBICM.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSNBICM.obj ..\core\MBSNBICM.C



$O\w32fater.OBJ:	..\win32\w32fater.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\w32fater.obj ..\win32\w32fater.c



$O\putch.OBJ:	..\core\putch.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\putch.obj ..\core\putch.c



$O\cputs.OBJ:	..\core\cputs.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\cputs.obj ..\core\cputs.c



$O\cgets.OBJ:	..\core\cgets.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\cgets.obj ..\core\cgets.c



$O\ftime.OBJ:	..\core\ftime.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ftime.obj ..\core\ftime.c



$O\time.OBJ:	..\core\time.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\time.obj ..\core\time.c



$O\argcargv.OBJ:	..\win32\argcargv.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\argcargv.obj ..\win32\argcargv.c


$O\strtold.OBJ:	..\core\strtold.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strtold.obj ..\core\strtold.C


$O\strtof.OBJ:	..\core\strtof.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strtof.obj ..\core\strtof.C


$O\_alloca.OBJ:	..\core\_alloca.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_alloca.obj ..\core\_alloca.c


$O\ldclass.OBJ:	..\core32\ldclass.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ldclass.obj ..\core32\ldclass.c


$O\regexp.OBJ:	..\core\regexp.cpp
		$(CC) $(CFLAGS_5) -D_MT $(INCLUDES) -o$O\regexp.obj ..\core\regexp.cpp


$O\inttypes.OBJ:	..\core\inttypes.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\inttypes.obj ..\core\inttypes.c


$O\strtoll.OBJ:	..\core\strtoll.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strtoll.obj ..\core\strtoll.c


$O\wcstoll.OBJ:	..\core\strtoll.c
		$(CC) $(CFLAGS_5) $(DEFINES) $(INCLUDES) -D_UNICODE -o$O\wcstoll.obj ..\core\strtoll.c


$O\wcstoull.OBJ: ..\core\wcstoull.c
		$(CC) $(CFLAGS_5) $(DEFINES) $(INCLUDES) -D_UNICODE -o$O\wcstoull.obj ..\core\wcstoull.c


$O\hypot.OBJ:	..\core\hypot.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\hypot.obj ..\core\hypot.c



$(SYMROOT).SYM: ..\include\precomp.h
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF$(SYMROOT).SYM -o$(SYMROOT).PCO ..\include\precomp.h



