
!IFDEF SUB_DEBUG
DEBUG		= $(SUB_DEBUG)
NDEBUG		= !$(SUB_DEBUG)
!ELSE
DEBUG		= 0
NDEBUG		= 1
!ENDIF

PROJ		= snn

CC		= SC
CPP		= SPP
MAKE		= SMAKE
RC		= RCC
HC		= HC31
ASM		= SC

DEFFILE		= snn.def
COMFLAGS	= -NL -wx -w- -w12 -r -Ae -Ar -c
O		= SNN

!IF EXIST (SNN)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $O\*.* md $O
!ENDIF

TARGETDIR	= ..\..\LIB

!IF EXIST (..\..\LIB)
CREATETARGETDIR	=
!ELSE
CREATETARGETDIR	= if not exist $(TARGETDIR)\*.* md $(TARGETDIR)
!ENDIF


SYMROOT		=  SNN\precomp 
SYMS		= $(SYMROOT).SYM 
LIBS		= ..\win32\trace.lib snnuni.lib 

OBJS1 = $O\inttypes.obj $O\strtoll.obj $O\wcstoll.obj $O\wcstoull.obj $O\hypot.obj $O\file.obj $O\u64_ldbl.obj $O\ldblullng.obj \
	$O\wmem.obj $O\math2.obj

DEFINES		= -D_MT -D_MBCS
DEFINES_49	= -D_MT

!IF $(DEBUG)

CFLAGS		=  $(COMFLAGS) -g -HIprecomp.h -H -HDSNN -HO-
CFLAGS_1	=  $(COMFLAGS) -g -cpp
CFLAGS_5	=  $(COMFLAGS) -g -f
CFLAGS_6	=  $(COMFLAGS) -g -Aa
CFLAGS_42	=  $(COMFLAGS) -g

DEFINES_42	=  $(DEFINES) -DTRACK_ALLOCS -DDUMP_POOLS

LFLAGS		=  /CO /NOI /DE /PACKF /XN /A:512

!ELSE

CFLAGS		=  $(COMFLAGS) -Nc -o -HIprecomp.h -H -HDSNN -HO- 
CFLAGS_1	=  $(COMFLAGS) -Nc -o -cpp 
CFLAGS_5	=  $(COMFLAGS) -Nc -o -f
CFLAGS_6	=  $(COMFLAGS) -Nc -o -Aa
CFLAGS_42	=  $(COMFLAGS) -Nc -o

DEFINES_42	=  $(DEFINES)

LFLAGS		=  /NOI /DE /E /PACKF /XN /A:512

!ENDIF

HFLAGS		= $(CFLAGS) 
MFLAGS		= MASTERPROJ=$(PROJ)
LIBFLAGS	=  /C /NOI 
DEBUGGERFLAGS	=  
AFLAGS		= $(CFLAGS) 
HELPFLAGS	= 

MODEL		= N

PAR		= OBJS

INCLUDES	= -I..\include -I..\..\include

INCLUDEDOBJS	=  snn\tlsseg.obj 

OBJS		=  $O\_8087.obj $O\_exit.obj $O\_exit2.obj $O\atoi.obj \
		 $O\atol.obj $O\chkstk.obj $O\cinit.obj $O\clearbss.obj \
		 $O\dbllng.obj $O\double.obj $O\excptlst.obj $O\faterr.obj \
		 $O\floattyp.obj $O\flt87.obj $O\fpatan32.obj $O\fprem32.obj \
		 $O\fptan32.obj $O\fputc.obj $O\ftol.obj \
		 $O\itoa.obj $O\ldexp.obj $O\llmath.obj $O\ltoa.obj \
		 $O\math87.obj $O\mathtrn.obj $O\mathtrnf.obj $O\memchr.obj \
		 $O\memcpy.obj $O\memicmp.obj $O\memint.obj $O\movedata.obj \
		 $O\page.obj $O\patch32.obj $O\port.obj $O\read.obj \
		 $O\rot.obj $O\seg.obj $O\stpcpy.obj \
		 $O\strcat.obj $O\strcmpl.obj $O\strncat.obj $O\strncpy.obj \
		 $O\strtoul.obj $O\ultoa.obj $O\write.obj $O\_dup.obj \
		 $O\_fcvt.obj $O\_fgtchar.obj $O\_fptchar.obj $O\_ismbbld.obj \
		 $O\_ismbbtl.obj $O\_itoa.obj $O\_ltoa.obj $O\_main.obj \
		 $O\_memicmp.obj $O\_printf.obj $O\_strdup.obj $O\_strerro.obj \
		 $O\_strrev.obj $O\_tzset.obj $O\arcerr.obj $O\asin.obj \
		 $O\assert.obj $O\atan.obj $O\atexit.obj $O\atof.obj \
		 $O\buildenv.obj $O\cgets.obj $O\charmax.obj $O\clock.obj \
		 $O\commit.obj $O\conio.obj $O\controlc.obj $O\coreleft.obj \
		 $O\cprintf.obj $O\cputs.obj $O\cputype.obj $O\creat.obj \
		 $O\critical.obj $O\crtinit.obj $O\cscanf.obj $O\ctype.obj \
		 $O\dclass.obj $O\dllopen.obj $O\dllstart.obj $O\dosclose.obj \
		 $O\doscomp.obj $O\doscreat.obj $O\dosdate.obj $O\doserr.obj \
		 $O\dosfil.obj $O\doslock.obj $O\dosopen.obj $O\dosseek.obj \
		 $O\dup.obj $O\ecvt.obj $O\ehinit.obj $O\ehsup.obj \
		 $O\ehsup2.obj $O\ehsup3.obj $O\eof.obj $O\exit.obj \
		 $O\exitstat.obj $O\exp.obj $O\expand.obj $O\fatexit.obj \
		 $O\fclose.obj $O\fcloseal.obj $O\fcvt.obj $O\fdivp.obj \
		 $O\feexcept.obj $O\fflush.obj $O\fgetchar.obj $O\fgets.obj \
		 $O\filelen.obj $O\floatcvt.obj $O\fltenv.obj $O\fltused.obj \
		 $O\flushall.obj $O\fmacros.obj $O\fmbyte.obj $O\fplock.obj \
		 $O\fpsig.obj $O\fputchar.obj $O\fputs.obj $O\fread.obj \
		 $O\ftime.obj $O\fwrite.obj $O\gcvt.obj $O\getchar.obj \
		 $O\getdate.obj $O\getenv.obj $O\getftime.obj $O\getpid.obj \
		 $O\gets.obj $O\gettime.obj $O\hyper.obj $O\ieee.obj \
		 $O\initmbcp.obj $O\iob.obj $O\isalnum.obj $O\isalpha.obj \
		 $O\isascii.obj $O\isatty.obj $O\iscntrl.obj $O\iscsym.obj \
		 $O\iscsymf.obj $O\isdigit.obj $O\isgraph.obj $O\islower.obj \
		 $O\isprint.obj $O\ispunct.obj $O\isspace.obj $O\isupper.obj \
		 $O\isxdigit.obj $O\isblank.obj $O\jmp.obj $O\lconv.obj $O\lfind.obj \
		 $O\locking.obj $O\loctime.obj $O\log.obj $O\lsearch.obj \
		 $O\logb.obj $O\ilogb.obj $O\cbrt.obj $O\erf.obj $O\gamma.obj \
		 $O\matherr.obj $O\mbctype.obj $O\mbstowcs.obj $O\mbstring.obj \
		 $O\mbyte.obj $O\memccpy.obj $O\memcmp.obj $O\memset.obj \
		 $O\okbigbuf.obj $O\onexit.obj $O\osfhand.obj $O\perror.obj \
		 $O\poly.obj $O\pow.obj $O\putch.obj $O\putchar.obj \
		 $O\putenv.obj $O\puts.obj $O\qsort.obj $O\rand.obj \
		 $O\response.obj $O\rmtmp.obj $O\round.obj $O\semlock.obj \
		 $O\setenvp.obj $O\seterrno.obj $O\setjmp.obj $O\setlc.obj \
		 $O\setmbcp.obj $O\setmode.obj $O\setnterr.obj $O\signal.obj \
		 $O\sin.obj $O\sleep.obj $O\sound.obj $O\sqrt.obj \
		 $O\stack.obj $O\stckaval.obj $O\strcspn.obj $O\strdup.obj \
		 $O\strerror.obj $O\strfunc.obj $O\strlwr.obj $O\strnicmp.obj \
		 $O\strnset.obj $O\strpbrk.obj $O\strrchr.obj $O\strrev.obj \
		 $O\strset.obj $O\strspn.obj $O\strstr.obj $O\strtod.obj \
		 $O\strtof.obj $O\strtok.obj $O\strtol.obj $O\strtoull.obj \
		 $O\strupr.obj $O\strxfrm.obj $O\swab.obj $O\tabsize.obj \
		 $O\tan.obj $O\tchar.obj $O\thread.obj $O\threadex.obj \
		 $O\time.obj $O\time3.obj $O\timesup.obj $O\tlsdata.obj \
		 $O\toascii.obj $O\tolower.obj $O\toupper.obj $O\trigerr.obj \
		 $O\umask.obj $O\ungetc.obj $O\unmangle.obj $O\vprintf.obj \
		 $O\w32sver.obj $O\xcfilter.obj $O\_delete.obj $O\_delnear.obj \
		 $O\_handler.obj $O\_new.obj $O\_new2.obj $O\_newnear.obj \
		 $O\_new3.obj $O\_delete2.obj $O\_delete3.obj $O\nothrow.obj \
		 $O\complex.obj $O\coperat.obj $O\cstrm.obj $O\ctrig.obj \
		 $O\dbp.obj $O\except.obj $O\fstream.obj $O\generr.obj \
		 $O\ios.obj $O\iostream.obj $O\istream.obj $O\istreamf.obj \
		 $O\istreami.obj $O\istreamx.obj $O\manip.obj $O\ostream.obj \
		 $O\ostreamf.obj $O\ostreami.obj $O\ostreamx.obj $O\rtti.obj \
		 $O\siosync.obj $O\stdiostr.obj $O\stream.obj $O\streambf.obj \
		 $O\strstrea.obj $O\typeinfo.obj $O\vecnew.obj $O\vector.obj \
		 $O\disp.obj $O\scver.obj $O\alloca.obj $O\anew.obj \
		 $O\anew2.obj $O\anew3.obj $O\adelete.obj $O\adelete2.obj \
		 $O\adelete3.obj $O\corenew.obj $O\purevirt.obj \
		 $O\vedbghk.obj $O\calloc.obj $O\free.obj $O\malloc.obj \
		 $O\msize.obj $O\realloc.obj $O\sbrk.obj $O\heap32.obj \
		 $O\ptrchk.obj $O\heap.obj $O\multpool.obj $O\pool.obj \
		 $O\_alloca.obj $O\heapinit.obj $O\_ptrchk.obj $O\decdata.obj \
		 $O\fgetwc.obj $O\fgetws.obj $O\flushwbu.obj $O\fputwc.obj \
		 $O\fputws.obj $O\getws.obj $O\isctype.obj $O\iswctype.obj \
		 $O\itow.obj $O\lcapi32.obj $O\lccrtdr.obj $O\lccrtfl.obj \
		 $O\LCCRTPC.obj $O\LCDELFL.obj $O\LCFNDFL.obj $O\LCGETCD.obj \
		 $O\LCGETCL.obj $O\LCGETFA.obj $O\LCGETMF.obj $O\LCMOVFL.obj \
		 $O\LCRMVDR.obj $O\LCSETCD.obj $O\LCSETFA.obj $O\LTOW.obj \
		 $O\MBTOWC.obj $O\NLSDATA.obj $O\NULDATA.obj $O\PUTWS.obj \
		 $O\SETCHARS.obj $O\STRCOLL.obj $O\STRICOLL.obj $O\STRNCOLL.obj \
		 $O\STRNICOL.obj $O\TIMDATA.obj $O\TNODATA.obj $O\TOWLOWER.obj \
		 $O\TOWUPPER.obj $O\ULTOW.obj $O\UNGETWC.obj $O\_fopen.obj \
		 $O\flush.obj $O\asctime.obj $O\ctime.obj $O\execl.obj \
		 $O\execle.obj $O\execlp.obj $O\execlpe.obj $O\execv.obj \
		 $O\execve.obj $O\execvp.obj $O\execvpe.obj $O\fdopen.obj \
		 $O\fmt.obj $O\fopen.obj $O\freopen.obj $O\fsopen.obj \
		 $O\disk.obj $O\mktemp.obj $O\printf.obj $O\scanf.obj \
		 $O\sopenf.obj $O\spawnlpe.obj $O\spawn.obj $O\spawnl.obj \
		 $O\spawnle.obj $O\spawnlp.obj $O\spawnv.obj $O\spawnve.obj \
		 $O\spawnvp.obj $O\spawnvpe.obj $O\splitpat.obj $O\stat.obj \
		 $O\strf.obj $O\system.obj $O\tmpnam.obj $O\getdos.obj \
		 $O\chsize.obj $O\constart.obj $O\exec2.obj $O\find.obj \
		 $O\getcwd.obj $O\io.obj $O\setargv.obj $O\unlink.obj \
		 $O\utime.obj $O\winstart.obj $O\thsup.obj $O\ehdata.obj \
		 $O\_ISMBSLD.obj $O\_ISMBSTL.obj $O\ISMBALNU.obj $O\ISMBALPH.obj \
		 $O\ISMBBYTE.obj $O\ISMBCL0.obj $O\ISMBCL1.obj $O\ISMBCL2.obj \
		 $O\ISMBDIGI.obj $O\ISMBGRAP.obj $O\ISMBHIRA.obj $O\ISMBKATA.obj \
		 $O\ISMBLEGA.obj $O\ISMBLOWE.obj $O\ISMBPRIN.obj $O\ISMBPUNC.obj \
		 $O\ISMBSPAC.obj $O\ISMBSYMB.obj $O\ISMBUPPE.obj $O\MBBTOMBC.obj \
		 $O\MBBTYPE.obj $O\MBCCPY.obj $O\MBCLEN.obj $O\MBCPINFO.obj \
		 $O\MBJISJMS.obj $O\MBJMSJIS.obj $O\MBLEN.obj $O\MBSBTYPE.obj \
		 $O\MBSCHR.obj $O\MBSCMP.obj $O\MBSCOLL.obj $O\MBSCSPN.obj \
		 $O\MBSDEC.obj $O\MBSICMP.obj $O\MBSICOLL.obj $O\MBSINC.obj \
		 $O\MBSLEN.obj $O\MBSLWR.obj $O\MBSNBCAT.obj $O\MBSNBCMP.obj \
		 $O\MBSNBCNT.obj $O\MBSNBCOL.obj $O\MBSNBCPY.obj $O\MBSNBICM.obj \
		 $O\MBSNBICO.obj $O\MBSNBSET.obj $O\MBSNCAT.obj $O\MBSNCCNT.obj \
		 $O\MBSNCMP.obj $O\MBSNCOLL.obj $O\MBSNCPY.obj $O\MBSNEXTC.obj \
		 $O\MBSNICMP.obj $O\MBSNICOL.obj $O\MBSNINC.obj $O\MBSNSET.obj \
		 $O\MBSPBRK.obj $O\MBSRCHR.obj $O\MBSREV.obj $O\MBSSET.obj \
		 $O\MBSSPN.obj $O\MBSSPNP.obj $O\MBSSTR.obj $O\MBSTOK.obj \
		 $O\MBSTRLEN.obj $O\MBSUPR.obj $O\MBTOHIRA.obj $O\MBTOKATA.obj \
		 $O\MBTOLOWE.obj $O\MBTOUPPE.obj $O\w32fater.obj \
		 $O\ldclass.obj $O\cmath.obj $O\complex2.obj \
		 $O\strtold.obj $O\ldexpl.obj $O\regexp.obj $O\_find.obj \
		 $O\frexpl.obj $O\fwide.obj $O\thunk.obj $(OBJS1)

PRECOMPILE	=  snnuni?Pprj  trace?Pmak  TLSSEG?PMAK 

POSTCOMPILE	= 

POSTLINK	= 

.SUFFIXES: .C .CP .CPP .CXX .CC .H .HPP .HXX .COM .EXE .DLL .LIB .RTF .DLG .ASM .RES .RC .obj 

.C.obj:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.c

.CPP.obj:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.cpp

.H.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -oSNN\$(*B).sym $*.h

.HPP.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -oSNN\$(*B).sym $*.hpp

.ASM.obj:
	$(ASM) $(AFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.asm


all:	noteout createdir $(PRECOMPILE) $(SYMS) $(OBJS) $(INCLUDEDOBJS) $(POSTCOMPILE) $(TARGETDIR)\$(PROJ).lib $(POSTLINK) _done


all2:	createdir $(PRECOMPILE) $(SYMS) $(OBJS) $(INCLUDEDOBJS) $(POSTCOMPILE) $(TARGETDIR)\$(PROJ).lib $(POSTLINK) _done

noteout:
	REM Output to $O

createdir:
	$(CREATEOUTPUTDIR)
	$(CREATETARGETDIR)
	
$(TARGETDIR)\$(PROJ).lib: $(SYMS) $(OBJS) $(INCLUDEDOBJS)
		lib -c $(TARGETDIR)\$(PROJ).lib \
				@<<
			$(OBJS) $(INCLUDEDOBJS) $(LIBS)
<<


_done:
		REM  Project is up to date

buildall:	clean	all


clean:
		-del $(TARGETDIR)\$$SCW$$.lib
		-del $(TARGETDIR)\$(PROJ).CLE
		-del $O\SCPH.SYM
		-del snn.dpd
		-del $(OBJS)
		-del $(SYMROOT).SYM
		-del $(SYMROOT).PCO
		$(MAKE) -fsnnuni.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ftrace.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@
		$(MAKE) -ftlsseg.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@

cleanres:

res:		cleanres all


snnuni?Pprj:
		$(MAKE) -fsnnuni.mak "SUB_DEBUG=$(DEBUG)"


trace?Pmak:
		$(MAKE) -ftrace.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)"

TLSSEG?PMAK:
		$(MAKE) -fTLSSEG.MAK $(MFLAGS) "SUB_DEBUG=$(DEBUG)"




!IF EXIST (snn.dpd)
!INCLUDE snn.dpd
!ENDIF



$O\_8087.obj:	..\win32\_8087.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_8087.obj ..\win32\_8087.asm


$O\_exit.obj:	..\core32\_exit.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_exit.obj ..\core32\_exit.c


$O\_exit2.obj:	..\core32\_exit2.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_exit2.obj ..\core32\_exit2.c


$O\atoi.obj:	..\core32\atoi.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\atoi.obj ..\core32\atoi.asm


$O\atol.obj:	..\core32\atol.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\atol.obj ..\core32\atol.asm


$O\chkstk.obj:	..\win32\chkstk.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\chkstk.obj ..\win32\chkstk.asm


$O\cinit.obj:	..\core32\cinit.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\cinit.obj ..\core32\cinit.asm


$O\clearbss.obj:	..\win32\clearbss.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\clearbss.obj ..\win32\clearbss.asm


$O\dbllng.obj:	..\core32\dbllng.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\dbllng.obj ..\core32\dbllng.asm


$O\u64_ldbl.obj: ..\core32\u64_ldbl.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\u64_ldbl.obj ..\core32\u64_ldbl.c


$O\ldblullng.obj: ..\core32\ldblullng.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ldblullng.obj ..\core32\ldblullng.c


$O\double.obj:	..\core32\double.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\double.obj ..\core32\double.asm


$O\excptlst.obj:	..\win32\excptlst.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\excptlst.obj ..\win32\excptlst.asm


$O\faterr.obj:	..\core32\faterr.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\faterr.obj ..\core32\faterr.asm


$O\floattyp.obj:	..\core32\floattyp.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\floattyp.obj ..\core32\floattyp.asm


$O\flt87.obj:	..\core32\flt87.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\flt87.obj ..\core32\flt87.asm


$O\fpatan32.obj:	..\core32\fpatan32.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fpatan32.obj ..\core32\fpatan32.asm


$O\fprem32.obj:	..\core32\fprem32.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fprem32.obj ..\core32\fprem32.asm


$O\fptan32.obj:	..\core32\fptan32.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fptan32.obj ..\core32\fptan32.asm


$O\fputc.obj:	..\core32\fputc.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fputc.obj ..\core32\fputc.asm



$O\ftol.obj:	..\win32\ftol.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ftol.obj ..\win32\ftol.asm



$O\itoa.obj:	..\core32\itoa.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\itoa.obj ..\core32\itoa.asm



$O\ldexp.obj:	..\core32\ldexp.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ldexp.obj ..\core32\ldexp.asm



$O\llmath.obj:	..\core32\llmath.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\llmath.obj ..\core32\llmath.asm



$O\ltoa.obj:	..\core32\ltoa.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ltoa.obj ..\core32\ltoa.asm



$O\math87.obj:	..\core32\math87.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\math87.obj ..\core32\math87.asm



$O\mathtrn.obj:	..\core32\mathtrn.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\mathtrn.obj ..\core32\mathtrn.asm



$O\mathtrnf.obj:	..\core32\mathtrnf.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\mathtrnf.obj ..\core32\mathtrnf.asm



$O\memchr.obj:	..\core32\memchr.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\memchr.obj ..\core32\memchr.asm



$O\memcpy.obj:	..\core32\memcpy.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\memcpy.obj ..\core32\memcpy.asm



$O\memicmp.obj:	..\core32\memicmp.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\memicmp.obj ..\core32\memicmp.asm



$O\memint.obj:	..\core32\memint.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\memint.obj ..\core32\memint.asm



$O\movedata.obj:	..\core32\movedata.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\movedata.obj ..\core32\movedata.asm



$O\page.obj:	..\core32\page.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\page.obj ..\core32\page.asm



$O\patch32.obj:	..\core32\patch32.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\patch32.obj ..\core32\patch32.asm



$O\port.obj:	..\core32\port.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\port.obj ..\core32\port.asm



$O\read.obj:	..\core32\read.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\read.obj ..\core32\read.asm



$O\rot.obj:	..\core32\rot.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\rot.obj ..\core32\rot.asm



$O\seg.obj:	..\core32\seg.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\seg.obj ..\core32\seg.asm



$O\stpcpy.obj:	..\core32\stpcpy.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\stpcpy.obj ..\core32\stpcpy.asm



$O\strcat.obj:	..\core32\strcat.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strcat.obj ..\core32\strcat.asm



$O\strcmpl.obj:	..\core32\strcmpl.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strcmpl.obj ..\core32\strcmpl.asm



$O\strncat.obj:	..\core32\strncat.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strncat.obj ..\core32\strncat.asm



$O\strncpy.obj:	..\core32\strncpy.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strncpy.obj ..\core32\strncpy.asm



$O\strtoul.obj:	..\core32\strtoul.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strtoul.obj ..\core32\strtoul.asm



$O\ultoa.obj:	..\core32\ultoa.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ultoa.obj ..\core32\ultoa.asm



$O\wmem.obj:	..\core\wmem.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\wmem.obj ..\core\wmem.c



$O\write.obj:	..\core32\write.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\write.obj ..\core32\write.asm



$O\_dup.obj:	..\core\_dup.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_dup.obj ..\core\_dup.c



$O\_fcvt.obj:	..\core\_fcvt.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_fcvt.obj ..\core\_fcvt.c



$O\_fgtchar.obj:	..\core\_fgtchar.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_fgtchar.obj ..\core\_fgtchar.c



$O\_fptchar.obj:	..\core\_fptchar.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_fptchar.obj ..\core\_fptchar.c



$O\_ismbbld.obj:	..\core\_ismbbld.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_ismbbld.obj ..\core\_ismbbld.c



$O\_ismbbtl.obj:	..\core\_ismbbtl.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_ismbbtl.obj ..\core\_ismbbtl.c



$O\_itoa.obj:	..\core\_itoa.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_itoa.obj ..\core\_itoa.c



$O\_ltoa.obj:	..\core\_ltoa.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_ltoa.obj ..\core\_ltoa.c



$O\_main.obj:	..\core\_main.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_main.obj ..\core\_main.c



$O\_memicmp.obj:	..\core\_memicmp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_memicmp.obj ..\core\_memicmp.c



$O\_printf.obj:	..\core\_printf.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_printf.obj ..\core\_printf.c



$O\_strdup.obj:	..\core\_strdup.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_strdup.obj ..\core\_strdup.c



$O\_strerro.obj:	..\core\_strerro.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_strerro.obj ..\core\_strerro.c



$O\_strrev.obj:	..\core\_strrev.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_strrev.obj ..\core\_strrev.c



$O\_tzset.obj:	..\core\_tzset.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_tzset.obj ..\core\_tzset.c



$O\arcerr.obj:	..\core\arcerr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\arcerr.obj ..\core\arcerr.c



$O\asin.obj:	..\core\asin.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\asin.obj ..\core\asin.c



$O\assert.obj:	..\core\assert.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\assert.obj ..\core\assert.c



$O\atan.obj:	..\core\atan.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\atan.obj ..\core\atan.c



$O\atexit.obj:	..\core\atexit.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\atexit.obj ..\core\atexit.c



$O\atof.obj:	..\core\atof.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\atof.obj ..\core\atof.c



$O\buildenv.obj:	..\core\buildenv.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\buildenv.obj ..\core\buildenv.c



$O\cgets.obj:	..\core\cgets.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\cgets.obj ..\core\cgets.c



$O\charmax.obj:	..\core\charmax.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\charmax.obj ..\core\charmax.c



$O\clock.obj:	..\win32\clock.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\clock.obj ..\win32\clock.c



$O\commit.obj:	..\core\commit.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\commit.obj ..\core\commit.c



$O\conio.obj:	..\win32\conio.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\conio.obj ..\win32\conio.c



$O\controlc.obj:	..\win32\controlc.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\controlc.obj ..\win32\controlc.c



$O\coreleft.obj:	..\win32\coreleft.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\coreleft.obj ..\win32\coreleft.c



$O\cprintf.obj:	..\core\cprintf.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\cprintf.obj ..\core\cprintf.c



$O\cputs.obj:	..\core\cputs.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\cputs.obj ..\core\cputs.c



$O\cputype.obj:	..\core32\cputype.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\cputype.obj ..\core32\cputype.c



$O\creat.obj:	..\core\creat.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\creat.obj ..\core\creat.c



$O\critical.obj:	..\win32\critical.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\critical.obj ..\win32\critical.c



$O\crtinit.obj:	..\win32\crtinit.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\crtinit.obj ..\win32\crtinit.c



$O\cscanf.obj:	..\core\cscanf.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\cscanf.obj ..\core\cscanf.c



$O\ctype.obj:	..\core\ctype.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ctype.obj ..\core\ctype.c



$O\dclass.obj:	..\core32\dclass.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\dclass.obj ..\core32\dclass.c



$O\dllopen.obj:	..\win32\dllopen.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\dllopen.obj ..\win32\dllopen.c



$O\dllstart.obj:	..\win32\dllstart.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\dllstart.obj ..\win32\dllstart.c



$O\dosclose.obj:	..\win32\dosclose.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\dosclose.obj ..\win32\dosclose.c



$O\doscomp.obj:	..\core\doscomp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\doscomp.obj ..\core\doscomp.c



$O\doscreat.obj:	..\win32\doscreat.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\doscreat.obj ..\win32\doscreat.c



$O\dosdate.obj:	..\win32\dosdate.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\dosdate.obj ..\win32\dosdate.c



$O\doserr.obj:	..\win32\doserr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\doserr.obj ..\win32\doserr.c



$O\dosfil.obj:	..\win32\dosfil.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\dosfil.obj ..\win32\dosfil.c



$O\doslock.obj:	..\core\doslock.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\doslock.obj ..\core\doslock.c



$O\dosopen.obj:	..\win32\dosopen.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\dosopen.obj ..\win32\dosopen.c



$O\dosseek.obj:	..\core\dosseek.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\dosseek.obj ..\core\dosseek.c



$O\dup.obj:	..\win32\dup.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\dup.obj ..\win32\dup.c



$O\ecvt.obj:	..\core\ecvt.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ecvt.obj ..\core\ecvt.c



$O\ehinit.obj:	..\win32\ehinit.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ehinit.obj ..\win32\ehinit.c



$O\ehsup.obj:	..\win32\ehsup.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ehsup.obj ..\win32\ehsup.c



$O\ehsup2.obj:	..\win32\ehsup2.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ehsup2.obj ..\win32\ehsup2.c



$O\ehsup3.obj:	..\win32\ehsup3.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ehsup3.obj ..\win32\ehsup3.c



$O\erf.obj:	..\core\erf.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\erf.obj ..\core\erf.c



$O\gamma.obj:	..\core\gamma.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\gamma.obj ..\core\gamma.c



$O\eof.obj:	..\core\eof.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\eof.obj ..\core\eof.c



$O\exit.obj:	..\core\exit.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\exit.obj ..\core\exit.c



$O\exitstat.obj:	..\core\exitstat.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\exitstat.obj ..\core\exitstat.c



$O\exp.obj:	..\core\exp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\exp.obj ..\core\exp.c



$O\expand.obj:	..\win32\expand.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\expand.obj ..\win32\expand.c



$O\fatexit.obj:	..\core\fatexit.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fatexit.obj ..\core\fatexit.c



$O\fclose.obj:	..\core\fclose.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fclose.obj ..\core\fclose.c



$O\fcloseal.obj:	..\core\fcloseal.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fcloseal.obj ..\core\fcloseal.c



$O\fcvt.obj:	..\core\fcvt.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fcvt.obj ..\core\fcvt.c



$O\fdivp.obj:	..\core\fdivp.c
		$(CC) $(CFLAGS_5) $(DEFINES) $(INCLUDES) -o$O\fdivp.obj ..\core\fdivp.c



$O\feexcept.obj:	..\core\feexcept.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\feexcept.obj ..\core\feexcept.c



$O\fflush.obj:	..\core\fflush.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fflush.obj ..\core\fflush.c



$O\fgetchar.obj:	..\core\fgetchar.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fgetchar.obj ..\core\fgetchar.c



$O\fgets.obj:	..\core\fgets.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fgets.obj ..\core\fgets.c



$O\filelen.obj:	..\win32\filelen.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\filelen.obj ..\win32\filelen.c



$O\floatcvt.obj:	..\core\floatcvt.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\floatcvt.obj ..\core\floatcvt.c



$O\fltenv.obj:	..\core\fltenv.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fltenv.obj ..\core\fltenv.c



$O\fltused.obj:	..\core\fltused.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fltused.obj ..\core\fltused.c



$O\flushall.obj:	..\core\flushall.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\flushall.obj ..\core\flushall.c



$O\fmacros.obj:	..\core\fmacros.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fmacros.obj ..\core\fmacros.c



$O\fmbyte.obj:	..\core\fmbyte.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fmbyte.obj ..\core\fmbyte.c



$O\fplock.obj:	..\win32\fplock.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fplock.obj ..\win32\fplock.c



$O\fpsig.obj:	..\core\fpsig.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fpsig.obj ..\core\fpsig.c



$O\fputchar.obj:	..\core\fputchar.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fputchar.obj ..\core\fputchar.c



$O\fputs.obj:	..\core\fputs.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fputs.obj ..\core\fputs.c



$O\fread.obj:	..\core\fread.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fread.obj ..\core\fread.c



$O\ftime.obj:	..\core\ftime.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ftime.obj ..\core\ftime.c



$O\fwide.obj:	..\core\fwide.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fwide.obj ..\core\fwide.c



$O\fwrite.obj:	..\core\fwrite.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fwrite.obj ..\core\fwrite.c



$O\gcvt.obj:	..\core\gcvt.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\gcvt.obj ..\core\gcvt.c



$O\getchar.obj:	..\core\getchar.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\getchar.obj ..\core\getchar.c



$O\getdate.obj:	..\win32\getdate.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\getdate.obj ..\win32\getdate.c



$O\getenv.obj:	..\win32\getenv.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\getenv.obj ..\win32\getenv.c



$O\getftime.obj:	..\win32\getftime.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\getftime.obj ..\win32\getftime.c



$O\getpid.obj:	..\core\getpid.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\getpid.obj ..\core\getpid.c



$O\gets.obj:	..\core\gets.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\gets.obj ..\core\gets.c



$O\gettime.obj:	..\win32\gettime.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\gettime.obj ..\win32\gettime.c



$O\hyper.obj:	..\core\hyper.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\hyper.obj ..\core\hyper.c



$O\ieee.obj:	..\core\ieee.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ieee.obj ..\core\ieee.c



$O\initmbcp.obj:	..\core\initmbcp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\initmbcp.obj ..\core\initmbcp.c



$O\iob.obj:	..\core\iob.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\iob.obj ..\core\iob.c



$O\isalnum.obj:	..\core\isalnum.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\isalnum.obj ..\core\isalnum.c



$O\isalpha.obj:	..\core\isalpha.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\isalpha.obj ..\core\isalpha.c



$O\isascii.obj:	..\core\isascii.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\isascii.obj ..\core\isascii.c



$O\isatty.obj:	..\win32\isatty.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\isatty.obj ..\win32\isatty.c



$O\isblank.obj:	..\core\isblank.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\isblank.obj ..\core\isblank.c



$O\iscntrl.obj:	..\core\iscntrl.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\iscntrl.obj ..\core\iscntrl.c



$O\iscsym.obj:	..\core\iscsym.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\iscsym.obj ..\core\iscsym.c



$O\iscsymf.obj:	..\core\iscsymf.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\iscsymf.obj ..\core\iscsymf.c



$O\isdigit.obj:	..\core\isdigit.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\isdigit.obj ..\core\isdigit.c



$O\isgraph.obj:	..\core\isgraph.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\isgraph.obj ..\core\isgraph.c



$O\islower.obj:	..\core\islower.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\islower.obj ..\core\islower.c



$O\isprint.obj:	..\core\isprint.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\isprint.obj ..\core\isprint.c



$O\ispunct.obj:	..\core\ispunct.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ispunct.obj ..\core\ispunct.c



$O\isspace.obj:	..\core\isspace.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\isspace.obj ..\core\isspace.c



$O\isupper.obj:	..\core\isupper.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\isupper.obj ..\core\isupper.c



$O\isxdigit.obj:	..\core\isxdigit.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\isxdigit.obj ..\core\isxdigit.c



$O\jmp.obj:	..\core32\jmp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\jmp.obj ..\core32\jmp.c



$O\lconv.obj:	..\core\lconv.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\lconv.obj ..\core\lconv.c



$O\lfind.obj:	..\core\lfind.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\lfind.obj ..\core\lfind.c



$O\locking.obj:	..\win32\locking.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\locking.obj ..\win32\locking.c



$O\loctime.obj:	..\core\loctime.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\loctime.obj ..\core\loctime.c



$O\log.obj:	..\core\log.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\log.obj ..\core\log.c



$O\logb.obj:	..\core\logb.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\logb.obj ..\core\logb.c



$O\ilogb.obj:	..\core\ilogb.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ilogb.obj ..\core\ilogb.c



$O\cbrt.obj:	..\core\cbrt.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\cbrt.obj ..\core\cbrt.c



$O\lsearch.obj:	..\core\lsearch.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\lsearch.obj ..\core\lsearch.c



$O\math2.obj:	..\core\math2.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\math2.obj ..\core\math2.c



$O\matherr.obj:	..\core\matherr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\matherr.obj ..\core\matherr.c



$O\mbctype.obj:	..\core\mbctype.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\mbctype.obj ..\core\mbctype.c



$O\mbstowcs.obj:	..\core\mbstowcs.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\mbstowcs.obj ..\core\mbstowcs.c



$O\mbstring.obj:	..\core\mbstring.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\mbstring.obj ..\core\mbstring.c



$O\mbyte.obj:	..\core\mbyte.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\mbyte.obj ..\core\mbyte.c



$O\memccpy.obj:	..\core\memccpy.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\memccpy.obj ..\core\memccpy.c



$O\memcmp.obj:	..\core32\memcmp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\memcmp.obj ..\core32\memcmp.c



$O\memset.obj:	..\core32\memset.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\memset.obj ..\core32\memset.c



$O\okbigbuf.obj:	..\core\okbigbuf.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\okbigbuf.obj ..\core\okbigbuf.c



$O\onexit.obj:	..\core\onexit.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\onexit.obj ..\core\onexit.c



$O\osfhand.obj:	..\win32\osfhand.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\osfhand.obj ..\win32\osfhand.c



$O\perror.obj:	..\core\perror.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\perror.obj ..\core\perror.c



$O\poly.obj:	..\core\poly.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\poly.obj ..\core\poly.c



$O\pow.obj:	..\core\pow.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\pow.obj ..\core\pow.c



$O\putch.obj:	..\core\putch.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\putch.obj ..\core\putch.c



$O\putchar.obj:	..\core\putchar.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\putchar.obj ..\core\putchar.c



$O\putenv.obj:	..\core\putenv.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\putenv.obj ..\core\putenv.c



$O\puts.obj:	..\core\puts.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\puts.obj ..\core\puts.c



$O\qsort.obj:	..\core\qsort.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\qsort.obj ..\core\qsort.c



$O\rand.obj:	..\core\rand.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\rand.obj ..\core\rand.c



$O\response.obj:	..\core\response.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\response.obj ..\core\response.c



$O\rmtmp.obj:	..\core\rmtmp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\rmtmp.obj ..\core\rmtmp.c



$O\round.obj:	..\core\round.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\round.obj ..\core\round.c



$O\semlock.obj:	..\win32\semlock.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\semlock.obj ..\win32\semlock.c



$O\setenvp.obj:	..\core\setenvp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\setenvp.obj ..\core\setenvp.c



$O\seterrno.obj:	..\core\seterrno.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\seterrno.obj ..\core\seterrno.c



$O\setjmp.obj:	..\core\setjmp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\setjmp.obj ..\core\setjmp.c



$O\setlc.obj:	..\core\setlc.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\setlc.obj ..\core\setlc.c



$O\setmbcp.obj:	..\core\setmbcp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\setmbcp.obj ..\core\setmbcp.c



$O\setmode.obj:	..\core\setmode.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\setmode.obj ..\core\setmode.c



$O\setnterr.obj:	..\win32\setnterr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\setnterr.obj ..\win32\setnterr.c



$O\signal.obj:	..\core\signal.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\signal.obj ..\core\signal.c



$O\sin.obj:	..\core\sin.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\sin.obj ..\core\sin.c



$O\sleep.obj:	..\core\sleep.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\sleep.obj ..\core\sleep.c



$O\sound.obj:	..\win32\sound.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\sound.obj ..\win32\sound.c



$O\sqrt.obj:	..\core\sqrt.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\sqrt.obj ..\core\sqrt.c



$O\stack.obj:	..\core\stack.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\stack.obj ..\core\stack.c



$O\stckaval.obj:	..\core32\stckaval.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\stckaval.obj ..\core32\stckaval.c



$O\strcspn.obj:	..\core\strcspn.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strcspn.obj ..\core\strcspn.c



$O\strdup.obj:	..\core\strdup.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strdup.obj ..\core\strdup.c



$O\strerror.obj:	..\core\strerror.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strerror.obj ..\core\strerror.c



$O\strfunc.obj:	..\core32\strfunc.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strfunc.obj ..\core32\strfunc.c



$O\strlwr.obj:	..\core\strlwr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strlwr.obj ..\core\strlwr.c



$O\strnicmp.obj:	..\core\strnicmp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strnicmp.obj ..\core\strnicmp.c



$O\strnset.obj:	..\core\strnset.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strnset.obj ..\core\strnset.c



$O\strpbrk.obj:	..\core\strpbrk.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strpbrk.obj ..\core\strpbrk.c



$O\strrchr.obj:	..\core\strrchr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strrchr.obj ..\core\strrchr.c



$O\strrev.obj:	..\core\strrev.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strrev.obj ..\core\strrev.c



$O\strset.obj:	..\core\strset.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strset.obj ..\core\strset.c



$O\strspn.obj:	..\core\strspn.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strspn.obj ..\core\strspn.c



$O\strstr.obj:	..\core\strstr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strstr.obj ..\core\strstr.c



$O\strtod.obj:	..\core\strtod.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strtod.obj ..\core\strtod.c



$O\strtof.obj:	..\core\strtof.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strtof.obj ..\core\strtof.c



$O\strtok.obj:	..\core32\strtok.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strtok.obj ..\core32\strtok.c



$O\strtol.obj:	..\core\strtol.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strtol.obj ..\core\strtol.c



$O\strtoull.obj:	..\core32\strtoull.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strtoull.obj ..\core32\strtoull.c



$O\strupr.obj:	..\core\strupr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strupr.obj ..\core\strupr.c



$O\strxfrm.obj:	..\core\strxfrm.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strxfrm.obj ..\core\strxfrm.c



$O\swab.obj:	..\core\swab.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\swab.obj ..\core\swab.c



$O\tabsize.obj:	..\core\tabsize.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\tabsize.obj ..\core\tabsize.c



$O\tan.obj:	..\core\tan.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\tan.obj ..\core\tan.c



$O\tchar.obj:	..\core\tchar.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\tchar.obj ..\core\tchar.c



$O\thread.obj:	..\win32\thread.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\thread.obj ..\win32\thread.c



$O\threadex.obj:	..\win32\threadex.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\threadex.obj ..\win32\threadex.c



$O\thunk.obj:	..\win32\thunk.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -H- -o$O\thunk.obj ..\win32\thunk.cpp



$O\time.obj:	..\core\time.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\time.obj ..\core\time.c



$O\time3.obj:	..\core\time3.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\time3.obj ..\core\time3.c



$O\timesup.obj:	..\core\timesup.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\timesup.obj ..\core\timesup.c



$O\tlsdata.obj:	..\win32\tlsdata.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\tlsdata.obj ..\win32\tlsdata.c



$O\toascii.obj:	..\core\toascii.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\toascii.obj ..\core\toascii.c



$O\tolower.obj:	..\core\tolower.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\tolower.obj ..\core\tolower.c



$O\toupper.obj:	..\core\toupper.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\toupper.obj ..\core\toupper.c



$O\trigerr.obj:	..\core\trigerr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\trigerr.obj ..\core\trigerr.c



$O\umask.obj:	..\core\umask.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\umask.obj ..\core\umask.c



$O\ungetc.obj:	..\core\ungetc.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ungetc.obj ..\core\ungetc.c



$O\unmangle.obj:	..\core\unmangle.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\unmangle.obj ..\core\unmangle.c



$O\vprintf.obj:	..\core\vprintf.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\vprintf.obj ..\core\vprintf.c



$O\w32sver.obj:	..\win32\w32sver.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\w32sver.obj ..\win32\w32sver.c



$O\xcfilter.obj:	..\win32\xcfilter.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\xcfilter.obj ..\win32\xcfilter.c



$O\_delete.obj:	..\core\_delete.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\_delete.obj ..\core\_delete.cpp



$O\_delete2.obj:	..\core\_delete2.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\_delete2.obj ..\core\_delete2.cpp



$O\_delete3.obj:	..\core\_delete3.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\_delete3.obj ..\core\_delete3.cpp



$O\_delnear.obj:	..\core\_delnear.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\_delnear.obj ..\core\_delnear.cpp



$O\_handler.obj:	..\core\_handler.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\_handler.obj ..\core\_handler.cpp



$O\_new.obj:	..\core\_new.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\_new.obj ..\core\_new.cpp



$O\_new2.obj:	..\core\_new2.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\_new2.obj ..\core\_new2.cpp



$O\_new3.obj:	..\core\_new3.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\_new3.obj ..\core\_new3.cpp



$O\_newnear.obj:	..\core\_newnear.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\_newnear.obj ..\core\_newnear.cpp



$O\nothrow.obj:	..\core\nothrow.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\nothrow.obj ..\core\nothrow.cpp



$O\complex.obj:	..\core\complex.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\complex.obj ..\core\complex.cpp



$O\coperat.obj:	..\core\coperat.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\coperat.obj ..\core\coperat.cpp



$O\cstrm.obj:	..\core\cstrm.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\cstrm.obj ..\core\cstrm.cpp



$O\ctrig.obj:	..\core\ctrig.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\ctrig.obj ..\core\ctrig.cpp



$O\dbp.obj:	..\ios\dbp.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\dbp.obj ..\ios\dbp.cpp



$O\except.obj:	..\core\except.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\except.obj ..\core\except.cpp



$O\fstream.obj:	..\ios\fstream.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\fstream.obj ..\ios\fstream.cpp



$O\generr.obj:	..\ios\generr.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\generr.obj ..\ios\generr.cpp



$O\ios.obj:	..\ios\ios.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\ios.obj ..\ios\ios.cpp



$O\iostream.obj:	..\ios\iostream.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\iostream.obj ..\ios\iostream.cpp



$O\istream.obj:	..\ios\istream.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\istream.obj ..\ios\istream.cpp



$O\istreamf.obj:	..\ios\istreamf.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\istreamf.obj ..\ios\istreamf.cpp



$O\istreami.obj:	..\ios\istreami.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\istreami.obj ..\ios\istreami.cpp



$O\istreamx.obj:	..\ios\istreamx.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\istreamx.obj ..\ios\istreamx.cpp



$O\manip.obj:	..\ios\manip.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\manip.obj ..\ios\manip.cpp



$O\ostream.obj:	..\ios\ostream.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\ostream.obj ..\ios\ostream.cpp



$O\ostreamf.obj:	..\ios\ostreamf.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\ostreamf.obj ..\ios\ostreamf.cpp



$O\ostreami.obj:	..\ios\ostreami.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\ostreami.obj ..\ios\ostreami.cpp



$O\ostreamx.obj:	..\ios\ostreamx.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\ostreamx.obj ..\ios\ostreamx.cpp



$O\rtti.obj:	..\core\rtti.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\rtti.obj ..\core\rtti.cpp



$O\siosync.obj:	..\ios\siosync.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\siosync.obj ..\ios\siosync.cpp



$O\stdiostr.obj:	..\ios\stdiostr.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\stdiostr.obj ..\ios\stdiostr.cpp



$O\stream.obj:	..\ios\stream.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\stream.obj ..\ios\stream.cpp



$O\streambf.obj:	..\ios\streambf.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\streambf.obj ..\ios\streambf.cpp



$O\strstrea.obj:	..\ios\strstrea.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\strstrea.obj ..\ios\strstrea.cpp



$O\typeinfo.obj:	..\core\typeinfo.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\typeinfo.obj ..\core\typeinfo.cpp



$O\vecnew.obj:	..\core\vecnew.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\vecnew.obj ..\core\vecnew.cpp



$O\vector.obj:	..\core\vector.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\vector.obj ..\core\vector.cpp



$O\disp.obj:	..\win32\disp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\disp.obj ..\win32\disp.c



$O\scver.obj:	..\core\scver.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\scver.obj ..\core\scver.c



$O\alloca.obj:	..\core\alloca.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\alloca.obj ..\core\alloca.c



$O\anew.obj:	..\core\anew.cpp
		$(CC) $(CFLAGS_6) $(DEFINES) $(INCLUDES) -o$O\anew.obj ..\core\anew.cpp



$O\anew2.obj:	..\core\anew2.cpp
		$(CC) $(CFLAGS_6) $(DEFINES) $(INCLUDES) -o$O\anew2.obj ..\core\anew2.cpp



$O\anew3.obj:	..\core\anew3.cpp
		$(CC) $(CFLAGS_6) $(DEFINES) $(INCLUDES) -o$O\anew3.obj ..\core\anew3.cpp



$O\adelete.obj:	..\core\adelete.cpp
		$(CC) $(CFLAGS_6) $(DEFINES) $(INCLUDES) -o$O\adelete.obj ..\core\adelete.cpp



$O\adelete2.obj: ..\core\adelete2.cpp
		$(CC) $(CFLAGS_6) $(DEFINES) $(INCLUDES) -o$O\adelete2.obj ..\core\adelete2.cpp



$O\adelete3.obj: ..\core\adelete3.cpp
		$(CC) $(CFLAGS_6) $(DEFINES) $(INCLUDES) -o$O\adelete3.obj ..\core\adelete3.cpp



$O\corenew.obj:	..\core\corenew.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\corenew.obj ..\core\corenew.cpp



$O\purevirt.obj:	..\core\purevirt.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\purevirt.obj ..\core\purevirt.c



$O\vedbghk.obj:	..\win32\vedbghk.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\vedbghk.obj ..\win32\vedbghk.c



$O\calloc.obj:	..\heap32\calloc.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\calloc.obj ..\heap32\calloc.c



$O\free.obj:	..\heap32\free.c
		$(CC) $(CFLAGS_1) $(DEFINES) $(INCLUDES) -o$O\free.obj ..\heap32\free.c



$O\malloc.obj:	..\heap32\malloc.c
		$(CC) $(CFLAGS_1) $(DEFINES) $(INCLUDES) -o$O\malloc.obj ..\heap32\malloc.c



$O\msize.obj:	..\heap32\msize.c
		$(CC) $(CFLAGS_1) $(DEFINES) $(INCLUDES) -o$O\msize.obj ..\heap32\msize.c



$O\realloc.obj:	..\heap32\realloc.c
		$(CC) $(CFLAGS_1) $(DEFINES) $(INCLUDES) -o$O\realloc.obj ..\heap32\realloc.c



$O\sbrk.obj:	..\heap32\sbrk.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\sbrk.obj ..\heap32\sbrk.c



$O\heap32.obj:	..\heap32\heap32.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\heap32.obj ..\heap32\heap32.c



$O\ptrchk.obj:	..\heap32\ptrchk.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ptrchk.obj ..\heap32\ptrchk.c



$O\heap.obj:	..\heap32\heap.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\heap.obj ..\heap32\heap.cpp



$O\multpool.obj:	..\heap32\multpool.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\multpool.obj ..\heap32\multpool.cpp



$O\pool.obj:	..\heap32\pool.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\pool.obj ..\heap32\pool.cpp



$O\_alloca.obj:	..\core\_alloca.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_alloca.obj ..\core\_alloca.c



$O\heapinit.obj:	..\heap32\heapinit.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\heapinit.obj ..\heap32\heapinit.cpp



$O\_ptrchk.obj:	..\core32\_ptrchk.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_ptrchk.obj ..\core32\_ptrchk.c



$O\decdata.obj:	..\core\decdata.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\decdata.obj ..\core\decdata.c



$O\fgetwc.obj:	..\core\fgetwc.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fgetwc.obj ..\core\fgetwc.c



$O\fgetws.obj:	..\core\fgetws.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fgetws.obj ..\core\fgetws.c



$O\flushwbu.obj:	..\core\flushwbu.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\flushwbu.obj ..\core\flushwbu.c



$O\fputwc.obj:	..\core\fputwc.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fputwc.obj ..\core\fputwc.c



$O\fputws.obj:	..\core\fputws.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fputws.obj ..\core\fputws.c



$O\getws.obj:	..\core\getws.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\getws.obj ..\core\getws.c



$O\isctype.obj:	..\core\isctype.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\isctype.obj ..\core\isctype.c



$O\iswctype.obj:	..\core\iswctype.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\iswctype.obj ..\core\iswctype.c



$O\itow.obj:	..\core32\itow.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\itow.obj ..\core32\itow.c



$O\lcapi32.obj:	..\core\lcapi32.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\lcapi32.obj ..\core\lcapi32.c



$O\lccrtdr.obj:	..\core\lccrtdr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\lccrtdr.obj ..\core\lccrtdr.c



$O\lccrtfl.obj:	..\core\lccrtfl.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\lccrtfl.obj ..\core\lccrtfl.c



$O\LCCRTPC.obj:	..\core\LCCRTPC.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\LCCRTPC.obj ..\core\LCCRTPC.C



$O\LCDELFL.obj:	..\core\LCDELFL.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\LCDELFL.obj ..\core\LCDELFL.C



$O\LCFNDFL.obj:	..\core\LCFNDFL.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\LCFNDFL.obj ..\core\LCFNDFL.C



$O\LCGETCD.obj:	..\core\LCGETCD.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\LCGETCD.obj ..\core\LCGETCD.C



$O\LCGETCL.obj:	..\core\LCGETCL.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\LCGETCL.obj ..\core\LCGETCL.C



$O\LCGETFA.obj:	..\core\LCGETFA.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\LCGETFA.obj ..\core\LCGETFA.C



$O\LCGETMF.obj:	..\core\LCGETMF.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\LCGETMF.obj ..\core\LCGETMF.C



$O\LCMOVFL.obj:	..\core\LCMOVFL.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\LCMOVFL.obj ..\core\LCMOVFL.C



$O\LCRMVDR.obj:	..\core\LCRMVDR.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\LCRMVDR.obj ..\core\LCRMVDR.C



$O\LCSETCD.obj:	..\core\LCSETCD.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\LCSETCD.obj ..\core\LCSETCD.C



$O\LCSETFA.obj:	..\core\LCSETFA.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\LCSETFA.obj ..\core\LCSETFA.C



$O\LTOW.obj:	..\core32\LTOW.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\LTOW.obj ..\core32\LTOW.C



$O\MBTOWC.obj:	..\core\MBTOWC.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBTOWC.obj ..\core\MBTOWC.C



$O\NLSDATA.obj:	..\core\NLSDATA.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\NLSDATA.obj ..\core\NLSDATA.C



$O\NULDATA.obj:	..\core\NULDATA.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\NULDATA.obj ..\core\NULDATA.C



$O\PUTWS.obj:	..\core\PUTWS.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\PUTWS.obj ..\core\PUTWS.C



$O\SETCHARS.obj:	..\core\SETCHARS.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\SETCHARS.obj ..\core\SETCHARS.C



$O\STRCOLL.obj:	..\core\STRCOLL.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\STRCOLL.obj ..\core\STRCOLL.C



$O\STRICOLL.obj:	..\core\STRICOLL.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\STRICOLL.obj ..\core\STRICOLL.C



$O\STRNCOLL.obj:	..\core\STRNCOLL.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\STRNCOLL.obj ..\core\STRNCOLL.C



$O\STRNICOL.obj:	..\core\STRNICOL.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\STRNICOL.obj ..\core\STRNICOL.C



$O\TIMDATA.obj:	..\core\TIMDATA.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\TIMDATA.obj ..\core\TIMDATA.C



$O\TNODATA.obj:	..\core\TNODATA.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\TNODATA.obj ..\core\TNODATA.C



$O\TOWLOWER.obj:	..\core\TOWLOWER.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\TOWLOWER.obj ..\core\TOWLOWER.C



$O\TOWUPPER.obj:	..\core\TOWUPPER.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\TOWUPPER.obj ..\core\TOWUPPER.C



$O\ULTOW.obj:	..\core32\ULTOW.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ULTOW.obj ..\core32\ULTOW.C



$O\UNGETWC.obj:	..\core\UNGETWC.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\UNGETWC.obj ..\core\UNGETWC.C



$O\_fopen.obj:	..\core\_fopen.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_fopen.obj ..\core\_fopen.c



$O\flush.obj:	..\core\flush.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\flush.obj ..\core\flush.c



$O\asctime.obj:	..\core\asctime.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\asctime.obj ..\core\asctime.c



$O\ctime.obj:	..\core\ctime.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ctime.obj ..\core\ctime.c



$O\execl.obj:	..\core\execl.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\execl.obj ..\core\execl.c



$O\execle.obj:	..\core\execle.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\execle.obj ..\core\execle.c



$O\execlp.obj:	..\core\execlp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\execlp.obj ..\core\execlp.c



$O\execlpe.obj:	..\core\execlpe.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\execlpe.obj ..\core\execlpe.c



$O\execv.obj:	..\core\execv.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\execv.obj ..\core\execv.c



$O\execve.obj:	..\core\execve.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\execve.obj ..\core\execve.c



$O\execvp.obj:	..\core\execvp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\execvp.obj ..\core\execvp.c



$O\execvpe.obj:	..\core\execvpe.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\execvpe.obj ..\core\execvpe.c



$O\fdopen.obj:	..\core\fdopen.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fdopen.obj ..\core\fdopen.c



$O\fmt.obj:	..\core\fmt.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fmt.obj ..\core\fmt.c



$O\fopen.obj:	..\core\fopen.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fopen.obj ..\core\fopen.c



$O\freopen.obj:	..\core\freopen.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\freopen.obj ..\core\freopen.c



$O\fsopen.obj:	..\core\fsopen.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\fsopen.obj ..\core\fsopen.c



$O\disk.obj:	..\core\disk.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\disk.obj ..\core\disk.c



$O\mktemp.obj:	..\core\mktemp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\mktemp.obj ..\core\mktemp.c



$O\printf.obj:	..\core\printf.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\printf.obj ..\core\printf.c



$O\scanf.obj:	..\core\scanf.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\scanf.obj ..\core\scanf.c



$O\sopenf.obj:	..\core\sopenf.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\sopenf.obj ..\core\sopenf.c



$O\spawnlpe.obj:	..\core\spawnlpe.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\spawnlpe.obj ..\core\spawnlpe.c



$O\spawn.obj:	..\core\spawn.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\spawn.obj ..\core\spawn.c



$O\spawnl.obj:	..\core\spawnl.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\spawnl.obj ..\core\spawnl.c



$O\spawnle.obj:	..\core\spawnle.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\spawnle.obj ..\core\spawnle.c



$O\spawnlp.obj:	..\core\spawnlp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\spawnlp.obj ..\core\spawnlp.c



$O\spawnv.obj:	..\core\spawnv.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\spawnv.obj ..\core\spawnv.c



$O\spawnve.obj:	..\core\spawnve.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\spawnve.obj ..\core\spawnve.c



$O\spawnvp.obj:	..\core\spawnvp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\spawnvp.obj ..\core\spawnvp.c



$O\spawnvpe.obj:	..\core\spawnvpe.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\spawnvpe.obj ..\core\spawnvpe.c



$O\splitpat.obj:	..\core\splitpat.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\splitpat.obj ..\core\splitpat.c



$O\stat.obj:	..\core\stat.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\stat.obj ..\core\stat.c



$O\strf.obj:	..\core\strf.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strf.obj ..\core\strf.c



$O\system.obj:	..\core\system.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\system.obj ..\core\system.c



$O\tmpnam.obj:	..\core\tmpnam.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\tmpnam.obj ..\core\tmpnam.c



$O\getdos.obj:	..\win32\getdos.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\getdos.obj ..\win32\getdos.c



$O\chsize.obj:	..\win32\chsize.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\chsize.obj ..\win32\chsize.c



$O\constart.obj:	..\win32\constart.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\constart.obj ..\win32\constart.c



$O\exec2.obj:	..\win32\exec2.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\exec2.obj ..\win32\exec2.c



$O\find.obj:	..\win32\find.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\find.obj ..\win32\find.c



$O\getcwd.obj:	..\win32\getcwd.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\getcwd.obj ..\win32\getcwd.c



$O\io.obj:	..\win32\io.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\io.obj ..\win32\io.c



$O\setargv.obj:	..\win32\setargv.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\setargv.obj ..\win32\setargv.c



$O\unlink.obj:	..\win32\unlink.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\unlink.obj ..\win32\unlink.c



$O\utime.obj:	..\win32\utime.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\utime.obj ..\win32\utime.c



$O\winstart.obj:	..\win32\winstart.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\winstart.obj ..\win32\winstart.c



$O\thsup.obj:	..\win32\thsup.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\thsup.obj ..\win32\thsup.c



$O\ehdata.obj:	..\core\ehdata.cpp
		$(CC) $(CFLAGS_42) $(DEFINES_42) $(INCLUDES) -o$O\ehdata.obj ..\core\ehdata.cpp



$O\_ISMBSLD.obj:	..\CORE\_ISMBSLD.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_ISMBSLD.obj ..\CORE\_ISMBSLD.C



$O\_ISMBSTL.obj:	..\CORE\_ISMBSTL.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_ISMBSTL.obj ..\CORE\_ISMBSTL.C



$O\ISMBALNU.obj:	..\CORE\ISMBALNU.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ISMBALNU.obj ..\CORE\ISMBALNU.C



$O\ISMBALPH.obj:	..\CORE\ISMBALPH.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ISMBALPH.obj ..\CORE\ISMBALPH.C



$O\ISMBBYTE.obj:	..\CORE\ISMBBYTE.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ISMBBYTE.obj ..\CORE\ISMBBYTE.C



$O\ISMBCL0.obj:	..\CORE\ISMBCL0.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ISMBCL0.obj ..\CORE\ISMBCL0.C



$O\ISMBCL1.obj:	..\CORE\ISMBCL1.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ISMBCL1.obj ..\CORE\ISMBCL1.C



$O\ISMBCL2.obj:	..\CORE\ISMBCL2.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ISMBCL2.obj ..\CORE\ISMBCL2.C



$O\ISMBDIGI.obj:	..\CORE\ISMBDIGI.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ISMBDIGI.obj ..\CORE\ISMBDIGI.C



$O\ISMBGRAP.obj:	..\CORE\ISMBGRAP.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ISMBGRAP.obj ..\CORE\ISMBGRAP.C



$O\ISMBHIRA.obj:	..\CORE\ISMBHIRA.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ISMBHIRA.obj ..\CORE\ISMBHIRA.C



$O\ISMBKATA.obj:	..\CORE\ISMBKATA.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ISMBKATA.obj ..\CORE\ISMBKATA.C



$O\ISMBLEGA.obj:	..\CORE\ISMBLEGA.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ISMBLEGA.obj ..\CORE\ISMBLEGA.C



$O\ISMBLOWE.obj:	..\CORE\ISMBLOWE.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ISMBLOWE.obj ..\CORE\ISMBLOWE.C



$O\ISMBPRIN.obj:	..\CORE\ISMBPRIN.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ISMBPRIN.obj ..\CORE\ISMBPRIN.C



$O\ISMBPUNC.obj:	..\CORE\ISMBPUNC.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ISMBPUNC.obj ..\CORE\ISMBPUNC.C



$O\ISMBSPAC.obj:	..\CORE\ISMBSPAC.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ISMBSPAC.obj ..\CORE\ISMBSPAC.C



$O\ISMBSYMB.obj:	..\CORE\ISMBSYMB.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ISMBSYMB.obj ..\CORE\ISMBSYMB.C



$O\ISMBUPPE.obj:	..\CORE\ISMBUPPE.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ISMBUPPE.obj ..\CORE\ISMBUPPE.C



$O\MBBTOMBC.obj:	..\CORE\MBBTOMBC.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBBTOMBC.obj ..\CORE\MBBTOMBC.C



$O\MBBTYPE.obj:	..\CORE\MBBTYPE.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBBTYPE.obj ..\CORE\MBBTYPE.C



$O\MBCCPY.obj:	..\CORE\MBCCPY.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBCCPY.obj ..\CORE\MBCCPY.C



$O\MBCLEN.obj:	..\CORE\MBCLEN.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBCLEN.obj ..\CORE\MBCLEN.C



$O\MBCPINFO.obj:	..\CORE\MBCPINFO.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBCPINFO.obj ..\CORE\MBCPINFO.C



$O\MBJISJMS.obj:	..\CORE\MBJISJMS.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBJISJMS.obj ..\CORE\MBJISJMS.C



$O\MBJMSJIS.obj:	..\CORE\MBJMSJIS.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBJMSJIS.obj ..\CORE\MBJMSJIS.C



$O\MBLEN.obj:	..\CORE\MBLEN.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBLEN.obj ..\CORE\MBLEN.C



$O\MBSBTYPE.obj:	..\CORE\MBSBTYPE.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSBTYPE.obj ..\CORE\MBSBTYPE.C



$O\MBSCHR.obj:	..\CORE\MBSCHR.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSCHR.obj ..\CORE\MBSCHR.C



$O\MBSCMP.obj:	..\CORE\MBSCMP.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSCMP.obj ..\CORE\MBSCMP.C



$O\MBSCOLL.obj:	..\CORE\MBSCOLL.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSCOLL.obj ..\CORE\MBSCOLL.C



$O\MBSCSPN.obj:	..\CORE\MBSCSPN.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSCSPN.obj ..\CORE\MBSCSPN.C



$O\MBSDEC.obj:	..\CORE\MBSDEC.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSDEC.obj ..\CORE\MBSDEC.C



$O\MBSICMP.obj:	..\CORE\MBSICMP.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSICMP.obj ..\CORE\MBSICMP.C



$O\MBSICOLL.obj:	..\CORE\MBSICOLL.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSICOLL.obj ..\CORE\MBSICOLL.C



$O\MBSINC.obj:	..\CORE\MBSINC.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSINC.obj ..\CORE\MBSINC.C



$O\MBSLEN.obj:	..\CORE\MBSLEN.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSLEN.obj ..\CORE\MBSLEN.C



$O\MBSLWR.obj:	..\CORE\MBSLWR.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSLWR.obj ..\CORE\MBSLWR.C



$O\MBSNBCAT.obj:	..\CORE\MBSNBCAT.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSNBCAT.obj ..\CORE\MBSNBCAT.C



$O\MBSNBCMP.obj:	..\CORE\MBSNBCMP.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSNBCMP.obj ..\CORE\MBSNBCMP.C



$O\MBSNBCNT.obj:	..\CORE\MBSNBCNT.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSNBCNT.obj ..\CORE\MBSNBCNT.C



$O\MBSNBCOL.obj:	..\CORE\MBSNBCOL.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSNBCOL.obj ..\CORE\MBSNBCOL.C



$O\MBSNBCPY.obj:	..\CORE\MBSNBCPY.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSNBCPY.obj ..\CORE\MBSNBCPY.C



$O\MBSNBICM.obj:	..\CORE\MBSNBICM.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSNBICM.obj ..\CORE\MBSNBICM.C



$O\MBSNBICO.obj:	..\CORE\MBSNBICO.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSNBICO.obj ..\CORE\MBSNBICO.C



$O\MBSNBSET.obj:	..\CORE\MBSNBSET.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSNBSET.obj ..\CORE\MBSNBSET.C



$O\MBSNCAT.obj:	..\CORE\MBSNCAT.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSNCAT.obj ..\CORE\MBSNCAT.C



$O\MBSNCCNT.obj:	..\CORE\MBSNCCNT.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSNCCNT.obj ..\CORE\MBSNCCNT.C



$O\MBSNCMP.obj:	..\CORE\MBSNCMP.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSNCMP.obj ..\CORE\MBSNCMP.C



$O\MBSNCOLL.obj:	..\CORE\MBSNCOLL.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSNCOLL.obj ..\CORE\MBSNCOLL.C



$O\MBSNCPY.obj:	..\CORE\MBSNCPY.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSNCPY.obj ..\CORE\MBSNCPY.C



$O\MBSNEXTC.obj:	..\CORE\MBSNEXTC.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSNEXTC.obj ..\CORE\MBSNEXTC.C



$O\MBSNICMP.obj:	..\CORE\MBSNICMP.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSNICMP.obj ..\CORE\MBSNICMP.C



$O\MBSNICOL.obj:	..\CORE\MBSNICOL.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSNICOL.obj ..\CORE\MBSNICOL.C



$O\MBSNINC.obj:	..\CORE\MBSNINC.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSNINC.obj ..\CORE\MBSNINC.C



$O\MBSNSET.obj:	..\CORE\MBSNSET.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSNSET.obj ..\CORE\MBSNSET.C



$O\MBSPBRK.obj:	..\CORE\MBSPBRK.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSPBRK.obj ..\CORE\MBSPBRK.C



$O\MBSRCHR.obj:	..\CORE\MBSRCHR.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSRCHR.obj ..\CORE\MBSRCHR.C



$O\MBSREV.obj:	..\CORE\MBSREV.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSREV.obj ..\CORE\MBSREV.C



$O\MBSSET.obj:	..\CORE\MBSSET.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSSET.obj ..\CORE\MBSSET.C



$O\MBSSPN.obj:	..\CORE\MBSSPN.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSSPN.obj ..\CORE\MBSSPN.C



$O\MBSSPNP.obj:	..\CORE\MBSSPNP.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSSPNP.obj ..\CORE\MBSSPNP.C



$O\MBSSTR.obj:	..\CORE\MBSSTR.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSSTR.obj ..\CORE\MBSSTR.C



$O\MBSTOK.obj:	..\CORE\MBSTOK.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSTOK.obj ..\CORE\MBSTOK.C



$O\MBSTRLEN.obj:	..\CORE\MBSTRLEN.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSTRLEN.obj ..\CORE\MBSTRLEN.C



$O\MBSUPR.obj:	..\CORE\MBSUPR.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBSUPR.obj ..\CORE\MBSUPR.C



$O\MBTOHIRA.obj:	..\CORE\MBTOHIRA.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBTOHIRA.obj ..\CORE\MBTOHIRA.C



$O\MBTOKATA.obj:	..\CORE\MBTOKATA.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBTOKATA.obj ..\CORE\MBTOKATA.C



$O\MBTOLOWE.obj:	..\CORE\MBTOLOWE.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBTOLOWE.obj ..\CORE\MBTOLOWE.C



$O\MBTOUPPE.obj:	..\CORE\MBTOUPPE.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\MBTOUPPE.obj ..\CORE\MBTOUPPE.C



$O\w32fater.obj:	..\win32\w32fater.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\w32fater.obj ..\win32\w32fater.c


$O\ldclass.obj:	..\core32\ldclass.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ldclass.obj ..\core32\ldclass.c



$O\cmath.obj:	..\core\cmath.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\cmath.obj ..\core\cmath.c



$O\complex2.obj:	..\core\complex2.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\complex2.obj ..\core\complex2.c



$O\strtold.obj:	..\core\strtold.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strtold.obj ..\core\strtold.c



$O\ldexpl.obj: ..\win32\ldexpl.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\ldexpl.obj ..\win32\ldexpl.c



$O\frexpl.obj: ..\win32\frexpl.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\frexpl.obj ..\win32\frexpl.c



$O\regexp.obj:	..\core\regexp.cpp
		$(CC) $(CFLAGS_6) $(DEFINES_49) $(INCLUDES) -o$O\regexp.obj ..\core\regexp.cpp



$O\inttypes.obj:	..\core\inttypes.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\inttypes.obj ..\core\inttypes.c



$O\strtoll.obj:	..\core\strtoll.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\strtoll.obj ..\core\strtoll.c


$O\wcstoll.obj:	..\core\strtoll.c
		$(CC) $(CFLAGS_6) $(DEFINES) $(INCLUDES) -D_MT -D_UNICODE -o$O\wcstoll.obj ..\core\strtoll.c


$O\wcstoull.obj: ..\core\wcstoull.c
		$(CC) $(CFLAGS_6) $(DEFINES) $(INCLUDES) -D_MT -D_UNICODE -o$O\wcstoull.obj ..\core\wcstoull.c


$O\hypot.obj:	..\core\hypot.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\hypot.obj ..\core\hypot.c


$O\file.obj:	..\core\file.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\file.obj ..\core\file.c

$O\_find.obj:	..\win32\_find.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$O\_find.obj ..\win32\_find.c






$(SYMROOT).SYM: ..\include\precomp.h
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF$(SYMROOT).SYM -o$(SYMROOT).PCO ..\include\precomp.h



