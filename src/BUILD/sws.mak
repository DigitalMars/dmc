ORIGIN		= Digital Mars C++
ORIGIN_VER	= Version 7.20
VERSION		= RELEASE

!IFDEF SUB_DEBUG
DEBUG		= $(SUB_DEBUG)
NDEBUG		= !$(SUB_DEBUG)
!ELSE
DEBUG		= 0
NDEBUG		= 1
!ENDIF

PROJ		= sws
APPTYPE		= WINDOWS LIB
PROJTYPE	= LIB

CC		= SC
CPP		= SPP
MAKE		= SMAKE
RC		= RCC
HC		= HC31
ASM		= SC
DISASM		= OBJ2ASM
LNK		= LINK
DLLS		= 

HEADERS		= ..\..\include\float.h ..\..\include\string.h ..\..\include\stdlib.h  \
		..\..\include\dos.h ..\..\include\process.h ..\..\include\int.h ..\..\include\locale.h  \
		..\..\include\stream.h ..\..\include\stdiostr.h ..\..\include\generic.h ..\..\include\iomanip.h  \
		..\..\include\strstrea.h ..\..\include\share.h ..\..\include\typeinfo.h ..\..\include\fstream.h  \
		..\..\include\eh.h ..\ios\filesys.h ..\..\include\complex.h ..\..\include\iostream.h  \
		..\..\include\new.h ..\..\include\msmouse.h ..\..\include\winc.h ..\..\include\wchar.h  \
		..\..\include\mbstring.h ..\..\include\tchar.h ..\..\include\tabsize.h ..\..\include\assert.h  \
		..\..\include\ctype.h ..\..\include\direct.h ..\..\include\stdarg.h ..\include\scio.h  \
		..\..\include\win16\windows.h ..\..\include\windows.h ..\..\include\math.h ..\..\include\fp.h  \
		..\..\include\fltpnt.h ..\..\include\signal.h ..\..\include\sys\stat.h ..\include\scrtl.h  \
		..\include\semlock.h ..\..\include\fenv.h ..\..\include\fltenv.h ..\..\include\time.h  \
		..\include\mt.h ..\..\include\errno.h ..\..\include\fcntl.h ..\..\include\io.h  \
		..\..\include\stddef.h ..\..\include\stdio.h ..\..\include\limits.h ..\..\include\dpmi.h  \
		..\..\include\memory.h ..\..\include\search.h ..\..\include\malloc.h ..\..\include\exitstat.h  \
		..\..\include\setjmp.h ..\..\include\disp.h ..\..\include\bios.h ..\..\include\mbctype.h  \
		..\..\include\sys\timeb.h 

DEFFILE		= sws.def

!IF $(DEBUG)
OUTPUTDIR	= SWS
!IF EXIST (SWS)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $(OUTPUTDIR)\*.* md $(OUTPUTDIR)
!ENDIF
TARGETDIR	= ..\..\LIB
!IF EXIST (..\..\LIB)
CREATETARGETDIR	=
!ELSE
CREATETARGETDIR	= if not exist $(TARGETDIR)\*.* md $(TARGETDIR)
!ENDIF

LIBS		= 

CFLAGS		=  -Ae -Ar -r -ms -W1 -NL -4 -a2 -c -wx -w- -w12 -g -gd 
CFLAGS_1	=  -Ae -Ar -Aa -r -ms -W1 -NL -4 -a2 -c -wx -w- -w12 -g -gd
DEFINES_1	= -D_MBCS
CFLAGS_2	=  -Ae -Ar -Aa -r -ms -W1 -NL -4 -a2 -c -wx -w- -w12 -g -gd
DEFINES_2	= -D_MBCS
CFLAGS_3	=  -Ae -Ar -Aa -r -ms -W1 -NL -4 -a2 -c -wx -w- -w12 -g -gd
DEFINES_3	= -D_MBCS
CFLAGS_4	=  -Ae -Ar -r -ms -W1 -f -NL -4 -a2 -Nc -c -wx -w- -w12 -g -gd
DEFINES_4	= -D_MBCS
LFLAGS		=  /CO /DE /PACKF /XU /A:512
DEFINES		= -D_MBCS
!ELSE
OUTPUTDIR	= SWS
!IF EXIST (SWS)
CREATEOUTPUTDIR	=
!ELSE
CREATEOUTPUTDIR	= if not exist $(OUTPUTDIR)\*.* md $(OUTPUTDIR)
!ENDIF
TARGETDIR	= ..\..\LIB
!IF EXIST (..\..\LIB)
CREATETARGETDIR	=
!ELSE
CREATETARGETDIR	= if not exist $(TARGETDIR)\*.* md $(TARGETDIR)
!ENDIF

LIBS		= 

CFLAGS		=  -Ae -Ar -r -ms -o+time -W1 -NL -4 -a2 -Nc -c -wx -w- -w12 
CFLAGS_1	=  -Ae -Ar -Aa -r -ms -o+time -W1 -NL -4 -a2 -Nc -c -wx -w- -w12
DEFINES_1	= -D_MBCS
CFLAGS_2	=  -Ae -Ar -Aa -r -ms -o+time -W1 -NL -4 -a2 -Nc -c -wx -w- -w12
DEFINES_2	= -D_MBCS
CFLAGS_3	=  -Ae -Ar -Aa -r -ms -o+time -W1 -NL -4 -a2 -Nc -c -wx -w- -w12
DEFINES_3	= -D_MBCS
CFLAGS_4	=  -Ae -Ar -r -ms -o+time -W1 -f -NL -4 -a2 -Nc -c -wx -w- -w12
DEFINES_4	= -D_MBCS
LFLAGS		=  /DE /E /PACKF /XU /A:512
DEFINES		= -D_MBCS
!ENDIF

HFLAGS		= $(CFLAGS) 
MFLAGS		= MASTERPROJ=$(PROJ) 
LIBFLAGS	=  /C /NOI 
RESFLAGS	=  -32 
DEBUGGERFLAGS	=  
AFLAGS		= $(CFLAGS) 
HELPFLAGS	= 

MODEL		= S

PAR		= PROJS BATS OBJS

RCDEFINES	= 

INCLUDES	= -I..\INCLUDE -I..\..\INCLUDE

INCLUDEDOBJS	= 

OBJS		=  $(OUTPUTDIR)\_8087.OBJ  $(OUTPUTDIR)\_afulshr.OBJ  $(OUTPUTDIR)\_anlshl.OBJ  \
		 $(OUTPUTDIR)\_anulshr.OBJ  $(OUTPUTDIR)\_fmemicm.OBJ  $(OUTPUTDIR)\_ptrchk.OBJ  $(OUTPUTDIR)\amath.OBJ  \
		 $(OUTPUTDIR)\atoi.OBJ  $(OUTPUTDIR)\atol.OBJ  $(OUTPUTDIR)\baslnk.OBJ  $(OUTPUTDIR)\bios.OBJ  \
		 $(OUTPUTDIR)\calloc.OBJ  $(OUTPUTDIR)\cerror.OBJ  $(OUTPUTDIR)\chkstk.OBJ  $(OUTPUTDIR)\cinit.OBJ  \
		 $(OUTPUTDIR)\clock.OBJ  $(OUTPUTDIR)\controlc.OBJ  $(OUTPUTDIR)\country.OBJ  $(OUTPUTDIR)\cputype.OBJ  \
		 $(OUTPUTDIR)\cw.OBJ  $(OUTPUTDIR)\dclass.OBJ  $(OUTPUTDIR)\disp.OBJ  $(OUTPUTDIR)\dllstart.OBJ  \
		 $(OUTPUTDIR)\dos_find.OBJ  $(OUTPUTDIR)\dosalloc.OBJ  $(OUTPUTDIR)\dosdate.OBJ  $(OUTPUTDIR)\doserr.OBJ  \
		 $(OUTPUTDIR)\dosfil.OBJ  $(OUTPUTDIR)\double.OBJ  $(OUTPUTDIR)\dpmi.OBJ  $(OUTPUTDIR)\dpmimin.OBJ  \
		 $(OUTPUTDIR)\dup.OBJ  $(OUTPUTDIR)\ehasm.OBJ  $(OUTPUTDIR)\emm.OBJ  $(OUTPUTDIR)\exec2.OBJ  \
		 $(OUTPUTDIR)\farptr.OBJ  $(OUTPUTDIR)\fcalloc.OBJ  $(OUTPUTDIR)\filelen.OBJ  $(OUTPUTDIR)\find.OBJ  \
		 $(OUTPUTDIR)\float.OBJ  $(OUTPUTDIR)\floattyp.OBJ  $(OUTPUTDIR)\flt87.OBJ  $(OUTPUTDIR)\fmalloc.OBJ  \
		 $(OUTPUTDIR)\fmemchr.OBJ  $(OUTPUTDIR)\fmemcmp.OBJ  $(OUTPUTDIR)\fmemcpy.OBJ  $(OUTPUTDIR)\fmemicmp.OBJ  \
		 $(OUTPUTDIR)\fmemset.OBJ  $(OUTPUTDIR)\fpatan16.OBJ  $(OUTPUTDIR)\fprem16.OBJ  $(OUTPUTDIR)\fptan16.OBJ  \
		 $(OUTPUTDIR)\fputc.OBJ  $(OUTPUTDIR)\fqq.OBJ  $(OUTPUTDIR)\frealloc.OBJ  $(OUTPUTDIR)\free.OBJ  \
		 $(OUTPUTDIR)\fstrcat.OBJ  $(OUTPUTDIR)\fstrchr.OBJ  $(OUTPUTDIR)\fstrcmp.OBJ  $(OUTPUTDIR)\fstrcpy.OBJ  \
		 $(OUTPUTDIR)\fstricmp.OBJ  $(OUTPUTDIR)\fstrlen.OBJ  $(OUTPUTDIR)\fstrncat.OBJ  $(OUTPUTDIR)\fstrncmp.OBJ  \
		 $(OUTPUTDIR)\fstrncpy.OBJ  $(OUTPUTDIR)\getcwd.OBJ  $(OUTPUTDIR)\getdos.OBJ  $(OUTPUTDIR)\getenv.OBJ  \
		 $(OUTPUTDIR)\getthd.OBJ  $(OUTPUTDIR)\handle.OBJ  $(OUTPUTDIR)\hdiff.OBJ  $(OUTPUTDIR)\hugeptr.OBJ  \
		 $(OUTPUTDIR)\inter.OBJ  $(OUTPUTDIR)\io.OBJ  $(OUTPUTDIR)\itoa.OBJ  $(OUTPUTDIR)\kbhit.OBJ  \
		 $(OUTPUTDIR)\ldexp.OBJ  $(OUTPUTDIR)\lmath.OBJ  $(OUTPUTDIR)\locking.OBJ  $(OUTPUTDIR)\lseek.OBJ  \
		 $(OUTPUTDIR)\malloc.OBJ  $(OUTPUTDIR)\math87.OBJ  $(OUTPUTDIR)\mathtrn.OBJ  $(OUTPUTDIR)\mathtrnf.OBJ  \
		 $(OUTPUTDIR)\memavl.OBJ  $(OUTPUTDIR)\memchr.OBJ  $(OUTPUTDIR)\memcmp.OBJ  $(OUTPUTDIR)\memcpy.OBJ  \
		 $(OUTPUTDIR)\memicmp.OBJ  $(OUTPUTDIR)\memmax.OBJ  $(OUTPUTDIR)\memset.OBJ  $(OUTPUTDIR)\movedata.OBJ  \
		 $(OUTPUTDIR)\msize.OBJ  $(OUTPUTDIR)\ncalloc.OBJ  $(OUTPUTDIR)\nmalloc.OBJ  $(OUTPUTDIR)\nrealloc.OBJ  \
		 $(OUTPUTDIR)\page.OBJ  $(OUTPUTDIR)\parall.OBJ  $(OUTPUTDIR)\patch16.OBJ  $(OUTPUTDIR)\peek.OBJ  \
		 $(OUTPUTDIR)\port.OBJ  $(OUTPUTDIR)\read.OBJ  $(OUTPUTDIR)\realloc.OBJ  $(OUTPUTDIR)\rot.OBJ  \
		 $(OUTPUTDIR)\seg.OBJ  $(OUTPUTDIR)\setargv.OBJ  $(OUTPUTDIR)\sound.OBJ  $(OUTPUTDIR)\stkovfl.OBJ  \
		 $(OUTPUTDIR)\stpcpy.OBJ  $(OUTPUTDIR)\strcat.OBJ  $(OUTPUTDIR)\strchr.OBJ  $(OUTPUTDIR)\strcmp.OBJ  \
		 $(OUTPUTDIR)\strcmpl.OBJ  $(OUTPUTDIR)\strcpy.OBJ  $(OUTPUTDIR)\strlen.OBJ  $(OUTPUTDIR)\strncat.OBJ  \
		 $(OUTPUTDIR)\strncmp.OBJ  $(OUTPUTDIR)\strncpy.OBJ  $(OUTPUTDIR)\strtoul.OBJ  $(OUTPUTDIR)\sysint.OBJ  \
		 $(OUTPUTDIR)\utime.OBJ  $(OUTPUTDIR)\wincio.OBJ  $(OUTPUTDIR)\write.OBJ  $(OUTPUTDIR)\_afftol.OBJ  \
		 $(OUTPUTDIR)\_afulrem.OBJ  $(OUTPUTDIR)\_dup.OBJ  $(OUTPUTDIR)\_fcvt.OBJ  $(OUTPUTDIR)\_fgtchar.OBJ  \
		 $(OUTPUTDIR)\_fheap.OBJ  $(OUTPUTDIR)\_fmemmve.OBJ  $(OUTPUTDIR)\_fmemset.OBJ  $(OUTPUTDIR)\_fopen.OBJ  \
		 $(OUTPUTDIR)\_fptchar.OBJ  $(OUTPUTDIR)\_fstrchr.OBJ  $(OUTPUTDIR)\_fstrtok.OBJ  $(OUTPUTDIR)\_ismbbld.OBJ  \
		 $(OUTPUTDIR)\_ismbbtl.OBJ  $(OUTPUTDIR)\_itoa.OBJ  $(OUTPUTDIR)\_ltoa.OBJ  $(OUTPUTDIR)\_main.OBJ  \
		 $(OUTPUTDIR)\_memicmp.OBJ  $(OUTPUTDIR)\_nstrdup.OBJ  $(OUTPUTDIR)\_printf.OBJ  $(OUTPUTDIR)\_strdup.OBJ  \
		 $(OUTPUTDIR)\_strerro.OBJ  $(OUTPUTDIR)\_strrev.OBJ  $(OUTPUTDIR)\_tzset.OBJ  $(OUTPUTDIR)\_wep.OBJ  \
		 $(OUTPUTDIR)\allocmem.OBJ  $(OUTPUTDIR)\arcerr.OBJ  $(OUTPUTDIR)\asctime.OBJ  $(OUTPUTDIR)\asin.OBJ  \
		 $(OUTPUTDIR)\assert.OBJ  $(OUTPUTDIR)\atan.OBJ  $(OUTPUTDIR)\atexit.OBJ  $(OUTPUTDIR)\atof.OBJ  \
		 $(OUTPUTDIR)\biosc.OBJ  $(OUTPUTDIR)\buildenv.OBJ  $(OUTPUTDIR)\cgets.OBJ  $(OUTPUTDIR)\charmax.OBJ  \
		 $(OUTPUTDIR)\chsize.OBJ  $(OUTPUTDIR)\commit.OBJ  $(OUTPUTDIR)\coreleft.OBJ  $(OUTPUTDIR)\cpphook.OBJ  \
		 $(OUTPUTDIR)\cprintf.OBJ  $(OUTPUTDIR)\cputs.OBJ  $(OUTPUTDIR)\creat.OBJ  $(OUTPUTDIR)\cscanf.OBJ  \
		 $(OUTPUTDIR)\ctime.OBJ  $(OUTPUTDIR)\ctrlbrk.OBJ  $(OUTPUTDIR)\ctype.OBJ  $(OUTPUTDIR)\disk.OBJ  \
		 $(OUTPUTDIR)\diskfree.OBJ  $(OUTPUTDIR)\disp2.OBJ  $(OUTPUTDIR)\doscomp.OBJ  $(OUTPUTDIR)\doskeep.OBJ  \
		 $(OUTPUTDIR)\doslock.OBJ  $(OUTPUTDIR)\dosseek.OBJ  $(OUTPUTDIR)\dprintf.OBJ  $(OUTPUTDIR)\ecvt.OBJ  \
		 $(OUTPUTDIR)\eof.OBJ  $(OUTPUTDIR)\execl.OBJ  $(OUTPUTDIR)\execle.OBJ  $(OUTPUTDIR)\execlp.OBJ  \
		 $(OUTPUTDIR)\execlpe.OBJ  $(OUTPUTDIR)\execv.OBJ  $(OUTPUTDIR)\execve.OBJ  $(OUTPUTDIR)\execvp.OBJ  \
		 $(OUTPUTDIR)\execvpe.OBJ  $(OUTPUTDIR)\exit.OBJ  $(OUTPUTDIR)\exitstat.OBJ  $(OUTPUTDIR)\exp.OBJ  \
		 $(OUTPUTDIR)\far.OBJ  $(OUTPUTDIR)\fatexit.OBJ  $(OUTPUTDIR)\fclose.OBJ  $(OUTPUTDIR)\fcloseal.OBJ  \
		 $(OUTPUTDIR)\fcvt.OBJ  $(OUTPUTDIR)\fdivp.OBJ  $(OUTPUTDIR)\fdopen.OBJ  $(OUTPUTDIR)\feexcept.OBJ  \
		 $(OUTPUTDIR)\fflush.OBJ  $(OUTPUTDIR)\fgetchar.OBJ  $(OUTPUTDIR)\fgets.OBJ  $(OUTPUTDIR)\floatcvt.OBJ  \
		 $(OUTPUTDIR)\fltenv.OBJ  $(OUTPUTDIR)\fltused.OBJ  $(OUTPUTDIR)\flush.OBJ  $(OUTPUTDIR)\flushall.OBJ  \
		 $(OUTPUTDIR)\fmacros.OBJ  $(OUTPUTDIR)\fmbyte.OBJ  $(OUTPUTDIR)\fmsize.OBJ  $(OUTPUTDIR)\fmt.OBJ  \
		 $(OUTPUTDIR)\fonexit.OBJ  $(OUTPUTDIR)\fopen.OBJ  $(OUTPUTDIR)\fpmath.OBJ  $(OUTPUTDIR)\fpsig.OBJ  \
		 $(OUTPUTDIR)\fputchar.OBJ  $(OUTPUTDIR)\fputs.OBJ  $(OUTPUTDIR)\fread.OBJ  $(OUTPUTDIR)\freect.OBJ  \
		 $(OUTPUTDIR)\freopen.OBJ  $(OUTPUTDIR)\fsopen.OBJ  $(OUTPUTDIR)\fstrcspn.OBJ  $(OUTPUTDIR)\fstrdup.OBJ  \
		 $(OUTPUTDIR)\fstrlwr.OBJ  $(OUTPUTDIR)\fstrnicm.OBJ  $(OUTPUTDIR)\fstrnset.OBJ  $(OUTPUTDIR)\fstrpbrk.OBJ  \
		 $(OUTPUTDIR)\fstrrchr.OBJ  $(OUTPUTDIR)\fstrrev.OBJ  $(OUTPUTDIR)\fstrset.OBJ  $(OUTPUTDIR)\fstrspn.OBJ  \
		 $(OUTPUTDIR)\fstrstr.OBJ  $(OUTPUTDIR)\fstrtok.OBJ  $(OUTPUTDIR)\fstrupr.OBJ  $(OUTPUTDIR)\ftime.OBJ  \
		 $(OUTPUTDIR)\fwrite.OBJ  $(OUTPUTDIR)\gcvt.OBJ  $(OUTPUTDIR)\getchar.OBJ  $(OUTPUTDIR)\getdate.OBJ  \
		 $(OUTPUTDIR)\getpid.OBJ  $(OUTPUTDIR)\gets.OBJ  $(OUTPUTDIR)\heap.OBJ  $(OUTPUTDIR)\hyper.OBJ  \
		 $(OUTPUTDIR)\ieee.OBJ  $(OUTPUTDIR)\iob.OBJ  $(OUTPUTDIR)\isalnum.OBJ  $(OUTPUTDIR)\isalpha.OBJ  \
		 $(OUTPUTDIR)\isascii.OBJ  $(OUTPUTDIR)\iscntrl.OBJ  $(OUTPUTDIR)\iscsym.OBJ  $(OUTPUTDIR)\iscsymf.OBJ  \
		 $(OUTPUTDIR)\isdigit.OBJ  $(OUTPUTDIR)\isgraph.OBJ  $(OUTPUTDIR)\islower.OBJ  $(OUTPUTDIR)\isprint.OBJ  \
		 $(OUTPUTDIR)\ispunct.OBJ  $(OUTPUTDIR)\isspace.OBJ  $(OUTPUTDIR)\isupper.OBJ  $(OUTPUTDIR)\isxdigit.OBJ  \
		 $(OUTPUTDIR)\jmp.OBJ  $(OUTPUTDIR)\lconv.OBJ  $(OUTPUTDIR)\lfind.OBJ  $(OUTPUTDIR)\loctime.OBJ  \
		 $(OUTPUTDIR)\log.OBJ  $(OUTPUTDIR)\lsearch.OBJ  $(OUTPUTDIR)\ltoa.OBJ  $(OUTPUTDIR)\mallocf.OBJ  \
		 $(OUTPUTDIR)\matherr.OBJ  $(OUTPUTDIR)\mbctype.OBJ  $(OUTPUTDIR)\mbstowcs.OBJ  $(OUTPUTDIR)\mbstring.OBJ  \
		 $(OUTPUTDIR)\mbyte.OBJ  $(OUTPUTDIR)\memccpy.OBJ  $(OUTPUTDIR)\mktemp.OBJ  $(OUTPUTDIR)\nmsize.OBJ  \
		 $(OUTPUTDIR)\okbigbuf.OBJ  $(OUTPUTDIR)\onexit.OBJ  $(OUTPUTDIR)\perror.OBJ  $(OUTPUTDIR)\poly.OBJ  \
		 $(OUTPUTDIR)\pow.OBJ  $(OUTPUTDIR)\printf.OBJ  $(OUTPUTDIR)\ptrchk.OBJ  $(OUTPUTDIR)\purecall.OBJ  \
		 $(OUTPUTDIR)\putch.OBJ  $(OUTPUTDIR)\putchar.OBJ  $(OUTPUTDIR)\putenv.OBJ  $(OUTPUTDIR)\puts.OBJ  \
		 $(OUTPUTDIR)\qsort.OBJ  $(OUTPUTDIR)\rand.OBJ  $(OUTPUTDIR)\response.OBJ  $(OUTPUTDIR)\rmtmp.OBJ  \
		 $(OUTPUTDIR)\round.OBJ  $(OUTPUTDIR)\sbrk.OBJ  $(OUTPUTDIR)\scanf.OBJ  $(OUTPUTDIR)\setdoser.OBJ  \
		 $(OUTPUTDIR)\setenvp.OBJ  $(OUTPUTDIR)\seterrno.OBJ  $(OUTPUTDIR)\setlc.OBJ  $(OUTPUTDIR)\setmode.OBJ  \
		 $(OUTPUTDIR)\signal.OBJ  $(OUTPUTDIR)\sin.OBJ  $(OUTPUTDIR)\sleep.OBJ  $(OUTPUTDIR)\sopenf.OBJ  \
		 $(OUTPUTDIR)\spawn.OBJ  $(OUTPUTDIR)\spawnl.OBJ  $(OUTPUTDIR)\spawnle.OBJ  $(OUTPUTDIR)\spawnlp.OBJ  \
		 $(OUTPUTDIR)\spawnlpe.OBJ  $(OUTPUTDIR)\spawnv.OBJ  $(OUTPUTDIR)\spawnve.OBJ  $(OUTPUTDIR)\spawnvp.OBJ  \
		 $(OUTPUTDIR)\spawnvpe.OBJ  $(OUTPUTDIR)\splitpat.OBJ  $(OUTPUTDIR)\sqrt.OBJ  $(OUTPUTDIR)\stack.OBJ  \
		 $(OUTPUTDIR)\stat.OBJ  $(OUTPUTDIR)\strcspn.OBJ  $(OUTPUTDIR)\strdup.OBJ  $(OUTPUTDIR)\strerror.OBJ  \
		 $(OUTPUTDIR)\strf.OBJ  $(OUTPUTDIR)\strlwr.OBJ  $(OUTPUTDIR)\strnicmp.OBJ  $(OUTPUTDIR)\strnset.OBJ  \
		 $(OUTPUTDIR)\strpbrk.OBJ  $(OUTPUTDIR)\strrchr.OBJ  $(OUTPUTDIR)\strrev.OBJ  $(OUTPUTDIR)\strset.OBJ  \
		 $(OUTPUTDIR)\strspn.OBJ  $(OUTPUTDIR)\strstr.OBJ  $(OUTPUTDIR)\strtod.OBJ  $(OUTPUTDIR)\strtof.OBJ  \
		 $(OUTPUTDIR)\strtok.OBJ  $(OUTPUTDIR)\strtol.OBJ  $(OUTPUTDIR)\strupr.OBJ  $(OUTPUTDIR)\strxfrm.OBJ  \
		 $(OUTPUTDIR)\swab.OBJ  $(OUTPUTDIR)\system.OBJ  $(OUTPUTDIR)\tabsize.OBJ  $(OUTPUTDIR)\tan.OBJ  \
		 $(OUTPUTDIR)\tchar.OBJ  $(OUTPUTDIR)\time.OBJ  $(OUTPUTDIR)\time3.OBJ  $(OUTPUTDIR)\timesup.OBJ  \
		 $(OUTPUTDIR)\tmpnam.OBJ  $(OUTPUTDIR)\toascii.OBJ  $(OUTPUTDIR)\tolower.OBJ  $(OUTPUTDIR)\toupper.OBJ  \
		 $(OUTPUTDIR)\trigerr.OBJ  $(OUTPUTDIR)\ultoa.OBJ  $(OUTPUTDIR)\umask.OBJ  $(OUTPUTDIR)\ungetc.OBJ  \
		 $(OUTPUTDIR)\unmangle.OBJ  $(OUTPUTDIR)\vprintf.OBJ  $(OUTPUTDIR)\wcscpy.OBJ  $(OUTPUTDIR)\wcslen.OBJ  \
		 $(OUTPUTDIR)\wep.OBJ  $(OUTPUTDIR)\winerr.OBJ  $(OUTPUTDIR)\winmouse.OBJ  $(OUTPUTDIR)\winsleep.OBJ  \
		 $(OUTPUTDIR)\_delete.OBJ  $(OUTPUTDIR)\_delnear.OBJ  $(OUTPUTDIR)\_handler.OBJ  $(OUTPUTDIR)\_new.OBJ  \
		 $(OUTPUTDIR)\_new2.OBJ  $(OUTPUTDIR)\_newnear.OBJ  $(OUTPUTDIR)\complex.OBJ  $(OUTPUTDIR)\coperat.OBJ  \
		 $(OUTPUTDIR)\cstrm.OBJ  $(OUTPUTDIR)\ctrig.OBJ  $(OUTPUTDIR)\dbp.OBJ  $(OUTPUTDIR)\except.OBJ  \
		 $(OUTPUTDIR)\fstream.OBJ  $(OUTPUTDIR)\generr.OBJ  $(OUTPUTDIR)\ios.OBJ  $(OUTPUTDIR)\iostream.OBJ  \
		 $(OUTPUTDIR)\istream.OBJ  $(OUTPUTDIR)\istreamf.OBJ  $(OUTPUTDIR)\istreami.OBJ  $(OUTPUTDIR)\istreamx.OBJ  \
		 $(OUTPUTDIR)\manip.OBJ  $(OUTPUTDIR)\ostream.OBJ  $(OUTPUTDIR)\ostreamf.OBJ  $(OUTPUTDIR)\ostreami.OBJ  \
		 $(OUTPUTDIR)\ostreamx.OBJ  $(OUTPUTDIR)\rtti.OBJ  $(OUTPUTDIR)\siosync.OBJ  $(OUTPUTDIR)\stdiostr.OBJ  \
		 $(OUTPUTDIR)\stream.OBJ  $(OUTPUTDIR)\streambf.OBJ  $(OUTPUTDIR)\strstrea.OBJ  $(OUTPUTDIR)\typeinfo.OBJ  \
		 $(OUTPUTDIR)\vecnew.OBJ  $(OUTPUTDIR)\vector.OBJ  $(OUTPUTDIR)\scver.OBJ  $(OUTPUTDIR)\libentry.OBJ  \
		 $(OUTPUTDIR)\_alloca.OBJ  $(OUTPUTDIR)\alloca.OBJ  $(OUTPUTDIR)\mschkstk.OBJ  $(OUTPUTDIR)\anew.OBJ  \
		 $(OUTPUTDIR)\anew2.OBJ  $(OUTPUTDIR)\adelete.OBJ  $(OUTPUTDIR)\corenew.OBJ  $(OUTPUTDIR)\purevirt.OBJ  \
		 $(OUTPUTDIR)\ehdata.OBJ  $(OUTPUTDIR)\_ISMBSLD.OBJ  $(OUTPUTDIR)\_ISMBSTL.OBJ  $(OUTPUTDIR)\ISMBALNU.OBJ  \
		 $(OUTPUTDIR)\ISMBALPH.OBJ  $(OUTPUTDIR)\ISMBBYTE.OBJ  $(OUTPUTDIR)\ISMBCL0.OBJ  $(OUTPUTDIR)\ISMBCL1.OBJ  \
		 $(OUTPUTDIR)\ISMBCL2.OBJ  $(OUTPUTDIR)\ISMBDIGI.OBJ  $(OUTPUTDIR)\ISMBGRAP.OBJ  $(OUTPUTDIR)\ISMBHIRA.OBJ  \
		 $(OUTPUTDIR)\ISMBKATA.OBJ  $(OUTPUTDIR)\ISMBLEGA.OBJ  $(OUTPUTDIR)\ISMBLOWE.OBJ  $(OUTPUTDIR)\ISMBPRIN.OBJ  \
		 $(OUTPUTDIR)\ISMBPUNC.OBJ  $(OUTPUTDIR)\ISMBSPAC.OBJ  $(OUTPUTDIR)\ISMBSYMB.OBJ  $(OUTPUTDIR)\ISMBUPPE.OBJ  \
		 $(OUTPUTDIR)\MBBTOMBC.OBJ  $(OUTPUTDIR)\MBBTYPE.OBJ  $(OUTPUTDIR)\MBCCPY.OBJ  $(OUTPUTDIR)\MBCLEN.OBJ  \
		 $(OUTPUTDIR)\MBCPINFO.OBJ  $(OUTPUTDIR)\MBJISJMS.OBJ  $(OUTPUTDIR)\MBJMSJIS.OBJ  $(OUTPUTDIR)\MBLEN.OBJ  \
		 $(OUTPUTDIR)\MBSBTYPE.OBJ  $(OUTPUTDIR)\MBSCHR.OBJ  $(OUTPUTDIR)\MBSCMP.OBJ  $(OUTPUTDIR)\MBSCOLL.OBJ  \
		 $(OUTPUTDIR)\MBSCSPN.OBJ  $(OUTPUTDIR)\MBSDEC.OBJ  $(OUTPUTDIR)\MBSICMP.OBJ  $(OUTPUTDIR)\MBSICOLL.OBJ  \
		 $(OUTPUTDIR)\MBSINC.OBJ  $(OUTPUTDIR)\MBSLEN.OBJ  $(OUTPUTDIR)\MBSLWR.OBJ  $(OUTPUTDIR)\MBSNBCAT.OBJ  \
		 $(OUTPUTDIR)\MBSNBCMP.OBJ  $(OUTPUTDIR)\MBSNBCNT.OBJ  $(OUTPUTDIR)\MBSNBCOL.OBJ  $(OUTPUTDIR)\MBSNBCPY.OBJ  \
		 $(OUTPUTDIR)\MBSNBICM.OBJ  $(OUTPUTDIR)\MBSNBICO.OBJ  $(OUTPUTDIR)\MBSNBSET.OBJ  $(OUTPUTDIR)\MBSNCAT.OBJ  \
		 $(OUTPUTDIR)\MBSNCCNT.OBJ  $(OUTPUTDIR)\MBSNCMP.OBJ  $(OUTPUTDIR)\MBSNCOLL.OBJ  $(OUTPUTDIR)\MBSNCPY.OBJ  \
		 $(OUTPUTDIR)\MBSNEXTC.OBJ  $(OUTPUTDIR)\MBSNICMP.OBJ  $(OUTPUTDIR)\MBSNICOL.OBJ  $(OUTPUTDIR)\MBSNINC.OBJ  \
		 $(OUTPUTDIR)\MBSNSET.OBJ  $(OUTPUTDIR)\MBSPBRK.OBJ  $(OUTPUTDIR)\MBSRCHR.OBJ  $(OUTPUTDIR)\MBSREV.OBJ  \
		 $(OUTPUTDIR)\MBSSET.OBJ  $(OUTPUTDIR)\MBSSPN.OBJ  $(OUTPUTDIR)\MBSSPNP.OBJ  $(OUTPUTDIR)\MBSSTR.OBJ  \
		 $(OUTPUTDIR)\MBSTOK.OBJ  $(OUTPUTDIR)\MBSTRLEN.OBJ  $(OUTPUTDIR)\MBSUPR.OBJ  $(OUTPUTDIR)\MBTOHIRA.OBJ  \
		 $(OUTPUTDIR)\MBTOKATA.OBJ  $(OUTPUTDIR)\MBTOLOWE.OBJ  $(OUTPUTDIR)\MBTOUPPE.OBJ  $(OUTPUTDIR)\initmbcp.OBJ  \
		 $(OUTPUTDIR)\setmbcp.OBJ 

RCFILES		= 

RESFILES	= 

SYMS		= 

HELPFILES	= 

PROJS		= swindos.prj 

BATS		= 

PRECOMPILE	=  C?C?Bsource?B75?BRTL?Bsrc?Bbuild?Bswindos?Pprj 

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

.OBJ.EXE:
	$(LNK) $(LFLAGS) @$(PROJ).LNK

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



all:	createdir $(PRECOMPILE) $(SYMS) $(OBJS) $(INCLUDEDOBJS) $(POSTCOMPILE) $(TARGETDIR)\$(PROJ).$(PROJTYPE) $(POSTLINK) _done

createdir:
	$(CREATEOUTPUTDIR)
	$(CREATETARGETDIR)

$(TARGETDIR)\$(PROJ).$(PROJTYPE): $(OBJS) $(INCLUDEDOBJS) $(RCFILES) $(RESFILES) $(HELPFILES) 
			-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		LIB $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(LIBFLAGS)\
				@<<
 +$(OUTPUTDIR)\_8087.OBJ +$(OUTPUTDIR)\_afulshr.OBJ +$(OUTPUTDIR)\_anlshl.OBJ &
		 +$(OUTPUTDIR)\_anulshr.OBJ +$(OUTPUTDIR)\_fmemicm.OBJ +$(OUTPUTDIR)\_ptrchk.OBJ +$(OUTPUTDIR)\amath.OBJ &
		 +$(OUTPUTDIR)\atoi.OBJ +$(OUTPUTDIR)\atol.OBJ +$(OUTPUTDIR)\baslnk.OBJ +$(OUTPUTDIR)\bios.OBJ &
		 +$(OUTPUTDIR)\calloc.OBJ +$(OUTPUTDIR)\cerror.OBJ +$(OUTPUTDIR)\chkstk.OBJ +$(OUTPUTDIR)\cinit.OBJ &
		 +$(OUTPUTDIR)\clock.OBJ +$(OUTPUTDIR)\controlc.OBJ +$(OUTPUTDIR)\country.OBJ +$(OUTPUTDIR)\cputype.OBJ &
		 +$(OUTPUTDIR)\cw.OBJ +$(OUTPUTDIR)\dclass.OBJ +$(OUTPUTDIR)\disp.OBJ +$(OUTPUTDIR)\dllstart.OBJ &
		 +$(OUTPUTDIR)\dos_find.OBJ +$(OUTPUTDIR)\dosalloc.OBJ +$(OUTPUTDIR)\dosdate.OBJ +$(OUTPUTDIR)\doserr.OBJ &
		 +$(OUTPUTDIR)\dosfil.OBJ +$(OUTPUTDIR)\double.OBJ +$(OUTPUTDIR)\dpmi.OBJ +$(OUTPUTDIR)\dpmimin.OBJ &
		 +$(OUTPUTDIR)\dup.OBJ +$(OUTPUTDIR)\ehasm.OBJ +$(OUTPUTDIR)\emm.OBJ +$(OUTPUTDIR)\exec2.OBJ &
		 +$(OUTPUTDIR)\farptr.OBJ +$(OUTPUTDIR)\fcalloc.OBJ +$(OUTPUTDIR)\filelen.OBJ +$(OUTPUTDIR)\find.OBJ &
		 +$(OUTPUTDIR)\float.OBJ +$(OUTPUTDIR)\floattyp.OBJ +$(OUTPUTDIR)\flt87.OBJ +$(OUTPUTDIR)\fmalloc.OBJ &
		 +$(OUTPUTDIR)\fmemchr.OBJ +$(OUTPUTDIR)\fmemcmp.OBJ +$(OUTPUTDIR)\fmemcpy.OBJ +$(OUTPUTDIR)\fmemicmp.OBJ &
		 +$(OUTPUTDIR)\fmemset.OBJ +$(OUTPUTDIR)\fpatan16.OBJ +$(OUTPUTDIR)\fprem16.OBJ +$(OUTPUTDIR)\fptan16.OBJ &
		 +$(OUTPUTDIR)\fputc.OBJ +$(OUTPUTDIR)\fqq.OBJ +$(OUTPUTDIR)\frealloc.OBJ +$(OUTPUTDIR)\free.OBJ &
		 +$(OUTPUTDIR)\fstrcat.OBJ +$(OUTPUTDIR)\fstrchr.OBJ +$(OUTPUTDIR)\fstrcmp.OBJ +$(OUTPUTDIR)\fstrcpy.OBJ &
		 +$(OUTPUTDIR)\fstricmp.OBJ +$(OUTPUTDIR)\fstrlen.OBJ +$(OUTPUTDIR)\fstrncat.OBJ +$(OUTPUTDIR)\fstrncmp.OBJ &
		 +$(OUTPUTDIR)\fstrncpy.OBJ +$(OUTPUTDIR)\getcwd.OBJ +$(OUTPUTDIR)\getdos.OBJ +$(OUTPUTDIR)\getenv.OBJ &
		 +$(OUTPUTDIR)\getthd.OBJ +$(OUTPUTDIR)\handle.OBJ +$(OUTPUTDIR)\hdiff.OBJ +$(OUTPUTDIR)\hugeptr.OBJ &
		 +$(OUTPUTDIR)\inter.OBJ +$(OUTPUTDIR)\io.OBJ +$(OUTPUTDIR)\itoa.OBJ +$(OUTPUTDIR)\kbhit.OBJ &
		 +$(OUTPUTDIR)\ldexp.OBJ +$(OUTPUTDIR)\lmath.OBJ +$(OUTPUTDIR)\locking.OBJ +$(OUTPUTDIR)\lseek.OBJ &
		 +$(OUTPUTDIR)\malloc.OBJ +$(OUTPUTDIR)\math87.OBJ +$(OUTPUTDIR)\mathtrn.OBJ +$(OUTPUTDIR)\mathtrnf.OBJ &
		 +$(OUTPUTDIR)\memavl.OBJ +$(OUTPUTDIR)\memchr.OBJ +$(OUTPUTDIR)\memcmp.OBJ +$(OUTPUTDIR)\memcpy.OBJ &
		 +$(OUTPUTDIR)\memicmp.OBJ +$(OUTPUTDIR)\memmax.OBJ +$(OUTPUTDIR)\memset.OBJ +$(OUTPUTDIR)\movedata.OBJ &
		 +$(OUTPUTDIR)\msize.OBJ +$(OUTPUTDIR)\ncalloc.OBJ +$(OUTPUTDIR)\nmalloc.OBJ +$(OUTPUTDIR)\nrealloc.OBJ &
		 +$(OUTPUTDIR)\page.OBJ +$(OUTPUTDIR)\parall.OBJ +$(OUTPUTDIR)\patch16.OBJ +$(OUTPUTDIR)\peek.OBJ &
		 +$(OUTPUTDIR)\port.OBJ +$(OUTPUTDIR)\read.OBJ +$(OUTPUTDIR)\realloc.OBJ +$(OUTPUTDIR)\rot.OBJ &
		 +$(OUTPUTDIR)\seg.OBJ +$(OUTPUTDIR)\setargv.OBJ +$(OUTPUTDIR)\sound.OBJ +$(OUTPUTDIR)\stkovfl.OBJ &
		 +$(OUTPUTDIR)\stpcpy.OBJ +$(OUTPUTDIR)\strcat.OBJ +$(OUTPUTDIR)\strchr.OBJ +$(OUTPUTDIR)\strcmp.OBJ &
		 +$(OUTPUTDIR)\strcmpl.OBJ +$(OUTPUTDIR)\strcpy.OBJ +$(OUTPUTDIR)\strlen.OBJ +$(OUTPUTDIR)\strncat.OBJ &
		 +$(OUTPUTDIR)\strncmp.OBJ +$(OUTPUTDIR)\strncpy.OBJ +$(OUTPUTDIR)\strtoul.OBJ +$(OUTPUTDIR)\sysint.OBJ &
		 +$(OUTPUTDIR)\utime.OBJ +$(OUTPUTDIR)\wincio.OBJ +$(OUTPUTDIR)\write.OBJ +$(OUTPUTDIR)\_afftol.OBJ &
		 +$(OUTPUTDIR)\_afulrem.OBJ +$(OUTPUTDIR)\_dup.OBJ +$(OUTPUTDIR)\_fcvt.OBJ +$(OUTPUTDIR)\_fgtchar.OBJ &
		 +$(OUTPUTDIR)\_fheap.OBJ +$(OUTPUTDIR)\_fmemmve.OBJ +$(OUTPUTDIR)\_fmemset.OBJ +$(OUTPUTDIR)\_fopen.OBJ &
		 +$(OUTPUTDIR)\_fptchar.OBJ +$(OUTPUTDIR)\_fstrchr.OBJ +$(OUTPUTDIR)\_fstrtok.OBJ +$(OUTPUTDIR)\_ismbbld.OBJ &
		 +$(OUTPUTDIR)\_ismbbtl.OBJ +$(OUTPUTDIR)\_itoa.OBJ +$(OUTPUTDIR)\_ltoa.OBJ +$(OUTPUTDIR)\_main.OBJ &
		 +$(OUTPUTDIR)\_memicmp.OBJ +$(OUTPUTDIR)\_nstrdup.OBJ +$(OUTPUTDIR)\_printf.OBJ +$(OUTPUTDIR)\_strdup.OBJ &
		 +$(OUTPUTDIR)\_strerro.OBJ +$(OUTPUTDIR)\_strrev.OBJ +$(OUTPUTDIR)\_tzset.OBJ +$(OUTPUTDIR)\_wep.OBJ &
		 +$(OUTPUTDIR)\allocmem.OBJ +$(OUTPUTDIR)\arcerr.OBJ +$(OUTPUTDIR)\asctime.OBJ +$(OUTPUTDIR)\asin.OBJ &
		 +$(OUTPUTDIR)\assert.OBJ +$(OUTPUTDIR)\atan.OBJ +$(OUTPUTDIR)\atexit.OBJ +$(OUTPUTDIR)\atof.OBJ &
		 +$(OUTPUTDIR)\biosc.OBJ +$(OUTPUTDIR)\buildenv.OBJ +$(OUTPUTDIR)\cgets.OBJ +$(OUTPUTDIR)\charmax.OBJ &
		 +$(OUTPUTDIR)\chsize.OBJ +$(OUTPUTDIR)\commit.OBJ +$(OUTPUTDIR)\coreleft.OBJ +$(OUTPUTDIR)\cpphook.OBJ &
		 +$(OUTPUTDIR)\cprintf.OBJ +$(OUTPUTDIR)\cputs.OBJ +$(OUTPUTDIR)\creat.OBJ +$(OUTPUTDIR)\cscanf.OBJ &
		 +$(OUTPUTDIR)\ctime.OBJ +$(OUTPUTDIR)\ctrlbrk.OBJ +$(OUTPUTDIR)\ctype.OBJ +$(OUTPUTDIR)\disk.OBJ &
		 +$(OUTPUTDIR)\diskfree.OBJ +$(OUTPUTDIR)\disp2.OBJ +$(OUTPUTDIR)\doscomp.OBJ +$(OUTPUTDIR)\doskeep.OBJ &
		 +$(OUTPUTDIR)\doslock.OBJ +$(OUTPUTDIR)\dosseek.OBJ +$(OUTPUTDIR)\dprintf.OBJ +$(OUTPUTDIR)\ecvt.OBJ &
		 +$(OUTPUTDIR)\eof.OBJ +$(OUTPUTDIR)\execl.OBJ +$(OUTPUTDIR)\execle.OBJ +$(OUTPUTDIR)\execlp.OBJ &
		 +$(OUTPUTDIR)\execlpe.OBJ +$(OUTPUTDIR)\execv.OBJ +$(OUTPUTDIR)\execve.OBJ +$(OUTPUTDIR)\execvp.OBJ &
		 +$(OUTPUTDIR)\execvpe.OBJ +$(OUTPUTDIR)\exit.OBJ +$(OUTPUTDIR)\exitstat.OBJ +$(OUTPUTDIR)\exp.OBJ &
		 +$(OUTPUTDIR)\far.OBJ +$(OUTPUTDIR)\fatexit.OBJ +$(OUTPUTDIR)\fclose.OBJ +$(OUTPUTDIR)\fcloseal.OBJ &
		 +$(OUTPUTDIR)\fcvt.OBJ +$(OUTPUTDIR)\fdivp.OBJ +$(OUTPUTDIR)\fdopen.OBJ +$(OUTPUTDIR)\feexcept.OBJ &
		 +$(OUTPUTDIR)\fflush.OBJ +$(OUTPUTDIR)\fgetchar.OBJ +$(OUTPUTDIR)\fgets.OBJ +$(OUTPUTDIR)\floatcvt.OBJ &
		 +$(OUTPUTDIR)\fltenv.OBJ +$(OUTPUTDIR)\fltused.OBJ +$(OUTPUTDIR)\flush.OBJ +$(OUTPUTDIR)\flushall.OBJ &
		 +$(OUTPUTDIR)\fmacros.OBJ +$(OUTPUTDIR)\fmbyte.OBJ +$(OUTPUTDIR)\fmsize.OBJ +$(OUTPUTDIR)\fmt.OBJ &
		 +$(OUTPUTDIR)\fonexit.OBJ +$(OUTPUTDIR)\fopen.OBJ +$(OUTPUTDIR)\fpmath.OBJ +$(OUTPUTDIR)\fpsig.OBJ &
		 +$(OUTPUTDIR)\fputchar.OBJ +$(OUTPUTDIR)\fputs.OBJ +$(OUTPUTDIR)\fread.OBJ +$(OUTPUTDIR)\freect.OBJ &
		 +$(OUTPUTDIR)\freopen.OBJ +$(OUTPUTDIR)\fsopen.OBJ +$(OUTPUTDIR)\fstrcspn.OBJ +$(OUTPUTDIR)\fstrdup.OBJ &
		 +$(OUTPUTDIR)\fstrlwr.OBJ +$(OUTPUTDIR)\fstrnicm.OBJ +$(OUTPUTDIR)\fstrnset.OBJ +$(OUTPUTDIR)\fstrpbrk.OBJ &
		 +$(OUTPUTDIR)\fstrrchr.OBJ +$(OUTPUTDIR)\fstrrev.OBJ +$(OUTPUTDIR)\fstrset.OBJ +$(OUTPUTDIR)\fstrspn.OBJ &
		 +$(OUTPUTDIR)\fstrstr.OBJ +$(OUTPUTDIR)\fstrtok.OBJ +$(OUTPUTDIR)\fstrupr.OBJ +$(OUTPUTDIR)\ftime.OBJ &
		 +$(OUTPUTDIR)\fwrite.OBJ +$(OUTPUTDIR)\gcvt.OBJ +$(OUTPUTDIR)\getchar.OBJ +$(OUTPUTDIR)\getdate.OBJ &
		 +$(OUTPUTDIR)\getpid.OBJ +$(OUTPUTDIR)\gets.OBJ +$(OUTPUTDIR)\heap.OBJ +$(OUTPUTDIR)\hyper.OBJ &
		 +$(OUTPUTDIR)\ieee.OBJ +$(OUTPUTDIR)\iob.OBJ +$(OUTPUTDIR)\isalnum.OBJ +$(OUTPUTDIR)\isalpha.OBJ &
		 +$(OUTPUTDIR)\isascii.OBJ +$(OUTPUTDIR)\iscntrl.OBJ +$(OUTPUTDIR)\iscsym.OBJ +$(OUTPUTDIR)\iscsymf.OBJ &
		 +$(OUTPUTDIR)\isdigit.OBJ +$(OUTPUTDIR)\isgraph.OBJ +$(OUTPUTDIR)\islower.OBJ +$(OUTPUTDIR)\isprint.OBJ &
		 +$(OUTPUTDIR)\ispunct.OBJ +$(OUTPUTDIR)\isspace.OBJ +$(OUTPUTDIR)\isupper.OBJ +$(OUTPUTDIR)\isxdigit.OBJ &
		 +$(OUTPUTDIR)\jmp.OBJ +$(OUTPUTDIR)\lconv.OBJ +$(OUTPUTDIR)\lfind.OBJ +$(OUTPUTDIR)\loctime.OBJ &
		 +$(OUTPUTDIR)\log.OBJ +$(OUTPUTDIR)\lsearch.OBJ +$(OUTPUTDIR)\ltoa.OBJ +$(OUTPUTDIR)\mallocf.OBJ &
		 +$(OUTPUTDIR)\matherr.OBJ +$(OUTPUTDIR)\mbctype.OBJ +$(OUTPUTDIR)\mbstowcs.OBJ +$(OUTPUTDIR)\mbstring.OBJ &
		 +$(OUTPUTDIR)\mbyte.OBJ +$(OUTPUTDIR)\memccpy.OBJ +$(OUTPUTDIR)\mktemp.OBJ +$(OUTPUTDIR)\nmsize.OBJ &
		 +$(OUTPUTDIR)\okbigbuf.OBJ +$(OUTPUTDIR)\onexit.OBJ +$(OUTPUTDIR)\perror.OBJ +$(OUTPUTDIR)\poly.OBJ &
		 +$(OUTPUTDIR)\pow.OBJ +$(OUTPUTDIR)\printf.OBJ +$(OUTPUTDIR)\ptrchk.OBJ +$(OUTPUTDIR)\purecall.OBJ &
		 +$(OUTPUTDIR)\putch.OBJ +$(OUTPUTDIR)\putchar.OBJ +$(OUTPUTDIR)\putenv.OBJ +$(OUTPUTDIR)\puts.OBJ &
		 +$(OUTPUTDIR)\qsort.OBJ +$(OUTPUTDIR)\rand.OBJ +$(OUTPUTDIR)\response.OBJ +$(OUTPUTDIR)\rmtmp.OBJ &
		 +$(OUTPUTDIR)\round.OBJ +$(OUTPUTDIR)\sbrk.OBJ +$(OUTPUTDIR)\scanf.OBJ +$(OUTPUTDIR)\setdoser.OBJ &
		 +$(OUTPUTDIR)\setenvp.OBJ +$(OUTPUTDIR)\seterrno.OBJ +$(OUTPUTDIR)\setlc.OBJ +$(OUTPUTDIR)\setmode.OBJ &
		 +$(OUTPUTDIR)\signal.OBJ +$(OUTPUTDIR)\sin.OBJ +$(OUTPUTDIR)\sleep.OBJ +$(OUTPUTDIR)\sopenf.OBJ &
		 +$(OUTPUTDIR)\spawn.OBJ +$(OUTPUTDIR)\spawnl.OBJ +$(OUTPUTDIR)\spawnle.OBJ +$(OUTPUTDIR)\spawnlp.OBJ &
		 +$(OUTPUTDIR)\spawnlpe.OBJ +$(OUTPUTDIR)\spawnv.OBJ +$(OUTPUTDIR)\spawnve.OBJ +$(OUTPUTDIR)\spawnvp.OBJ &
		 +$(OUTPUTDIR)\spawnvpe.OBJ +$(OUTPUTDIR)\splitpat.OBJ +$(OUTPUTDIR)\sqrt.OBJ +$(OUTPUTDIR)\stack.OBJ &
		 +$(OUTPUTDIR)\stat.OBJ +$(OUTPUTDIR)\strcspn.OBJ +$(OUTPUTDIR)\strdup.OBJ +$(OUTPUTDIR)\strerror.OBJ &
		 +$(OUTPUTDIR)\strf.OBJ +$(OUTPUTDIR)\strlwr.OBJ +$(OUTPUTDIR)\strnicmp.OBJ +$(OUTPUTDIR)\strnset.OBJ &
		 +$(OUTPUTDIR)\strpbrk.OBJ +$(OUTPUTDIR)\strrchr.OBJ +$(OUTPUTDIR)\strrev.OBJ +$(OUTPUTDIR)\strset.OBJ &
		 +$(OUTPUTDIR)\strspn.OBJ +$(OUTPUTDIR)\strstr.OBJ +$(OUTPUTDIR)\strtod.OBJ +$(OUTPUTDIR)\strtof.OBJ &
		 +$(OUTPUTDIR)\strtok.OBJ +$(OUTPUTDIR)\strtol.OBJ +$(OUTPUTDIR)\strupr.OBJ +$(OUTPUTDIR)\strxfrm.OBJ &
		 +$(OUTPUTDIR)\swab.OBJ +$(OUTPUTDIR)\system.OBJ +$(OUTPUTDIR)\tabsize.OBJ +$(OUTPUTDIR)\tan.OBJ &
		 +$(OUTPUTDIR)\tchar.OBJ +$(OUTPUTDIR)\time.OBJ +$(OUTPUTDIR)\time3.OBJ +$(OUTPUTDIR)\timesup.OBJ &
		 +$(OUTPUTDIR)\tmpnam.OBJ +$(OUTPUTDIR)\toascii.OBJ +$(OUTPUTDIR)\tolower.OBJ +$(OUTPUTDIR)\toupper.OBJ &
		 +$(OUTPUTDIR)\trigerr.OBJ +$(OUTPUTDIR)\ultoa.OBJ +$(OUTPUTDIR)\umask.OBJ +$(OUTPUTDIR)\ungetc.OBJ &
		 +$(OUTPUTDIR)\unmangle.OBJ +$(OUTPUTDIR)\vprintf.OBJ +$(OUTPUTDIR)\wcscpy.OBJ +$(OUTPUTDIR)\wcslen.OBJ &
		 +$(OUTPUTDIR)\wep.OBJ +$(OUTPUTDIR)\winerr.OBJ +$(OUTPUTDIR)\winmouse.OBJ +$(OUTPUTDIR)\winsleep.OBJ &
		 +$(OUTPUTDIR)\_delete.OBJ +$(OUTPUTDIR)\_delnear.OBJ +$(OUTPUTDIR)\_handler.OBJ +$(OUTPUTDIR)\_new.OBJ &
		 +$(OUTPUTDIR)\_new2.OBJ +$(OUTPUTDIR)\_newnear.OBJ +$(OUTPUTDIR)\complex.OBJ +$(OUTPUTDIR)\coperat.OBJ &
		 +$(OUTPUTDIR)\cstrm.OBJ +$(OUTPUTDIR)\ctrig.OBJ +$(OUTPUTDIR)\dbp.OBJ +$(OUTPUTDIR)\except.OBJ &
		 +$(OUTPUTDIR)\fstream.OBJ +$(OUTPUTDIR)\generr.OBJ +$(OUTPUTDIR)\ios.OBJ +$(OUTPUTDIR)\iostream.OBJ &
		 +$(OUTPUTDIR)\istream.OBJ +$(OUTPUTDIR)\istreamf.OBJ +$(OUTPUTDIR)\istreami.OBJ +$(OUTPUTDIR)\istreamx.OBJ &
		 +$(OUTPUTDIR)\manip.OBJ +$(OUTPUTDIR)\ostream.OBJ +$(OUTPUTDIR)\ostreamf.OBJ +$(OUTPUTDIR)\ostreami.OBJ &
		 +$(OUTPUTDIR)\ostreamx.OBJ +$(OUTPUTDIR)\rtti.OBJ +$(OUTPUTDIR)\siosync.OBJ +$(OUTPUTDIR)\stdiostr.OBJ &
		 +$(OUTPUTDIR)\stream.OBJ +$(OUTPUTDIR)\streambf.OBJ +$(OUTPUTDIR)\strstrea.OBJ +$(OUTPUTDIR)\typeinfo.OBJ &
		 +$(OUTPUTDIR)\vecnew.OBJ +$(OUTPUTDIR)\vector.OBJ +$(OUTPUTDIR)\scver.OBJ +$(OUTPUTDIR)\libentry.OBJ &
		 +$(OUTPUTDIR)\_alloca.OBJ +$(OUTPUTDIR)\alloca.OBJ +$(OUTPUTDIR)\mschkstk.OBJ +$(OUTPUTDIR)\anew.OBJ &
		 +$(OUTPUTDIR)\anew2.OBJ +$(OUTPUTDIR)\adelete.OBJ +$(OUTPUTDIR)\corenew.OBJ +$(OUTPUTDIR)\purevirt.OBJ &
		 +$(OUTPUTDIR)\ehdata.OBJ +$(OUTPUTDIR)\_ISMBSLD.OBJ +$(OUTPUTDIR)\_ISMBSTL.OBJ +$(OUTPUTDIR)\ISMBALNU.OBJ &
		 +$(OUTPUTDIR)\ISMBALPH.OBJ +$(OUTPUTDIR)\ISMBBYTE.OBJ +$(OUTPUTDIR)\ISMBCL0.OBJ +$(OUTPUTDIR)\ISMBCL1.OBJ &
		 +$(OUTPUTDIR)\ISMBCL2.OBJ +$(OUTPUTDIR)\ISMBDIGI.OBJ +$(OUTPUTDIR)\ISMBGRAP.OBJ +$(OUTPUTDIR)\ISMBHIRA.OBJ &
		 +$(OUTPUTDIR)\ISMBKATA.OBJ +$(OUTPUTDIR)\ISMBLEGA.OBJ +$(OUTPUTDIR)\ISMBLOWE.OBJ +$(OUTPUTDIR)\ISMBPRIN.OBJ &
		 +$(OUTPUTDIR)\ISMBPUNC.OBJ +$(OUTPUTDIR)\ISMBSPAC.OBJ +$(OUTPUTDIR)\ISMBSYMB.OBJ +$(OUTPUTDIR)\ISMBUPPE.OBJ &
		 +$(OUTPUTDIR)\MBBTOMBC.OBJ +$(OUTPUTDIR)\MBBTYPE.OBJ +$(OUTPUTDIR)\MBCCPY.OBJ +$(OUTPUTDIR)\MBCLEN.OBJ &
		 +$(OUTPUTDIR)\MBCPINFO.OBJ +$(OUTPUTDIR)\MBJISJMS.OBJ +$(OUTPUTDIR)\MBJMSJIS.OBJ +$(OUTPUTDIR)\MBLEN.OBJ &
		 +$(OUTPUTDIR)\MBSBTYPE.OBJ +$(OUTPUTDIR)\MBSCHR.OBJ +$(OUTPUTDIR)\MBSCMP.OBJ +$(OUTPUTDIR)\MBSCOLL.OBJ &
		 +$(OUTPUTDIR)\MBSCSPN.OBJ +$(OUTPUTDIR)\MBSDEC.OBJ +$(OUTPUTDIR)\MBSICMP.OBJ +$(OUTPUTDIR)\MBSICOLL.OBJ &
		 +$(OUTPUTDIR)\MBSINC.OBJ +$(OUTPUTDIR)\MBSLEN.OBJ +$(OUTPUTDIR)\MBSLWR.OBJ +$(OUTPUTDIR)\MBSNBCAT.OBJ &
		 +$(OUTPUTDIR)\MBSNBCMP.OBJ +$(OUTPUTDIR)\MBSNBCNT.OBJ +$(OUTPUTDIR)\MBSNBCOL.OBJ +$(OUTPUTDIR)\MBSNBCPY.OBJ &
		 +$(OUTPUTDIR)\MBSNBICM.OBJ +$(OUTPUTDIR)\MBSNBICO.OBJ +$(OUTPUTDIR)\MBSNBSET.OBJ +$(OUTPUTDIR)\MBSNCAT.OBJ &
		 +$(OUTPUTDIR)\MBSNCCNT.OBJ +$(OUTPUTDIR)\MBSNCMP.OBJ +$(OUTPUTDIR)\MBSNCOLL.OBJ +$(OUTPUTDIR)\MBSNCPY.OBJ &
		 +$(OUTPUTDIR)\MBSNEXTC.OBJ +$(OUTPUTDIR)\MBSNICMP.OBJ +$(OUTPUTDIR)\MBSNICOL.OBJ +$(OUTPUTDIR)\MBSNINC.OBJ &
		 +$(OUTPUTDIR)\MBSNSET.OBJ +$(OUTPUTDIR)\MBSPBRK.OBJ +$(OUTPUTDIR)\MBSRCHR.OBJ +$(OUTPUTDIR)\MBSREV.OBJ &
		 +$(OUTPUTDIR)\MBSSET.OBJ +$(OUTPUTDIR)\MBSSPN.OBJ +$(OUTPUTDIR)\MBSSPNP.OBJ +$(OUTPUTDIR)\MBSSTR.OBJ &
		 +$(OUTPUTDIR)\MBSTOK.OBJ +$(OUTPUTDIR)\MBSTRLEN.OBJ +$(OUTPUTDIR)\MBSUPR.OBJ +$(OUTPUTDIR)\MBTOHIRA.OBJ &
		 +$(OUTPUTDIR)\MBTOKATA.OBJ +$(OUTPUTDIR)\MBTOLOWE.OBJ +$(OUTPUTDIR)\MBTOUPPE.OBJ +$(OUTPUTDIR)\initmbcp.OBJ &
		 +$(OUTPUTDIR)\setmbcp.OBJ
<<
			-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)
			-echo $(TARGETDIR)\$(PROJ).$(PROJTYPE) built

_done:
		-echo $(PROJ).$(PROJTYPE) done

buildall:	clean	all


clean:
		-del $(TARGETDIR)\$$SCW$$.$(PROJTYPE)
		-del $(TARGETDIR)\$(PROJ).CLE
		-del $(OUTPUTDIR)\SCPH.SYM
		-del $(OBJS)
		$(MAKE) -fswindos.mak $(MFLAGS) "SUB_DEBUG=$(DEBUG)" $@

cleanres:

res:		cleanres $(RCFILES) all


link:
		LIB $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(LIBFLAGS)\
				@<<
 +$(OUTPUTDIR)\_8087.OBJ +$(OUTPUTDIR)\_afulshr.OBJ +$(OUTPUTDIR)\_anlshl.OBJ &
		 +$(OUTPUTDIR)\_anulshr.OBJ +$(OUTPUTDIR)\_fmemicm.OBJ +$(OUTPUTDIR)\_ptrchk.OBJ +$(OUTPUTDIR)\amath.OBJ &
		 +$(OUTPUTDIR)\atoi.OBJ +$(OUTPUTDIR)\atol.OBJ +$(OUTPUTDIR)\baslnk.OBJ +$(OUTPUTDIR)\bios.OBJ &
		 +$(OUTPUTDIR)\calloc.OBJ +$(OUTPUTDIR)\cerror.OBJ +$(OUTPUTDIR)\chkstk.OBJ +$(OUTPUTDIR)\cinit.OBJ &
		 +$(OUTPUTDIR)\clock.OBJ +$(OUTPUTDIR)\controlc.OBJ +$(OUTPUTDIR)\country.OBJ +$(OUTPUTDIR)\cputype.OBJ &
		 +$(OUTPUTDIR)\cw.OBJ +$(OUTPUTDIR)\dclass.OBJ +$(OUTPUTDIR)\disp.OBJ +$(OUTPUTDIR)\dllstart.OBJ &
		 +$(OUTPUTDIR)\dos_find.OBJ +$(OUTPUTDIR)\dosalloc.OBJ +$(OUTPUTDIR)\dosdate.OBJ +$(OUTPUTDIR)\doserr.OBJ &
		 +$(OUTPUTDIR)\dosfil.OBJ +$(OUTPUTDIR)\double.OBJ +$(OUTPUTDIR)\dpmi.OBJ +$(OUTPUTDIR)\dpmimin.OBJ &
		 +$(OUTPUTDIR)\dup.OBJ +$(OUTPUTDIR)\ehasm.OBJ +$(OUTPUTDIR)\emm.OBJ +$(OUTPUTDIR)\exec2.OBJ &
		 +$(OUTPUTDIR)\farptr.OBJ +$(OUTPUTDIR)\fcalloc.OBJ +$(OUTPUTDIR)\filelen.OBJ +$(OUTPUTDIR)\find.OBJ &
		 +$(OUTPUTDIR)\float.OBJ +$(OUTPUTDIR)\floattyp.OBJ +$(OUTPUTDIR)\flt87.OBJ +$(OUTPUTDIR)\fmalloc.OBJ &
		 +$(OUTPUTDIR)\fmemchr.OBJ +$(OUTPUTDIR)\fmemcmp.OBJ +$(OUTPUTDIR)\fmemcpy.OBJ +$(OUTPUTDIR)\fmemicmp.OBJ &
		 +$(OUTPUTDIR)\fmemset.OBJ +$(OUTPUTDIR)\fpatan16.OBJ +$(OUTPUTDIR)\fprem16.OBJ +$(OUTPUTDIR)\fptan16.OBJ &
		 +$(OUTPUTDIR)\fputc.OBJ +$(OUTPUTDIR)\fqq.OBJ +$(OUTPUTDIR)\frealloc.OBJ +$(OUTPUTDIR)\free.OBJ &
		 +$(OUTPUTDIR)\fstrcat.OBJ +$(OUTPUTDIR)\fstrchr.OBJ +$(OUTPUTDIR)\fstrcmp.OBJ +$(OUTPUTDIR)\fstrcpy.OBJ &
		 +$(OUTPUTDIR)\fstricmp.OBJ +$(OUTPUTDIR)\fstrlen.OBJ +$(OUTPUTDIR)\fstrncat.OBJ +$(OUTPUTDIR)\fstrncmp.OBJ &
		 +$(OUTPUTDIR)\fstrncpy.OBJ +$(OUTPUTDIR)\getcwd.OBJ +$(OUTPUTDIR)\getdos.OBJ +$(OUTPUTDIR)\getenv.OBJ &
		 +$(OUTPUTDIR)\getthd.OBJ +$(OUTPUTDIR)\handle.OBJ +$(OUTPUTDIR)\hdiff.OBJ +$(OUTPUTDIR)\hugeptr.OBJ &
		 +$(OUTPUTDIR)\inter.OBJ +$(OUTPUTDIR)\io.OBJ +$(OUTPUTDIR)\itoa.OBJ +$(OUTPUTDIR)\kbhit.OBJ &
		 +$(OUTPUTDIR)\ldexp.OBJ +$(OUTPUTDIR)\lmath.OBJ +$(OUTPUTDIR)\locking.OBJ +$(OUTPUTDIR)\lseek.OBJ &
		 +$(OUTPUTDIR)\malloc.OBJ +$(OUTPUTDIR)\math87.OBJ +$(OUTPUTDIR)\mathtrn.OBJ +$(OUTPUTDIR)\mathtrnf.OBJ &
		 +$(OUTPUTDIR)\memavl.OBJ +$(OUTPUTDIR)\memchr.OBJ +$(OUTPUTDIR)\memcmp.OBJ +$(OUTPUTDIR)\memcpy.OBJ &
		 +$(OUTPUTDIR)\memicmp.OBJ +$(OUTPUTDIR)\memmax.OBJ +$(OUTPUTDIR)\memset.OBJ +$(OUTPUTDIR)\movedata.OBJ &
		 +$(OUTPUTDIR)\msize.OBJ +$(OUTPUTDIR)\ncalloc.OBJ +$(OUTPUTDIR)\nmalloc.OBJ +$(OUTPUTDIR)\nrealloc.OBJ &
		 +$(OUTPUTDIR)\page.OBJ +$(OUTPUTDIR)\parall.OBJ +$(OUTPUTDIR)\patch16.OBJ +$(OUTPUTDIR)\peek.OBJ &
		 +$(OUTPUTDIR)\port.OBJ +$(OUTPUTDIR)\read.OBJ +$(OUTPUTDIR)\realloc.OBJ +$(OUTPUTDIR)\rot.OBJ &
		 +$(OUTPUTDIR)\seg.OBJ +$(OUTPUTDIR)\setargv.OBJ +$(OUTPUTDIR)\sound.OBJ +$(OUTPUTDIR)\stkovfl.OBJ &
		 +$(OUTPUTDIR)\stpcpy.OBJ +$(OUTPUTDIR)\strcat.OBJ +$(OUTPUTDIR)\strchr.OBJ +$(OUTPUTDIR)\strcmp.OBJ &
		 +$(OUTPUTDIR)\strcmpl.OBJ +$(OUTPUTDIR)\strcpy.OBJ +$(OUTPUTDIR)\strlen.OBJ +$(OUTPUTDIR)\strncat.OBJ &
		 +$(OUTPUTDIR)\strncmp.OBJ +$(OUTPUTDIR)\strncpy.OBJ +$(OUTPUTDIR)\strtoul.OBJ +$(OUTPUTDIR)\sysint.OBJ &
		 +$(OUTPUTDIR)\utime.OBJ +$(OUTPUTDIR)\wincio.OBJ +$(OUTPUTDIR)\write.OBJ +$(OUTPUTDIR)\_afftol.OBJ &
		 +$(OUTPUTDIR)\_afulrem.OBJ +$(OUTPUTDIR)\_dup.OBJ +$(OUTPUTDIR)\_fcvt.OBJ +$(OUTPUTDIR)\_fgtchar.OBJ &
		 +$(OUTPUTDIR)\_fheap.OBJ +$(OUTPUTDIR)\_fmemmve.OBJ +$(OUTPUTDIR)\_fmemset.OBJ +$(OUTPUTDIR)\_fopen.OBJ &
		 +$(OUTPUTDIR)\_fptchar.OBJ +$(OUTPUTDIR)\_fstrchr.OBJ +$(OUTPUTDIR)\_fstrtok.OBJ +$(OUTPUTDIR)\_ismbbld.OBJ &
		 +$(OUTPUTDIR)\_ismbbtl.OBJ +$(OUTPUTDIR)\_itoa.OBJ +$(OUTPUTDIR)\_ltoa.OBJ +$(OUTPUTDIR)\_main.OBJ &
		 +$(OUTPUTDIR)\_memicmp.OBJ +$(OUTPUTDIR)\_nstrdup.OBJ +$(OUTPUTDIR)\_printf.OBJ +$(OUTPUTDIR)\_strdup.OBJ &
		 +$(OUTPUTDIR)\_strerro.OBJ +$(OUTPUTDIR)\_strrev.OBJ +$(OUTPUTDIR)\_tzset.OBJ +$(OUTPUTDIR)\_wep.OBJ &
		 +$(OUTPUTDIR)\allocmem.OBJ +$(OUTPUTDIR)\arcerr.OBJ +$(OUTPUTDIR)\asctime.OBJ +$(OUTPUTDIR)\asin.OBJ &
		 +$(OUTPUTDIR)\assert.OBJ +$(OUTPUTDIR)\atan.OBJ +$(OUTPUTDIR)\atexit.OBJ +$(OUTPUTDIR)\atof.OBJ &
		 +$(OUTPUTDIR)\biosc.OBJ +$(OUTPUTDIR)\buildenv.OBJ +$(OUTPUTDIR)\cgets.OBJ +$(OUTPUTDIR)\charmax.OBJ &
		 +$(OUTPUTDIR)\chsize.OBJ +$(OUTPUTDIR)\commit.OBJ +$(OUTPUTDIR)\coreleft.OBJ +$(OUTPUTDIR)\cpphook.OBJ &
		 +$(OUTPUTDIR)\cprintf.OBJ +$(OUTPUTDIR)\cputs.OBJ +$(OUTPUTDIR)\creat.OBJ +$(OUTPUTDIR)\cscanf.OBJ &
		 +$(OUTPUTDIR)\ctime.OBJ +$(OUTPUTDIR)\ctrlbrk.OBJ +$(OUTPUTDIR)\ctype.OBJ +$(OUTPUTDIR)\disk.OBJ &
		 +$(OUTPUTDIR)\diskfree.OBJ +$(OUTPUTDIR)\disp2.OBJ +$(OUTPUTDIR)\doscomp.OBJ +$(OUTPUTDIR)\doskeep.OBJ &
		 +$(OUTPUTDIR)\doslock.OBJ +$(OUTPUTDIR)\dosseek.OBJ +$(OUTPUTDIR)\dprintf.OBJ +$(OUTPUTDIR)\ecvt.OBJ &
		 +$(OUTPUTDIR)\eof.OBJ +$(OUTPUTDIR)\execl.OBJ +$(OUTPUTDIR)\execle.OBJ +$(OUTPUTDIR)\execlp.OBJ &
		 +$(OUTPUTDIR)\execlpe.OBJ +$(OUTPUTDIR)\execv.OBJ +$(OUTPUTDIR)\execve.OBJ +$(OUTPUTDIR)\execvp.OBJ &
		 +$(OUTPUTDIR)\execvpe.OBJ +$(OUTPUTDIR)\exit.OBJ +$(OUTPUTDIR)\exitstat.OBJ +$(OUTPUTDIR)\exp.OBJ &
		 +$(OUTPUTDIR)\far.OBJ +$(OUTPUTDIR)\fatexit.OBJ +$(OUTPUTDIR)\fclose.OBJ +$(OUTPUTDIR)\fcloseal.OBJ &
		 +$(OUTPUTDIR)\fcvt.OBJ +$(OUTPUTDIR)\fdivp.OBJ +$(OUTPUTDIR)\fdopen.OBJ +$(OUTPUTDIR)\feexcept.OBJ &
		 +$(OUTPUTDIR)\fflush.OBJ +$(OUTPUTDIR)\fgetchar.OBJ +$(OUTPUTDIR)\fgets.OBJ +$(OUTPUTDIR)\floatcvt.OBJ &
		 +$(OUTPUTDIR)\fltenv.OBJ +$(OUTPUTDIR)\fltused.OBJ +$(OUTPUTDIR)\flush.OBJ +$(OUTPUTDIR)\flushall.OBJ &
		 +$(OUTPUTDIR)\fmacros.OBJ +$(OUTPUTDIR)\fmbyte.OBJ +$(OUTPUTDIR)\fmsize.OBJ +$(OUTPUTDIR)\fmt.OBJ &
		 +$(OUTPUTDIR)\fonexit.OBJ +$(OUTPUTDIR)\fopen.OBJ +$(OUTPUTDIR)\fpmath.OBJ +$(OUTPUTDIR)\fpsig.OBJ &
		 +$(OUTPUTDIR)\fputchar.OBJ +$(OUTPUTDIR)\fputs.OBJ +$(OUTPUTDIR)\fread.OBJ +$(OUTPUTDIR)\freect.OBJ &
		 +$(OUTPUTDIR)\freopen.OBJ +$(OUTPUTDIR)\fsopen.OBJ +$(OUTPUTDIR)\fstrcspn.OBJ +$(OUTPUTDIR)\fstrdup.OBJ &
		 +$(OUTPUTDIR)\fstrlwr.OBJ +$(OUTPUTDIR)\fstrnicm.OBJ +$(OUTPUTDIR)\fstrnset.OBJ +$(OUTPUTDIR)\fstrpbrk.OBJ &
		 +$(OUTPUTDIR)\fstrrchr.OBJ +$(OUTPUTDIR)\fstrrev.OBJ +$(OUTPUTDIR)\fstrset.OBJ +$(OUTPUTDIR)\fstrspn.OBJ &
		 +$(OUTPUTDIR)\fstrstr.OBJ +$(OUTPUTDIR)\fstrtok.OBJ +$(OUTPUTDIR)\fstrupr.OBJ +$(OUTPUTDIR)\ftime.OBJ &
		 +$(OUTPUTDIR)\fwrite.OBJ +$(OUTPUTDIR)\gcvt.OBJ +$(OUTPUTDIR)\getchar.OBJ +$(OUTPUTDIR)\getdate.OBJ &
		 +$(OUTPUTDIR)\getpid.OBJ +$(OUTPUTDIR)\gets.OBJ +$(OUTPUTDIR)\heap.OBJ +$(OUTPUTDIR)\hyper.OBJ &
		 +$(OUTPUTDIR)\ieee.OBJ +$(OUTPUTDIR)\iob.OBJ +$(OUTPUTDIR)\isalnum.OBJ +$(OUTPUTDIR)\isalpha.OBJ &
		 +$(OUTPUTDIR)\isascii.OBJ +$(OUTPUTDIR)\iscntrl.OBJ +$(OUTPUTDIR)\iscsym.OBJ +$(OUTPUTDIR)\iscsymf.OBJ &
		 +$(OUTPUTDIR)\isdigit.OBJ +$(OUTPUTDIR)\isgraph.OBJ +$(OUTPUTDIR)\islower.OBJ +$(OUTPUTDIR)\isprint.OBJ &
		 +$(OUTPUTDIR)\ispunct.OBJ +$(OUTPUTDIR)\isspace.OBJ +$(OUTPUTDIR)\isupper.OBJ +$(OUTPUTDIR)\isxdigit.OBJ &
		 +$(OUTPUTDIR)\jmp.OBJ +$(OUTPUTDIR)\lconv.OBJ +$(OUTPUTDIR)\lfind.OBJ +$(OUTPUTDIR)\loctime.OBJ &
		 +$(OUTPUTDIR)\log.OBJ +$(OUTPUTDIR)\lsearch.OBJ +$(OUTPUTDIR)\ltoa.OBJ +$(OUTPUTDIR)\mallocf.OBJ &
		 +$(OUTPUTDIR)\matherr.OBJ +$(OUTPUTDIR)\mbctype.OBJ +$(OUTPUTDIR)\mbstowcs.OBJ +$(OUTPUTDIR)\mbstring.OBJ &
		 +$(OUTPUTDIR)\mbyte.OBJ +$(OUTPUTDIR)\memccpy.OBJ +$(OUTPUTDIR)\mktemp.OBJ +$(OUTPUTDIR)\nmsize.OBJ &
		 +$(OUTPUTDIR)\okbigbuf.OBJ +$(OUTPUTDIR)\onexit.OBJ +$(OUTPUTDIR)\perror.OBJ +$(OUTPUTDIR)\poly.OBJ &
		 +$(OUTPUTDIR)\pow.OBJ +$(OUTPUTDIR)\printf.OBJ +$(OUTPUTDIR)\ptrchk.OBJ +$(OUTPUTDIR)\purecall.OBJ &
		 +$(OUTPUTDIR)\putch.OBJ +$(OUTPUTDIR)\putchar.OBJ +$(OUTPUTDIR)\putenv.OBJ +$(OUTPUTDIR)\puts.OBJ &
		 +$(OUTPUTDIR)\qsort.OBJ +$(OUTPUTDIR)\rand.OBJ +$(OUTPUTDIR)\response.OBJ +$(OUTPUTDIR)\rmtmp.OBJ &
		 +$(OUTPUTDIR)\round.OBJ +$(OUTPUTDIR)\sbrk.OBJ +$(OUTPUTDIR)\scanf.OBJ +$(OUTPUTDIR)\setdoser.OBJ &
		 +$(OUTPUTDIR)\setenvp.OBJ +$(OUTPUTDIR)\seterrno.OBJ +$(OUTPUTDIR)\setlc.OBJ +$(OUTPUTDIR)\setmode.OBJ &
		 +$(OUTPUTDIR)\signal.OBJ +$(OUTPUTDIR)\sin.OBJ +$(OUTPUTDIR)\sleep.OBJ +$(OUTPUTDIR)\sopenf.OBJ &
		 +$(OUTPUTDIR)\spawn.OBJ +$(OUTPUTDIR)\spawnl.OBJ +$(OUTPUTDIR)\spawnle.OBJ +$(OUTPUTDIR)\spawnlp.OBJ &
		 +$(OUTPUTDIR)\spawnlpe.OBJ +$(OUTPUTDIR)\spawnv.OBJ +$(OUTPUTDIR)\spawnve.OBJ +$(OUTPUTDIR)\spawnvp.OBJ &
		 +$(OUTPUTDIR)\spawnvpe.OBJ +$(OUTPUTDIR)\splitpat.OBJ +$(OUTPUTDIR)\sqrt.OBJ +$(OUTPUTDIR)\stack.OBJ &
		 +$(OUTPUTDIR)\stat.OBJ +$(OUTPUTDIR)\strcspn.OBJ +$(OUTPUTDIR)\strdup.OBJ +$(OUTPUTDIR)\strerror.OBJ &
		 +$(OUTPUTDIR)\strf.OBJ +$(OUTPUTDIR)\strlwr.OBJ +$(OUTPUTDIR)\strnicmp.OBJ +$(OUTPUTDIR)\strnset.OBJ &
		 +$(OUTPUTDIR)\strpbrk.OBJ +$(OUTPUTDIR)\strrchr.OBJ +$(OUTPUTDIR)\strrev.OBJ +$(OUTPUTDIR)\strset.OBJ &
		 +$(OUTPUTDIR)\strspn.OBJ +$(OUTPUTDIR)\strstr.OBJ +$(OUTPUTDIR)\strtod.OBJ +$(OUTPUTDIR)\strtof.OBJ &
		 +$(OUTPUTDIR)\strtok.OBJ +$(OUTPUTDIR)\strtol.OBJ +$(OUTPUTDIR)\strupr.OBJ +$(OUTPUTDIR)\strxfrm.OBJ &
		 +$(OUTPUTDIR)\swab.OBJ +$(OUTPUTDIR)\system.OBJ +$(OUTPUTDIR)\tabsize.OBJ +$(OUTPUTDIR)\tan.OBJ &
		 +$(OUTPUTDIR)\tchar.OBJ +$(OUTPUTDIR)\time.OBJ +$(OUTPUTDIR)\time3.OBJ +$(OUTPUTDIR)\timesup.OBJ &
		 +$(OUTPUTDIR)\tmpnam.OBJ +$(OUTPUTDIR)\toascii.OBJ +$(OUTPUTDIR)\tolower.OBJ +$(OUTPUTDIR)\toupper.OBJ &
		 +$(OUTPUTDIR)\trigerr.OBJ +$(OUTPUTDIR)\ultoa.OBJ +$(OUTPUTDIR)\umask.OBJ +$(OUTPUTDIR)\ungetc.OBJ &
		 +$(OUTPUTDIR)\unmangle.OBJ +$(OUTPUTDIR)\vprintf.OBJ +$(OUTPUTDIR)\wcscpy.OBJ +$(OUTPUTDIR)\wcslen.OBJ &
		 +$(OUTPUTDIR)\wep.OBJ +$(OUTPUTDIR)\winerr.OBJ +$(OUTPUTDIR)\winmouse.OBJ +$(OUTPUTDIR)\winsleep.OBJ &
		 +$(OUTPUTDIR)\_delete.OBJ +$(OUTPUTDIR)\_delnear.OBJ +$(OUTPUTDIR)\_handler.OBJ +$(OUTPUTDIR)\_new.OBJ &
		 +$(OUTPUTDIR)\_new2.OBJ +$(OUTPUTDIR)\_newnear.OBJ +$(OUTPUTDIR)\complex.OBJ +$(OUTPUTDIR)\coperat.OBJ &
		 +$(OUTPUTDIR)\cstrm.OBJ +$(OUTPUTDIR)\ctrig.OBJ +$(OUTPUTDIR)\dbp.OBJ +$(OUTPUTDIR)\except.OBJ &
		 +$(OUTPUTDIR)\fstream.OBJ +$(OUTPUTDIR)\generr.OBJ +$(OUTPUTDIR)\ios.OBJ +$(OUTPUTDIR)\iostream.OBJ &
		 +$(OUTPUTDIR)\istream.OBJ +$(OUTPUTDIR)\istreamf.OBJ +$(OUTPUTDIR)\istreami.OBJ +$(OUTPUTDIR)\istreamx.OBJ &
		 +$(OUTPUTDIR)\manip.OBJ +$(OUTPUTDIR)\ostream.OBJ +$(OUTPUTDIR)\ostreamf.OBJ +$(OUTPUTDIR)\ostreami.OBJ &
		 +$(OUTPUTDIR)\ostreamx.OBJ +$(OUTPUTDIR)\rtti.OBJ +$(OUTPUTDIR)\siosync.OBJ +$(OUTPUTDIR)\stdiostr.OBJ &
		 +$(OUTPUTDIR)\stream.OBJ +$(OUTPUTDIR)\streambf.OBJ +$(OUTPUTDIR)\strstrea.OBJ +$(OUTPUTDIR)\typeinfo.OBJ &
		 +$(OUTPUTDIR)\vecnew.OBJ +$(OUTPUTDIR)\vector.OBJ +$(OUTPUTDIR)\scver.OBJ +$(OUTPUTDIR)\libentry.OBJ &
		 +$(OUTPUTDIR)\_alloca.OBJ +$(OUTPUTDIR)\alloca.OBJ +$(OUTPUTDIR)\mschkstk.OBJ +$(OUTPUTDIR)\anew.OBJ &
		 +$(OUTPUTDIR)\anew2.OBJ +$(OUTPUTDIR)\adelete.OBJ +$(OUTPUTDIR)\corenew.OBJ +$(OUTPUTDIR)\purevirt.OBJ &
		 +$(OUTPUTDIR)\ehdata.OBJ +$(OUTPUTDIR)\_ISMBSLD.OBJ +$(OUTPUTDIR)\_ISMBSTL.OBJ +$(OUTPUTDIR)\ISMBALNU.OBJ &
		 +$(OUTPUTDIR)\ISMBALPH.OBJ +$(OUTPUTDIR)\ISMBBYTE.OBJ +$(OUTPUTDIR)\ISMBCL0.OBJ +$(OUTPUTDIR)\ISMBCL1.OBJ &
		 +$(OUTPUTDIR)\ISMBCL2.OBJ +$(OUTPUTDIR)\ISMBDIGI.OBJ +$(OUTPUTDIR)\ISMBGRAP.OBJ +$(OUTPUTDIR)\ISMBHIRA.OBJ &
		 +$(OUTPUTDIR)\ISMBKATA.OBJ +$(OUTPUTDIR)\ISMBLEGA.OBJ +$(OUTPUTDIR)\ISMBLOWE.OBJ +$(OUTPUTDIR)\ISMBPRIN.OBJ &
		 +$(OUTPUTDIR)\ISMBPUNC.OBJ +$(OUTPUTDIR)\ISMBSPAC.OBJ +$(OUTPUTDIR)\ISMBSYMB.OBJ +$(OUTPUTDIR)\ISMBUPPE.OBJ &
		 +$(OUTPUTDIR)\MBBTOMBC.OBJ +$(OUTPUTDIR)\MBBTYPE.OBJ +$(OUTPUTDIR)\MBCCPY.OBJ +$(OUTPUTDIR)\MBCLEN.OBJ &
		 +$(OUTPUTDIR)\MBCPINFO.OBJ +$(OUTPUTDIR)\MBJISJMS.OBJ +$(OUTPUTDIR)\MBJMSJIS.OBJ +$(OUTPUTDIR)\MBLEN.OBJ &
		 +$(OUTPUTDIR)\MBSBTYPE.OBJ +$(OUTPUTDIR)\MBSCHR.OBJ +$(OUTPUTDIR)\MBSCMP.OBJ +$(OUTPUTDIR)\MBSCOLL.OBJ &
		 +$(OUTPUTDIR)\MBSCSPN.OBJ +$(OUTPUTDIR)\MBSDEC.OBJ +$(OUTPUTDIR)\MBSICMP.OBJ +$(OUTPUTDIR)\MBSICOLL.OBJ &
		 +$(OUTPUTDIR)\MBSINC.OBJ +$(OUTPUTDIR)\MBSLEN.OBJ +$(OUTPUTDIR)\MBSLWR.OBJ +$(OUTPUTDIR)\MBSNBCAT.OBJ &
		 +$(OUTPUTDIR)\MBSNBCMP.OBJ +$(OUTPUTDIR)\MBSNBCNT.OBJ +$(OUTPUTDIR)\MBSNBCOL.OBJ +$(OUTPUTDIR)\MBSNBCPY.OBJ &
		 +$(OUTPUTDIR)\MBSNBICM.OBJ +$(OUTPUTDIR)\MBSNBICO.OBJ +$(OUTPUTDIR)\MBSNBSET.OBJ +$(OUTPUTDIR)\MBSNCAT.OBJ &
		 +$(OUTPUTDIR)\MBSNCCNT.OBJ +$(OUTPUTDIR)\MBSNCMP.OBJ +$(OUTPUTDIR)\MBSNCOLL.OBJ +$(OUTPUTDIR)\MBSNCPY.OBJ &
		 +$(OUTPUTDIR)\MBSNEXTC.OBJ +$(OUTPUTDIR)\MBSNICMP.OBJ +$(OUTPUTDIR)\MBSNICOL.OBJ +$(OUTPUTDIR)\MBSNINC.OBJ &
		 +$(OUTPUTDIR)\MBSNSET.OBJ +$(OUTPUTDIR)\MBSPBRK.OBJ +$(OUTPUTDIR)\MBSRCHR.OBJ +$(OUTPUTDIR)\MBSREV.OBJ &
		 +$(OUTPUTDIR)\MBSSET.OBJ +$(OUTPUTDIR)\MBSSPN.OBJ +$(OUTPUTDIR)\MBSSPNP.OBJ +$(OUTPUTDIR)\MBSSTR.OBJ &
		 +$(OUTPUTDIR)\MBSTOK.OBJ +$(OUTPUTDIR)\MBSTRLEN.OBJ +$(OUTPUTDIR)\MBSUPR.OBJ +$(OUTPUTDIR)\MBTOHIRA.OBJ &
		 +$(OUTPUTDIR)\MBTOKATA.OBJ +$(OUTPUTDIR)\MBTOLOWE.OBJ +$(OUTPUTDIR)\MBTOUPPE.OBJ +$(OUTPUTDIR)\initmbcp.OBJ &
		 +$(OUTPUTDIR)\setmbcp.OBJ
<<
		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)

C?C?Bsource?B75?BRTL?Bsrc?Bbuild?Bswindos?Pprj:
		$(MAKE) -fswindos.mak "SUB_DEBUG=$(DEBUG)"








$(OUTPUTDIR)\_8087.OBJ:	..\core16\_8087.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_8087.obj ..\core16\_8087.asm



$(OUTPUTDIR)\_afulshr.OBJ:	..\compat16\_afulshr.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_afulshr.obj ..\compat16\_afulshr.asm



$(OUTPUTDIR)\_anlshl.OBJ:	..\compat16\_anlshl.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_anlshl.obj ..\compat16\_anlshl.asm



$(OUTPUTDIR)\_anulshr.OBJ:	..\compat16\_anulshr.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_anulshr.obj ..\compat16\_anulshr.asm



$(OUTPUTDIR)\_fmemicm.OBJ:	..\compat16\_fmemicm.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_fmemicm.obj ..\compat16\_fmemicm.asm



$(OUTPUTDIR)\_ptrchk.OBJ:	..\core16\_ptrchk.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_ptrchk.obj ..\core16\_ptrchk.asm



$(OUTPUTDIR)\amath.OBJ:	..\core16\amath.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\amath.obj ..\core16\amath.asm



$(OUTPUTDIR)\atoi.OBJ:	..\core16\atoi.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\atoi.obj ..\core16\atoi.asm



$(OUTPUTDIR)\atol.OBJ:	..\core16\atol.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\atol.obj ..\core16\atol.asm



$(OUTPUTDIR)\baslnk.OBJ:	..\core\baslnk.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\baslnk.obj ..\core\baslnk.asm



$(OUTPUTDIR)\bios.OBJ:	..\core16\bios.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\bios.obj ..\core16\bios.asm



$(OUTPUTDIR)\calloc.OBJ:	..\core16\calloc.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\calloc.obj ..\core16\calloc.asm



$(OUTPUTDIR)\cerror.OBJ:	..\core16\cerror.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\cerror.obj ..\core16\cerror.asm



$(OUTPUTDIR)\chkstk.OBJ:	..\core16\chkstk.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\chkstk.obj ..\core16\chkstk.asm



$(OUTPUTDIR)\cinit.OBJ:	..\core16\cinit.asm
		$(CC) -M/Ml $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\cinit.obj ..\core16\cinit.asm



$(OUTPUTDIR)\clock.OBJ:	..\core16\clock.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\clock.obj ..\core16\clock.asm



$(OUTPUTDIR)\controlc.OBJ:	..\core16\controlc.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\controlc.obj ..\core16\controlc.asm



$(OUTPUTDIR)\country.OBJ:	..\core16\country.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\country.obj ..\core16\country.asm



$(OUTPUTDIR)\cputype.OBJ:	..\core16\cputype.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\cputype.obj ..\core16\cputype.asm



$(OUTPUTDIR)\cw.OBJ:	..\win16\cw.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\cw.obj ..\win16\cw.asm



$(OUTPUTDIR)\dclass.OBJ:	..\core16\dclass.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dclass.obj ..\core16\dclass.asm



$(OUTPUTDIR)\disp.OBJ:	..\core16\disp.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\disp.obj ..\core16\disp.asm



$(OUTPUTDIR)\dllstart.OBJ:	..\win16\dllstart.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dllstart.obj ..\win16\dllstart.asm



$(OUTPUTDIR)\dos_find.OBJ:	..\core16\dos_find.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dos_find.obj ..\core16\dos_find.asm



$(OUTPUTDIR)\dosalloc.OBJ:	..\core16\dosalloc.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dosalloc.obj ..\core16\dosalloc.asm



$(OUTPUTDIR)\dosdate.OBJ:	..\core16\dosdate.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dosdate.obj ..\core16\dosdate.asm



$(OUTPUTDIR)\doserr.OBJ:	..\core16\doserr.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\doserr.obj ..\core16\doserr.asm



$(OUTPUTDIR)\dosfil.OBJ:	..\core16\dosfil.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dosfil.obj ..\core16\dosfil.asm



$(OUTPUTDIR)\double.OBJ:	..\core16\double.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\double.obj ..\core16\double.asm



$(OUTPUTDIR)\dpmi.OBJ:	..\win16\dpmi.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dpmi.obj ..\win16\dpmi.asm



$(OUTPUTDIR)\dpmimin.OBJ:	..\win16\dpmimin.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dpmimin.obj ..\win16\dpmimin.asm



$(OUTPUTDIR)\dup.OBJ:	..\core16\dup.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dup.obj ..\core16\dup.asm



$(OUTPUTDIR)\ehasm.OBJ:	..\core16\ehasm.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ehasm.obj ..\core16\ehasm.asm



$(OUTPUTDIR)\emm.OBJ:	..\core16\emm.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\emm.obj ..\core16\emm.asm



$(OUTPUTDIR)\exec2.OBJ:	..\core16\exec2.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\exec2.obj ..\core16\exec2.asm



$(OUTPUTDIR)\farptr.OBJ:	..\core16\farptr.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\farptr.obj ..\core16\farptr.asm



$(OUTPUTDIR)\fcalloc.OBJ:	..\core16\fcalloc.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fcalloc.obj ..\core16\fcalloc.asm



$(OUTPUTDIR)\filelen.OBJ:	..\core16\filelen.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\filelen.obj ..\core16\filelen.asm



$(OUTPUTDIR)\find.OBJ:	..\core16\find.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\find.obj ..\core16\find.asm



$(OUTPUTDIR)\float.OBJ:	..\core16\float.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\float.obj ..\core16\float.asm



$(OUTPUTDIR)\floattyp.OBJ:	..\core16\floattyp.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\floattyp.obj ..\core16\floattyp.asm



$(OUTPUTDIR)\flt87.OBJ:	..\core16\flt87.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\flt87.obj ..\core16\flt87.asm



$(OUTPUTDIR)\fmalloc.OBJ:	..\core16\fmalloc.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fmalloc.obj ..\core16\fmalloc.asm



$(OUTPUTDIR)\fmemchr.OBJ:	..\core16\fmemchr.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fmemchr.obj ..\core16\fmemchr.asm



$(OUTPUTDIR)\fmemcmp.OBJ:	..\core16\fmemcmp.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fmemcmp.obj ..\core16\fmemcmp.asm



$(OUTPUTDIR)\fmemcpy.OBJ:	..\core16\fmemcpy.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fmemcpy.obj ..\core16\fmemcpy.asm



$(OUTPUTDIR)\fmemicmp.OBJ:	..\core16\fmemicmp.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fmemicmp.obj ..\core16\fmemicmp.asm



$(OUTPUTDIR)\fmemset.OBJ:	..\core16\fmemset.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fmemset.obj ..\core16\fmemset.asm



$(OUTPUTDIR)\fpatan16.OBJ:	..\core16\fpatan16.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fpatan16.obj ..\core16\fpatan16.asm



$(OUTPUTDIR)\fprem16.OBJ:	..\core16\fprem16.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fprem16.obj ..\core16\fprem16.asm



$(OUTPUTDIR)\fptan16.OBJ:	..\core16\fptan16.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fptan16.obj ..\core16\fptan16.asm



$(OUTPUTDIR)\fputc.OBJ:	..\core16\fputc.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fputc.obj ..\core16\fputc.asm



$(OUTPUTDIR)\fqq.OBJ:	..\core16\fqq.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fqq.obj ..\core16\fqq.asm



$(OUTPUTDIR)\frealloc.OBJ:	..\core16\frealloc.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\frealloc.obj ..\core16\frealloc.asm



$(OUTPUTDIR)\free.OBJ:	..\core16\free.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\free.obj ..\core16\free.asm



$(OUTPUTDIR)\fstrcat.OBJ:	..\core16\fstrcat.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fstrcat.obj ..\core16\fstrcat.asm



$(OUTPUTDIR)\fstrchr.OBJ:	..\core16\fstrchr.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fstrchr.obj ..\core16\fstrchr.asm



$(OUTPUTDIR)\fstrcmp.OBJ:	..\core16\fstrcmp.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fstrcmp.obj ..\core16\fstrcmp.asm



$(OUTPUTDIR)\fstrcpy.OBJ:	..\core16\fstrcpy.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fstrcpy.obj ..\core16\fstrcpy.asm



$(OUTPUTDIR)\fstricmp.OBJ:	..\core16\fstricmp.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fstricmp.obj ..\core16\fstricmp.asm



$(OUTPUTDIR)\fstrlen.OBJ:	..\core16\fstrlen.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fstrlen.obj ..\core16\fstrlen.asm



$(OUTPUTDIR)\fstrncat.OBJ:	..\core16\fstrncat.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fstrncat.obj ..\core16\fstrncat.asm



$(OUTPUTDIR)\fstrncmp.OBJ:	..\core16\fstrncmp.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fstrncmp.obj ..\core16\fstrncmp.asm



$(OUTPUTDIR)\fstrncpy.OBJ:	..\core16\fstrncpy.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fstrncpy.obj ..\core16\fstrncpy.asm



$(OUTPUTDIR)\getcwd.OBJ:	..\core16\getcwd.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\getcwd.obj ..\core16\getcwd.asm



$(OUTPUTDIR)\getdos.OBJ:	..\core16\getdos.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\getdos.obj ..\core16\getdos.asm



$(OUTPUTDIR)\getenv.OBJ:	..\core16\getenv.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\getenv.obj ..\core16\getenv.asm



$(OUTPUTDIR)\getthd.OBJ:	..\core16\getthd.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\getthd.obj ..\core16\getthd.asm



$(OUTPUTDIR)\handle.OBJ:	..\core16\handle.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\handle.obj ..\core16\handle.asm



$(OUTPUTDIR)\hdiff.OBJ:	..\core16\hdiff.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\hdiff.obj ..\core16\hdiff.asm



$(OUTPUTDIR)\hugeptr.OBJ:	..\core16\hugeptr.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\hugeptr.obj ..\core16\hugeptr.asm



$(OUTPUTDIR)\inter.OBJ:	..\core16\inter.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\inter.obj ..\core16\inter.asm



$(OUTPUTDIR)\io.OBJ:	..\core16\io.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\io.obj ..\core16\io.asm



$(OUTPUTDIR)\itoa.OBJ:	..\core16\itoa.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\itoa.obj ..\core16\itoa.asm



$(OUTPUTDIR)\kbhit.OBJ:	..\core16\kbhit.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\kbhit.obj ..\core16\kbhit.asm



$(OUTPUTDIR)\ldexp.OBJ:	..\core16\ldexp.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ldexp.obj ..\core16\ldexp.asm



$(OUTPUTDIR)\lmath.OBJ:	..\core16\lmath.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\lmath.obj ..\core16\lmath.asm



$(OUTPUTDIR)\locking.OBJ:	..\core16\locking.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\locking.obj ..\core16\locking.asm



$(OUTPUTDIR)\lseek.OBJ:	..\core16\lseek.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\lseek.obj ..\core16\lseek.asm



$(OUTPUTDIR)\malloc.OBJ:	..\core16\malloc.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\malloc.obj ..\core16\malloc.asm



$(OUTPUTDIR)\math87.OBJ:	..\core16\math87.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\math87.obj ..\core16\math87.asm



$(OUTPUTDIR)\mathtrn.OBJ:	..\core16\mathtrn.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\mathtrn.obj ..\core16\mathtrn.asm



$(OUTPUTDIR)\mathtrnf.OBJ:	..\core16\mathtrnf.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\mathtrnf.obj ..\core16\mathtrnf.asm



$(OUTPUTDIR)\memavl.OBJ:	..\core16\memavl.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\memavl.obj ..\core16\memavl.asm



$(OUTPUTDIR)\memchr.OBJ:	..\core16\memchr.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\memchr.obj ..\core16\memchr.asm



$(OUTPUTDIR)\memcmp.OBJ:	..\core16\memcmp.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\memcmp.obj ..\core16\memcmp.asm



$(OUTPUTDIR)\memcpy.OBJ:	..\core16\memcpy.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\memcpy.obj ..\core16\memcpy.asm



$(OUTPUTDIR)\memicmp.OBJ:	..\core16\memicmp.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\memicmp.obj ..\core16\memicmp.asm



$(OUTPUTDIR)\memmax.OBJ:	..\core16\memmax.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\memmax.obj ..\core16\memmax.asm



$(OUTPUTDIR)\memset.OBJ:	..\core16\memset.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\memset.obj ..\core16\memset.asm



$(OUTPUTDIR)\movedata.OBJ:	..\core16\movedata.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\movedata.obj ..\core16\movedata.asm



$(OUTPUTDIR)\msize.OBJ:	..\core16\msize.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\msize.obj ..\core16\msize.asm



$(OUTPUTDIR)\ncalloc.OBJ:	..\core16\ncalloc.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ncalloc.obj ..\core16\ncalloc.asm



$(OUTPUTDIR)\nmalloc.OBJ:	..\core16\nmalloc.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\nmalloc.obj ..\core16\nmalloc.asm



$(OUTPUTDIR)\nrealloc.OBJ:	..\core16\nrealloc.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\nrealloc.obj ..\core16\nrealloc.asm



$(OUTPUTDIR)\page.OBJ:	..\core16\page.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\page.obj ..\core16\page.asm



$(OUTPUTDIR)\parall.OBJ:	..\core16\parall.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\parall.obj ..\core16\parall.asm



$(OUTPUTDIR)\patch16.OBJ:	..\core16\patch16.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\patch16.obj ..\core16\patch16.asm



$(OUTPUTDIR)\peek.OBJ:	..\core16\peek.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\peek.obj ..\core16\peek.asm



$(OUTPUTDIR)\port.OBJ:	..\core16\port.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\port.obj ..\core16\port.asm



$(OUTPUTDIR)\read.OBJ:	..\core16\read.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\read.obj ..\core16\read.asm



$(OUTPUTDIR)\realloc.OBJ:	..\core16\realloc.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\realloc.obj ..\core16\realloc.asm



$(OUTPUTDIR)\rot.OBJ:	..\core16\rot.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\rot.obj ..\core16\rot.asm



$(OUTPUTDIR)\seg.OBJ:	..\core16\seg.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\seg.obj ..\core16\seg.asm



$(OUTPUTDIR)\setargv.OBJ:	..\win16\setargv.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\setargv.obj ..\win16\setargv.asm



$(OUTPUTDIR)\sound.OBJ:	..\core16\sound.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\sound.obj ..\core16\sound.asm



$(OUTPUTDIR)\stkovfl.OBJ:	..\core16\stkovfl.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\stkovfl.obj ..\core16\stkovfl.asm



$(OUTPUTDIR)\stpcpy.OBJ:	..\core16\stpcpy.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\stpcpy.obj ..\core16\stpcpy.asm



$(OUTPUTDIR)\strcat.OBJ:	..\core16\strcat.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strcat.obj ..\core16\strcat.asm



$(OUTPUTDIR)\strchr.OBJ:	..\core16\strchr.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strchr.obj ..\core16\strchr.asm



$(OUTPUTDIR)\strcmp.OBJ:	..\core16\strcmp.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strcmp.obj ..\core16\strcmp.asm



$(OUTPUTDIR)\strcmpl.OBJ:	..\core16\strcmpl.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strcmpl.obj ..\core16\strcmpl.asm



$(OUTPUTDIR)\strcpy.OBJ:	..\core16\strcpy.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strcpy.obj ..\core16\strcpy.asm



$(OUTPUTDIR)\strlen.OBJ:	..\core16\strlen.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strlen.obj ..\core16\strlen.asm



$(OUTPUTDIR)\strncat.OBJ:	..\core16\strncat.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strncat.obj ..\core16\strncat.asm



$(OUTPUTDIR)\strncmp.OBJ:	..\core16\strncmp.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strncmp.obj ..\core16\strncmp.asm



$(OUTPUTDIR)\strncpy.OBJ:	..\core16\strncpy.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strncpy.obj ..\core16\strncpy.asm



$(OUTPUTDIR)\strtoul.OBJ:	..\core16\strtoul.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strtoul.obj ..\core16\strtoul.asm



$(OUTPUTDIR)\sysint.OBJ:	..\core16\sysint.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\sysint.obj ..\core16\sysint.asm



$(OUTPUTDIR)\utime.OBJ:	..\core16\utime.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\utime.obj ..\core16\utime.asm



$(OUTPUTDIR)\wincio.OBJ:	..\win16\wincio.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wincio.obj ..\win16\wincio.asm



$(OUTPUTDIR)\write.OBJ:	..\core16\write.asm
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\write.obj ..\core16\write.asm



$(OUTPUTDIR)\_afftol.OBJ:	..\compat16\_afftol.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_afftol.obj ..\compat16\_afftol.c



$(OUTPUTDIR)\_afulrem.OBJ:	..\compat16\_afulrem.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_afulrem.obj ..\compat16\_afulrem.c



$(OUTPUTDIR)\_dup.OBJ:	..\core\_dup.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_dup.obj ..\core\_dup.c



$(OUTPUTDIR)\_fcvt.OBJ:	..\core\_fcvt.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_fcvt.obj ..\core\_fcvt.c



$(OUTPUTDIR)\_fgtchar.OBJ:	..\core\_fgtchar.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_fgtchar.obj ..\core\_fgtchar.c



$(OUTPUTDIR)\_fheap.OBJ:	..\compat16\_fheap.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_fheap.obj ..\compat16\_fheap.c



$(OUTPUTDIR)\_fmemmve.OBJ:	..\core16\_fmemmve.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_fmemmve.obj ..\core16\_fmemmve.c



$(OUTPUTDIR)\_fmemset.OBJ:	..\compat16\_fmemset.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_fmemset.obj ..\compat16\_fmemset.c



$(OUTPUTDIR)\_fopen.OBJ:	..\core\_fopen.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_fopen.obj ..\core\_fopen.c



$(OUTPUTDIR)\_fptchar.OBJ:	..\core\_fptchar.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_fptchar.obj ..\core\_fptchar.c



$(OUTPUTDIR)\_fstrchr.OBJ:	..\core16\_fstrchr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_fstrchr.obj ..\core16\_fstrchr.c



$(OUTPUTDIR)\_fstrtok.OBJ:	..\core16\_fstrtok.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_fstrtok.obj ..\core16\_fstrtok.c



$(OUTPUTDIR)\_ismbbld.OBJ:	..\core\_ismbbld.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_ismbbld.obj ..\core\_ismbbld.c



$(OUTPUTDIR)\_ismbbtl.OBJ:	..\core\_ismbbtl.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_ismbbtl.obj ..\core\_ismbbtl.c



$(OUTPUTDIR)\_itoa.OBJ:	..\core\_itoa.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_itoa.obj ..\core\_itoa.c



$(OUTPUTDIR)\_ltoa.OBJ:	..\core\_ltoa.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_ltoa.obj ..\core\_ltoa.c



$(OUTPUTDIR)\_main.OBJ:	..\core\_main.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_main.obj ..\core\_main.c



$(OUTPUTDIR)\_memicmp.OBJ:	..\core\_memicmp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_memicmp.obj ..\core\_memicmp.c



$(OUTPUTDIR)\_nstrdup.OBJ:	..\core16\_nstrdup.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_nstrdup.obj ..\core16\_nstrdup.c



$(OUTPUTDIR)\_printf.OBJ:	..\core\_printf.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_printf.obj ..\core\_printf.c



$(OUTPUTDIR)\_strdup.OBJ:	..\core\_strdup.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_strdup.obj ..\core\_strdup.c



$(OUTPUTDIR)\_strerro.OBJ:	..\core\_strerro.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_strerro.obj ..\core\_strerro.c



$(OUTPUTDIR)\_strrev.OBJ:	..\core\_strrev.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_strrev.obj ..\core\_strrev.c



$(OUTPUTDIR)\_tzset.OBJ:	..\core\_tzset.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_tzset.obj ..\core\_tzset.c



$(OUTPUTDIR)\_wep.OBJ:	..\win16\_wep.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_wep.obj ..\win16\_wep.c



$(OUTPUTDIR)\allocmem.OBJ:	..\core16\allocmem.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\allocmem.obj ..\core16\allocmem.c



$(OUTPUTDIR)\arcerr.OBJ:	..\core\arcerr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\arcerr.obj ..\core\arcerr.c



$(OUTPUTDIR)\asctime.OBJ:	..\core\asctime.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\asctime.obj ..\core\asctime.c



$(OUTPUTDIR)\asin.OBJ:	..\core\asin.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\asin.obj ..\core\asin.c



$(OUTPUTDIR)\assert.OBJ:	..\core\assert.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\assert.obj ..\core\assert.c



$(OUTPUTDIR)\atan.OBJ:	..\core\atan.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\atan.obj ..\core\atan.c



$(OUTPUTDIR)\atexit.OBJ:	..\core\atexit.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\atexit.obj ..\core\atexit.c



$(OUTPUTDIR)\atof.OBJ:	..\core\atof.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\atof.obj ..\core\atof.c



$(OUTPUTDIR)\biosc.OBJ:	..\core\biosc.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\biosc.obj ..\core\biosc.c



$(OUTPUTDIR)\buildenv.OBJ:	..\core\buildenv.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\buildenv.obj ..\core\buildenv.c



$(OUTPUTDIR)\cgets.OBJ:	..\core\cgets.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\cgets.obj ..\core\cgets.c



$(OUTPUTDIR)\charmax.OBJ:	..\core\charmax.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\charmax.obj ..\core\charmax.c



$(OUTPUTDIR)\chsize.OBJ:	..\core\chsize.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\chsize.obj ..\core\chsize.c



$(OUTPUTDIR)\commit.OBJ:	..\core\commit.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\commit.obj ..\core\commit.c



$(OUTPUTDIR)\coreleft.OBJ:	..\win16\coreleft.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\coreleft.obj ..\win16\coreleft.c



$(OUTPUTDIR)\cpphook.OBJ:	..\win16\cpphook.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\cpphook.obj ..\win16\cpphook.c



$(OUTPUTDIR)\cprintf.OBJ:	..\core\cprintf.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\cprintf.obj ..\core\cprintf.c



$(OUTPUTDIR)\cputs.OBJ:	..\core\cputs.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\cputs.obj ..\core\cputs.c



$(OUTPUTDIR)\creat.OBJ:	..\core\creat.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\creat.obj ..\core\creat.c



$(OUTPUTDIR)\cscanf.OBJ:	..\core\cscanf.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\cscanf.obj ..\core\cscanf.c



$(OUTPUTDIR)\ctime.OBJ:	..\core\ctime.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctime.obj ..\core\ctime.c



$(OUTPUTDIR)\ctrlbrk.OBJ:	..\core16\ctrlbrk.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctrlbrk.obj ..\core16\ctrlbrk.c



$(OUTPUTDIR)\ctype.OBJ:	..\core\ctype.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctype.obj ..\core\ctype.c



$(OUTPUTDIR)\disk.OBJ:	..\core\disk.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\disk.obj ..\core\disk.c



$(OUTPUTDIR)\diskfree.OBJ:	..\core\diskfree.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\diskfree.obj ..\core\diskfree.c



$(OUTPUTDIR)\disp2.OBJ:	..\core\disp2.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\disp2.obj ..\core\disp2.c



$(OUTPUTDIR)\doscomp.OBJ:	..\core\doscomp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\doscomp.obj ..\core\doscomp.c



$(OUTPUTDIR)\doskeep.OBJ:	..\core\doskeep.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\doskeep.obj ..\core\doskeep.c



$(OUTPUTDIR)\doslock.OBJ:	..\core\doslock.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\doslock.obj ..\core\doslock.c



$(OUTPUTDIR)\dosseek.OBJ:	..\core\dosseek.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dosseek.obj ..\core\dosseek.c



$(OUTPUTDIR)\dprintf.OBJ:	..\core\dprintf.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dprintf.obj ..\core\dprintf.c



$(OUTPUTDIR)\ecvt.OBJ:	..\core\ecvt.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ecvt.obj ..\core\ecvt.c



$(OUTPUTDIR)\eof.OBJ:	..\core\eof.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\eof.obj ..\core\eof.c



$(OUTPUTDIR)\execl.OBJ:	..\core\execl.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\execl.obj ..\core\execl.c



$(OUTPUTDIR)\execle.OBJ:	..\core\execle.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\execle.obj ..\core\execle.c



$(OUTPUTDIR)\execlp.OBJ:	..\core\execlp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\execlp.obj ..\core\execlp.c



$(OUTPUTDIR)\execlpe.OBJ:	..\core\execlpe.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\execlpe.obj ..\core\execlpe.c



$(OUTPUTDIR)\execv.OBJ:	..\core\execv.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\execv.obj ..\core\execv.c



$(OUTPUTDIR)\execve.OBJ:	..\core\execve.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\execve.obj ..\core\execve.c



$(OUTPUTDIR)\execvp.OBJ:	..\core\execvp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\execvp.obj ..\core\execvp.c



$(OUTPUTDIR)\execvpe.OBJ:	..\core\execvpe.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\execvpe.obj ..\core\execvpe.c



$(OUTPUTDIR)\exit.OBJ:	..\core\exit.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\exit.obj ..\core\exit.c



$(OUTPUTDIR)\exitstat.OBJ:	..\core\exitstat.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\exitstat.obj ..\core\exitstat.c



$(OUTPUTDIR)\exp.OBJ:	..\core\exp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\exp.obj ..\core\exp.c



$(OUTPUTDIR)\far.OBJ:	..\core16\far.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\far.obj ..\core16\far.c



$(OUTPUTDIR)\fatexit.OBJ:	..\core\fatexit.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fatexit.obj ..\core\fatexit.c



$(OUTPUTDIR)\fclose.OBJ:	..\core\fclose.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fclose.obj ..\core\fclose.c



$(OUTPUTDIR)\fcloseal.OBJ:	..\core\fcloseal.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fcloseal.obj ..\core\fcloseal.c



$(OUTPUTDIR)\fcvt.OBJ:	..\core\fcvt.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fcvt.obj ..\core\fcvt.c



$(OUTPUTDIR)\fdivp.OBJ:	..\core\fdivp.c
		$(CC) $(CFLAGS_4) $(DEFINES_4) $(INCLUDES) -o$(OUTPUTDIR)\fdivp.obj ..\core\fdivp.c



$(OUTPUTDIR)\fdopen.OBJ:	..\core\fdopen.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fdopen.obj ..\core\fdopen.c



$(OUTPUTDIR)\feexcept.OBJ:	..\core\feexcept.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\feexcept.obj ..\core\feexcept.c



$(OUTPUTDIR)\fflush.OBJ:	..\core\fflush.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fflush.obj ..\core\fflush.c



$(OUTPUTDIR)\fgetchar.OBJ:	..\core\fgetchar.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fgetchar.obj ..\core\fgetchar.c



$(OUTPUTDIR)\fgets.OBJ:	..\core\fgets.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fgets.obj ..\core\fgets.c



$(OUTPUTDIR)\floatcvt.OBJ:	..\core\floatcvt.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\floatcvt.obj ..\core\floatcvt.c



$(OUTPUTDIR)\fltenv.OBJ:	..\core\fltenv.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fltenv.obj ..\core\fltenv.c



$(OUTPUTDIR)\fltused.OBJ:	..\core\fltused.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fltused.obj ..\core\fltused.c



$(OUTPUTDIR)\flush.OBJ:	..\core\flush.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\flush.obj ..\core\flush.c



$(OUTPUTDIR)\flushall.OBJ:	..\core\flushall.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\flushall.obj ..\core\flushall.c



$(OUTPUTDIR)\fmacros.OBJ:	..\core\fmacros.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fmacros.obj ..\core\fmacros.c



$(OUTPUTDIR)\fmbyte.OBJ:	..\core\fmbyte.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fmbyte.obj ..\core\fmbyte.c



$(OUTPUTDIR)\fmsize.OBJ:	..\core\fmsize.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fmsize.obj ..\core\fmsize.c



$(OUTPUTDIR)\fmt.OBJ:	..\core\fmt.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fmt.obj ..\core\fmt.c



$(OUTPUTDIR)\fonexit.OBJ:	..\core\fonexit.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fonexit.obj ..\core\fonexit.c



$(OUTPUTDIR)\fopen.OBJ:	..\core\fopen.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fopen.obj ..\core\fopen.c



$(OUTPUTDIR)\fpmath.OBJ:	..\compat16\fpmath.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fpmath.obj ..\compat16\fpmath.c



$(OUTPUTDIR)\fpsig.OBJ:	..\core\fpsig.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fpsig.obj ..\core\fpsig.c



$(OUTPUTDIR)\fputchar.OBJ:	..\core\fputchar.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fputchar.obj ..\core\fputchar.c



$(OUTPUTDIR)\fputs.OBJ:	..\core\fputs.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fputs.obj ..\core\fputs.c



$(OUTPUTDIR)\fread.OBJ:	..\core\fread.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fread.obj ..\core\fread.c



$(OUTPUTDIR)\freect.OBJ:	..\core\freect.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\freect.obj ..\core\freect.c



$(OUTPUTDIR)\freopen.OBJ:	..\core\freopen.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\freopen.obj ..\core\freopen.c



$(OUTPUTDIR)\fsopen.OBJ:	..\core\fsopen.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fsopen.obj ..\core\fsopen.c



$(OUTPUTDIR)\fstrcspn.OBJ:	..\core16\fstrcspn.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fstrcspn.obj ..\core16\fstrcspn.c



$(OUTPUTDIR)\fstrdup.OBJ:	..\core16\fstrdup.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fstrdup.obj ..\core16\fstrdup.c



$(OUTPUTDIR)\fstrlwr.OBJ:	..\core16\fstrlwr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fstrlwr.obj ..\core16\fstrlwr.c



$(OUTPUTDIR)\fstrnicm.OBJ:	..\core16\fstrnicm.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fstrnicm.obj ..\core16\fstrnicm.c



$(OUTPUTDIR)\fstrnset.OBJ:	..\core16\fstrnset.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fstrnset.obj ..\core16\fstrnset.c



$(OUTPUTDIR)\fstrpbrk.OBJ:	..\core16\fstrpbrk.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fstrpbrk.obj ..\core16\fstrpbrk.c



$(OUTPUTDIR)\fstrrchr.OBJ:	..\core16\fstrrchr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fstrrchr.obj ..\core16\fstrrchr.c



$(OUTPUTDIR)\fstrrev.OBJ:	..\core16\fstrrev.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fstrrev.obj ..\core16\fstrrev.c



$(OUTPUTDIR)\fstrset.OBJ:	..\core16\fstrset.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fstrset.obj ..\core16\fstrset.c



$(OUTPUTDIR)\fstrspn.OBJ:	..\core16\fstrspn.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fstrspn.obj ..\core16\fstrspn.c



$(OUTPUTDIR)\fstrstr.OBJ:	..\core16\fstrstr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fstrstr.obj ..\core16\fstrstr.c



$(OUTPUTDIR)\fstrtok.OBJ:	..\core16\fstrtok.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fstrtok.obj ..\core16\fstrtok.c



$(OUTPUTDIR)\fstrupr.OBJ:	..\core16\fstrupr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fstrupr.obj ..\core16\fstrupr.c



$(OUTPUTDIR)\ftime.OBJ:	..\core\ftime.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ftime.obj ..\core\ftime.c



$(OUTPUTDIR)\fwrite.OBJ:	..\core\fwrite.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fwrite.obj ..\core\fwrite.c



$(OUTPUTDIR)\gcvt.OBJ:	..\core\gcvt.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\gcvt.obj ..\core\gcvt.c



$(OUTPUTDIR)\getchar.OBJ:	..\core\getchar.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\getchar.obj ..\core\getchar.c



$(OUTPUTDIR)\getdate.OBJ:	..\core16\getdate.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\getdate.obj ..\core16\getdate.c



$(OUTPUTDIR)\getpid.OBJ:	..\core\getpid.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\getpid.obj ..\core\getpid.c



$(OUTPUTDIR)\gets.OBJ:	..\core\gets.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\gets.obj ..\core\gets.c



$(OUTPUTDIR)\heap.OBJ:	..\core\heap.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\heap.obj ..\core\heap.c



$(OUTPUTDIR)\hyper.OBJ:	..\core\hyper.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\hyper.obj ..\core\hyper.c



$(OUTPUTDIR)\ieee.OBJ:	..\core\ieee.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ieee.obj ..\core\ieee.c



$(OUTPUTDIR)\iob.OBJ:	..\core\iob.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\iob.obj ..\core\iob.c



$(OUTPUTDIR)\isalnum.OBJ:	..\core\isalnum.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\isalnum.obj ..\core\isalnum.c



$(OUTPUTDIR)\isalpha.OBJ:	..\core\isalpha.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\isalpha.obj ..\core\isalpha.c



$(OUTPUTDIR)\isascii.OBJ:	..\core\isascii.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\isascii.obj ..\core\isascii.c



$(OUTPUTDIR)\iscntrl.OBJ:	..\core\iscntrl.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\iscntrl.obj ..\core\iscntrl.c



$(OUTPUTDIR)\iscsym.OBJ:	..\core\iscsym.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\iscsym.obj ..\core\iscsym.c



$(OUTPUTDIR)\iscsymf.OBJ:	..\core\iscsymf.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\iscsymf.obj ..\core\iscsymf.c



$(OUTPUTDIR)\isdigit.OBJ:	..\core\isdigit.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\isdigit.obj ..\core\isdigit.c



$(OUTPUTDIR)\isgraph.OBJ:	..\core\isgraph.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\isgraph.obj ..\core\isgraph.c



$(OUTPUTDIR)\islower.OBJ:	..\core\islower.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\islower.obj ..\core\islower.c



$(OUTPUTDIR)\isprint.OBJ:	..\core\isprint.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\isprint.obj ..\core\isprint.c



$(OUTPUTDIR)\ispunct.OBJ:	..\core\ispunct.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ispunct.obj ..\core\ispunct.c



$(OUTPUTDIR)\isspace.OBJ:	..\core\isspace.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\isspace.obj ..\core\isspace.c



$(OUTPUTDIR)\isupper.OBJ:	..\core\isupper.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\isupper.obj ..\core\isupper.c



$(OUTPUTDIR)\isxdigit.OBJ:	..\core\isxdigit.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\isxdigit.obj ..\core\isxdigit.c



$(OUTPUTDIR)\jmp.OBJ:	..\win16\jmp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\jmp.obj ..\win16\jmp.c



$(OUTPUTDIR)\lconv.OBJ:	..\core\lconv.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\lconv.obj ..\core\lconv.c



$(OUTPUTDIR)\lfind.OBJ:	..\core\lfind.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\lfind.obj ..\core\lfind.c



$(OUTPUTDIR)\loctime.OBJ:	..\core\loctime.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\loctime.obj ..\core\loctime.c



$(OUTPUTDIR)\log.OBJ:	..\core\log.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\log.obj ..\core\log.c



$(OUTPUTDIR)\lsearch.OBJ:	..\core\lsearch.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\lsearch.obj ..\core\lsearch.c



$(OUTPUTDIR)\ltoa.OBJ:	..\core16\ltoa.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ltoa.obj ..\core16\ltoa.c



$(OUTPUTDIR)\mallocf.OBJ:	..\core16\mallocf.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\mallocf.obj ..\core16\mallocf.c



$(OUTPUTDIR)\matherr.OBJ:	..\core\matherr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\matherr.obj ..\core\matherr.c



$(OUTPUTDIR)\mbctype.OBJ:	..\core\mbctype.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\mbctype.obj ..\core\mbctype.c



$(OUTPUTDIR)\mbstowcs.OBJ:	..\core\mbstowcs.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\mbstowcs.obj ..\core\mbstowcs.c



$(OUTPUTDIR)\mbstring.OBJ:	..\core\mbstring.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\mbstring.obj ..\core\mbstring.c



$(OUTPUTDIR)\mbyte.OBJ:	..\core\mbyte.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\mbyte.obj ..\core\mbyte.c



$(OUTPUTDIR)\memccpy.OBJ:	..\core\memccpy.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\memccpy.obj ..\core\memccpy.c



$(OUTPUTDIR)\mktemp.OBJ:	..\core\mktemp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\mktemp.obj ..\core\mktemp.c



$(OUTPUTDIR)\nmsize.OBJ:	..\core\nmsize.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\nmsize.obj ..\core\nmsize.c



$(OUTPUTDIR)\okbigbuf.OBJ:	..\core\okbigbuf.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\okbigbuf.obj ..\core\okbigbuf.c



$(OUTPUTDIR)\onexit.OBJ:	..\core\onexit.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\onexit.obj ..\core\onexit.c



$(OUTPUTDIR)\perror.OBJ:	..\core\perror.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\perror.obj ..\core\perror.c



$(OUTPUTDIR)\poly.OBJ:	..\core\poly.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\poly.obj ..\core\poly.c



$(OUTPUTDIR)\pow.OBJ:	..\core\pow.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\pow.obj ..\core\pow.c



$(OUTPUTDIR)\printf.OBJ:	..\core\printf.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\printf.obj ..\core\printf.c



$(OUTPUTDIR)\ptrchk.OBJ:	..\core16\ptrchk.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ptrchk.obj ..\core16\ptrchk.c



$(OUTPUTDIR)\purecall.OBJ:	..\compat16\purecall.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\purecall.obj ..\compat16\purecall.c



$(OUTPUTDIR)\putch.OBJ:	..\core\putch.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\putch.obj ..\core\putch.c



$(OUTPUTDIR)\putchar.OBJ:	..\core\putchar.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\putchar.obj ..\core\putchar.c



$(OUTPUTDIR)\putenv.OBJ:	..\core\putenv.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\putenv.obj ..\core\putenv.c



$(OUTPUTDIR)\puts.OBJ:	..\core\puts.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\puts.obj ..\core\puts.c



$(OUTPUTDIR)\qsort.OBJ:	..\core\qsort.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\qsort.obj ..\core\qsort.c



$(OUTPUTDIR)\rand.OBJ:	..\core\rand.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\rand.obj ..\core\rand.c



$(OUTPUTDIR)\response.OBJ:	..\core\response.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\response.obj ..\core\response.c



$(OUTPUTDIR)\rmtmp.OBJ:	..\core\rmtmp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\rmtmp.obj ..\core\rmtmp.c



$(OUTPUTDIR)\round.OBJ:	..\core\round.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\round.obj ..\core\round.c



$(OUTPUTDIR)\sbrk.OBJ:	..\win16\sbrk.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\sbrk.obj ..\win16\sbrk.c



$(OUTPUTDIR)\scanf.OBJ:	..\core\scanf.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\scanf.obj ..\core\scanf.c



$(OUTPUTDIR)\setdoser.OBJ:	..\core\setdoser.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\setdoser.obj ..\core\setdoser.c



$(OUTPUTDIR)\setenvp.OBJ:	..\core\setenvp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\setenvp.obj ..\core\setenvp.c



$(OUTPUTDIR)\seterrno.OBJ:	..\core\seterrno.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\seterrno.obj ..\core\seterrno.c



$(OUTPUTDIR)\setlc.OBJ:	..\core\setlc.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\setlc.obj ..\core\setlc.c



$(OUTPUTDIR)\setmode.OBJ:	..\core\setmode.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\setmode.obj ..\core\setmode.c



$(OUTPUTDIR)\signal.OBJ:	..\core\signal.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\signal.obj ..\core\signal.c



$(OUTPUTDIR)\sin.OBJ:	..\core\sin.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\sin.obj ..\core\sin.c



$(OUTPUTDIR)\sleep.OBJ:	..\core\sleep.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\sleep.obj ..\core\sleep.c



$(OUTPUTDIR)\sopenf.OBJ:	..\core\sopenf.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\sopenf.obj ..\core\sopenf.c



$(OUTPUTDIR)\spawn.OBJ:	..\core\spawn.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\spawn.obj ..\core\spawn.c



$(OUTPUTDIR)\spawnl.OBJ:	..\core\spawnl.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\spawnl.obj ..\core\spawnl.c



$(OUTPUTDIR)\spawnle.OBJ:	..\core\spawnle.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\spawnle.obj ..\core\spawnle.c



$(OUTPUTDIR)\spawnlp.OBJ:	..\core\spawnlp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\spawnlp.obj ..\core\spawnlp.c



$(OUTPUTDIR)\spawnlpe.OBJ:	..\core\spawnlpe.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\spawnlpe.obj ..\core\spawnlpe.c



$(OUTPUTDIR)\spawnv.OBJ:	..\core\spawnv.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\spawnv.obj ..\core\spawnv.c



$(OUTPUTDIR)\spawnve.OBJ:	..\core\spawnve.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\spawnve.obj ..\core\spawnve.c



$(OUTPUTDIR)\spawnvp.OBJ:	..\core\spawnvp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\spawnvp.obj ..\core\spawnvp.c



$(OUTPUTDIR)\spawnvpe.OBJ:	..\core\spawnvpe.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\spawnvpe.obj ..\core\spawnvpe.c



$(OUTPUTDIR)\splitpat.OBJ:	..\core\splitpat.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\splitpat.obj ..\core\splitpat.c



$(OUTPUTDIR)\sqrt.OBJ:	..\core\sqrt.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\sqrt.obj ..\core\sqrt.c



$(OUTPUTDIR)\stack.OBJ:	..\core\stack.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\stack.obj ..\core\stack.c



$(OUTPUTDIR)\stat.OBJ:	..\core\stat.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\stat.obj ..\core\stat.c



$(OUTPUTDIR)\strcspn.OBJ:	..\core\strcspn.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strcspn.obj ..\core\strcspn.c



$(OUTPUTDIR)\strdup.OBJ:	..\core\strdup.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strdup.obj ..\core\strdup.c



$(OUTPUTDIR)\strerror.OBJ:	..\core\strerror.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strerror.obj ..\core\strerror.c



$(OUTPUTDIR)\strf.OBJ:	..\core\strf.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strf.obj ..\core\strf.c



$(OUTPUTDIR)\strlwr.OBJ:	..\core\strlwr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strlwr.obj ..\core\strlwr.c



$(OUTPUTDIR)\strnicmp.OBJ:	..\core\strnicmp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strnicmp.obj ..\core\strnicmp.c



$(OUTPUTDIR)\strnset.OBJ:	..\core\strnset.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strnset.obj ..\core\strnset.c



$(OUTPUTDIR)\strpbrk.OBJ:	..\core\strpbrk.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strpbrk.obj ..\core\strpbrk.c



$(OUTPUTDIR)\strrchr.OBJ:	..\core\strrchr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strrchr.obj ..\core\strrchr.c



$(OUTPUTDIR)\strrev.OBJ:	..\core\strrev.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strrev.obj ..\core\strrev.c



$(OUTPUTDIR)\strset.OBJ:	..\core\strset.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strset.obj ..\core\strset.c



$(OUTPUTDIR)\strspn.OBJ:	..\core\strspn.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strspn.obj ..\core\strspn.c



$(OUTPUTDIR)\strstr.OBJ:	..\core\strstr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strstr.obj ..\core\strstr.c



$(OUTPUTDIR)\strtod.OBJ:	..\core\strtod.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strtod.obj ..\core\strtod.c



$(OUTPUTDIR)\strtof.OBJ:	..\core\strtof.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strtof.obj ..\core\strtof.c



$(OUTPUTDIR)\strtok.OBJ:	..\core16\strtok.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strtok.obj ..\core16\strtok.c



$(OUTPUTDIR)\strtol.OBJ:	..\core\strtol.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strtol.obj ..\core\strtol.c



$(OUTPUTDIR)\strupr.OBJ:	..\core\strupr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strupr.obj ..\core\strupr.c



$(OUTPUTDIR)\strxfrm.OBJ:	..\core\strxfrm.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strxfrm.obj ..\core\strxfrm.c



$(OUTPUTDIR)\swab.OBJ:	..\core\swab.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\swab.obj ..\core\swab.c



$(OUTPUTDIR)\system.OBJ:	..\core\system.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\system.obj ..\core\system.c



$(OUTPUTDIR)\tabsize.OBJ:	..\core\tabsize.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\tabsize.obj ..\core\tabsize.c



$(OUTPUTDIR)\tan.OBJ:	..\core\tan.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\tan.obj ..\core\tan.c



$(OUTPUTDIR)\tchar.OBJ:	..\core\tchar.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\tchar.obj ..\core\tchar.c



$(OUTPUTDIR)\time.OBJ:	..\core\time.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\time.obj ..\core\time.c



$(OUTPUTDIR)\time3.OBJ:	..\core\time3.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\time3.obj ..\core\time3.c



$(OUTPUTDIR)\timesup.OBJ:	..\core\timesup.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\timesup.obj ..\core\timesup.c



$(OUTPUTDIR)\tmpnam.OBJ:	..\core\tmpnam.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\tmpnam.obj ..\core\tmpnam.c



$(OUTPUTDIR)\toascii.OBJ:	..\core\toascii.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\toascii.obj ..\core\toascii.c



$(OUTPUTDIR)\tolower.OBJ:	..\core\tolower.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\tolower.obj ..\core\tolower.c



$(OUTPUTDIR)\toupper.OBJ:	..\core\toupper.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\toupper.obj ..\core\toupper.c



$(OUTPUTDIR)\trigerr.OBJ:	..\core\trigerr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\trigerr.obj ..\core\trigerr.c



$(OUTPUTDIR)\ultoa.OBJ:	..\core16\ultoa.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ultoa.obj ..\core16\ultoa.c



$(OUTPUTDIR)\umask.OBJ:	..\core\umask.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\umask.obj ..\core\umask.c



$(OUTPUTDIR)\ungetc.OBJ:	..\core\ungetc.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ungetc.obj ..\core\ungetc.c



$(OUTPUTDIR)\unmangle.OBJ:	..\core\unmangle.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\unmangle.obj ..\core\unmangle.c



$(OUTPUTDIR)\vprintf.OBJ:	..\core\vprintf.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\vprintf.obj ..\core\vprintf.c



$(OUTPUTDIR)\wcscpy.OBJ:	..\core\wcscpy.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcscpy.obj ..\core\wcscpy.c



$(OUTPUTDIR)\wcslen.OBJ:	..\core\wcslen.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wcslen.obj ..\core\wcslen.c



$(OUTPUTDIR)\wep.OBJ:	..\win16\wep.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\wep.obj ..\win16\wep.c



$(OUTPUTDIR)\winerr.OBJ:	..\win16\winerr.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\winerr.obj ..\win16\winerr.c



$(OUTPUTDIR)\winmouse.OBJ:	..\win16\winmouse.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\winmouse.obj ..\win16\winmouse.c



$(OUTPUTDIR)\winsleep.OBJ:	..\win16\winsleep.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\winsleep.obj ..\win16\winsleep.c



$(OUTPUTDIR)\_delete.OBJ:	..\core\_delete.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_delete.obj ..\core\_delete.cpp



$(OUTPUTDIR)\_delnear.OBJ:	..\core\_delnear.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_delnear.obj ..\core\_delnear.cpp



$(OUTPUTDIR)\_handler.OBJ:	..\core\_handler.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_handler.obj ..\core\_handler.cpp



$(OUTPUTDIR)\_new.OBJ:	..\core\_new.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_new.obj ..\core\_new.cpp



$(OUTPUTDIR)\_new2.OBJ:	..\core\_new2.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_new2.obj ..\core\_new2.cpp



$(OUTPUTDIR)\_newnear.OBJ:	..\core\_newnear.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_newnear.obj ..\core\_newnear.cpp



$(OUTPUTDIR)\complex.OBJ:	..\core\complex.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\complex.obj ..\core\complex.cpp



$(OUTPUTDIR)\coperat.OBJ:	..\core\coperat.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\coperat.obj ..\core\coperat.cpp



$(OUTPUTDIR)\cstrm.OBJ:	..\core\cstrm.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\cstrm.obj ..\core\cstrm.cpp



$(OUTPUTDIR)\ctrig.OBJ:	..\core\ctrig.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ctrig.obj ..\core\ctrig.cpp



$(OUTPUTDIR)\dbp.OBJ:	..\ios\dbp.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\dbp.obj ..\ios\dbp.cpp



$(OUTPUTDIR)\except.OBJ:	..\core\except.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\except.obj ..\core\except.cpp



$(OUTPUTDIR)\fstream.OBJ:	..\ios\fstream.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\fstream.obj ..\ios\fstream.cpp



$(OUTPUTDIR)\generr.OBJ:	..\ios\generr.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\generr.obj ..\ios\generr.cpp



$(OUTPUTDIR)\ios.OBJ:	..\ios\ios.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ios.obj ..\ios\ios.cpp



$(OUTPUTDIR)\iostream.OBJ:	..\ios\iostream.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\iostream.obj ..\ios\iostream.cpp



$(OUTPUTDIR)\istream.OBJ:	..\ios\istream.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\istream.obj ..\ios\istream.cpp



$(OUTPUTDIR)\istreamf.OBJ:	..\ios\istreamf.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\istreamf.obj ..\ios\istreamf.cpp



$(OUTPUTDIR)\istreami.OBJ:	..\ios\istreami.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\istreami.obj ..\ios\istreami.cpp



$(OUTPUTDIR)\istreamx.OBJ:	..\ios\istreamx.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\istreamx.obj ..\ios\istreamx.cpp



$(OUTPUTDIR)\manip.OBJ:	..\ios\manip.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\manip.obj ..\ios\manip.cpp



$(OUTPUTDIR)\ostream.OBJ:	..\ios\ostream.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ostream.obj ..\ios\ostream.cpp



$(OUTPUTDIR)\ostreamf.OBJ:	..\ios\ostreamf.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ostreamf.obj ..\ios\ostreamf.cpp



$(OUTPUTDIR)\ostreami.OBJ:	..\ios\ostreami.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ostreami.obj ..\ios\ostreami.cpp



$(OUTPUTDIR)\ostreamx.OBJ:	..\ios\ostreamx.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ostreamx.obj ..\ios\ostreamx.cpp



$(OUTPUTDIR)\rtti.OBJ:	..\core\rtti.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\rtti.obj ..\core\rtti.cpp



$(OUTPUTDIR)\siosync.OBJ:	..\ios\siosync.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\siosync.obj ..\ios\siosync.cpp



$(OUTPUTDIR)\stdiostr.OBJ:	..\ios\stdiostr.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\stdiostr.obj ..\ios\stdiostr.cpp



$(OUTPUTDIR)\stream.OBJ:	..\ios\stream.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\stream.obj ..\ios\stream.cpp



$(OUTPUTDIR)\streambf.OBJ:	..\ios\streambf.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\streambf.obj ..\ios\streambf.cpp



$(OUTPUTDIR)\strstrea.OBJ:	..\ios\strstrea.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\strstrea.obj ..\ios\strstrea.cpp



$(OUTPUTDIR)\typeinfo.OBJ:	..\core\typeinfo.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\typeinfo.obj ..\core\typeinfo.cpp



$(OUTPUTDIR)\vecnew.OBJ:	..\core\vecnew.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\vecnew.obj ..\core\vecnew.cpp



$(OUTPUTDIR)\vector.OBJ:	..\core\vector.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\vector.obj ..\core\vector.cpp



$(OUTPUTDIR)\scver.OBJ:	..\core\scver.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\scver.obj ..\core\scver.c



$(OUTPUTDIR)\libentry.OBJ:	..\win16\libentry.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\libentry.obj ..\win16\libentry.c



$(OUTPUTDIR)\_alloca.OBJ:	..\core\_alloca.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_alloca.obj ..\core\_alloca.c



$(OUTPUTDIR)\alloca.OBJ:	..\core\alloca.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\alloca.obj ..\core\alloca.c



$(OUTPUTDIR)\mschkstk.OBJ:	..\compat16\mschkstk.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\mschkstk.obj ..\compat16\mschkstk.c



$(OUTPUTDIR)\anew.OBJ:	..\core\anew.cpp
		$(CC) $(CFLAGS_2) $(DEFINES_2) $(INCLUDES) -o$(OUTPUTDIR)\anew.obj ..\core\anew.cpp



$(OUTPUTDIR)\anew2.OBJ:	..\core\anew2.cpp
		$(CC) $(CFLAGS_3) $(DEFINES_3) $(INCLUDES) -o$(OUTPUTDIR)\anew2.obj ..\core\anew2.cpp



$(OUTPUTDIR)\adelete.OBJ:	..\core\adelete.cpp
		$(CC) $(CFLAGS_1) $(DEFINES_1) $(INCLUDES) -o$(OUTPUTDIR)\adelete.obj ..\core\adelete.cpp



$(OUTPUTDIR)\corenew.OBJ:	..\core\corenew.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\corenew.obj ..\core\corenew.cpp



$(OUTPUTDIR)\purevirt.OBJ:	..\core\purevirt.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\purevirt.obj ..\core\purevirt.c



$(OUTPUTDIR)\ehdata.OBJ:	..\core\ehdata.cpp
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ehdata.obj ..\core\ehdata.cpp



$(OUTPUTDIR)\_ISMBSLD.OBJ:	..\CORE\_ISMBSLD.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_ISMBSLD.obj ..\CORE\_ISMBSLD.C



$(OUTPUTDIR)\_ISMBSTL.OBJ:	..\CORE\_ISMBSTL.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\_ISMBSTL.obj ..\CORE\_ISMBSTL.C



$(OUTPUTDIR)\ISMBALNU.OBJ:	..\CORE\ISMBALNU.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ISMBALNU.obj ..\CORE\ISMBALNU.C



$(OUTPUTDIR)\ISMBALPH.OBJ:	..\CORE\ISMBALPH.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ISMBALPH.obj ..\CORE\ISMBALPH.C



$(OUTPUTDIR)\ISMBBYTE.OBJ:	..\CORE\ISMBBYTE.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ISMBBYTE.obj ..\CORE\ISMBBYTE.C



$(OUTPUTDIR)\ISMBCL0.OBJ:	..\CORE\ISMBCL0.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ISMBCL0.obj ..\CORE\ISMBCL0.C



$(OUTPUTDIR)\ISMBCL1.OBJ:	..\CORE\ISMBCL1.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ISMBCL1.obj ..\CORE\ISMBCL1.C



$(OUTPUTDIR)\ISMBCL2.OBJ:	..\CORE\ISMBCL2.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ISMBCL2.obj ..\CORE\ISMBCL2.C



$(OUTPUTDIR)\ISMBDIGI.OBJ:	..\CORE\ISMBDIGI.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ISMBDIGI.obj ..\CORE\ISMBDIGI.C



$(OUTPUTDIR)\ISMBGRAP.OBJ:	..\CORE\ISMBGRAP.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ISMBGRAP.obj ..\CORE\ISMBGRAP.C



$(OUTPUTDIR)\ISMBHIRA.OBJ:	..\CORE\ISMBHIRA.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ISMBHIRA.obj ..\CORE\ISMBHIRA.C



$(OUTPUTDIR)\ISMBKATA.OBJ:	..\CORE\ISMBKATA.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ISMBKATA.obj ..\CORE\ISMBKATA.C



$(OUTPUTDIR)\ISMBLEGA.OBJ:	..\CORE\ISMBLEGA.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ISMBLEGA.obj ..\CORE\ISMBLEGA.C



$(OUTPUTDIR)\ISMBLOWE.OBJ:	..\CORE\ISMBLOWE.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ISMBLOWE.obj ..\CORE\ISMBLOWE.C



$(OUTPUTDIR)\ISMBPRIN.OBJ:	..\CORE\ISMBPRIN.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ISMBPRIN.obj ..\CORE\ISMBPRIN.C



$(OUTPUTDIR)\ISMBPUNC.OBJ:	..\CORE\ISMBPUNC.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ISMBPUNC.obj ..\CORE\ISMBPUNC.C



$(OUTPUTDIR)\ISMBSPAC.OBJ:	..\CORE\ISMBSPAC.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ISMBSPAC.obj ..\CORE\ISMBSPAC.C



$(OUTPUTDIR)\ISMBSYMB.OBJ:	..\CORE\ISMBSYMB.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ISMBSYMB.obj ..\CORE\ISMBSYMB.C



$(OUTPUTDIR)\ISMBUPPE.OBJ:	..\CORE\ISMBUPPE.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\ISMBUPPE.obj ..\CORE\ISMBUPPE.C



$(OUTPUTDIR)\MBBTOMBC.OBJ:	..\CORE\MBBTOMBC.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBBTOMBC.obj ..\CORE\MBBTOMBC.C



$(OUTPUTDIR)\MBBTYPE.OBJ:	..\CORE\MBBTYPE.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBBTYPE.obj ..\CORE\MBBTYPE.C



$(OUTPUTDIR)\MBCCPY.OBJ:	..\CORE\MBCCPY.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBCCPY.obj ..\CORE\MBCCPY.C



$(OUTPUTDIR)\MBCLEN.OBJ:	..\CORE\MBCLEN.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBCLEN.obj ..\CORE\MBCLEN.C



$(OUTPUTDIR)\MBCPINFO.OBJ:	..\CORE\MBCPINFO.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBCPINFO.obj ..\CORE\MBCPINFO.C



$(OUTPUTDIR)\MBJISJMS.OBJ:	..\CORE\MBJISJMS.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBJISJMS.obj ..\CORE\MBJISJMS.C



$(OUTPUTDIR)\MBJMSJIS.OBJ:	..\CORE\MBJMSJIS.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBJMSJIS.obj ..\CORE\MBJMSJIS.C



$(OUTPUTDIR)\MBLEN.OBJ:	..\CORE\MBLEN.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBLEN.obj ..\CORE\MBLEN.C



$(OUTPUTDIR)\MBSBTYPE.OBJ:	..\CORE\MBSBTYPE.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSBTYPE.obj ..\CORE\MBSBTYPE.C



$(OUTPUTDIR)\MBSCHR.OBJ:	..\CORE\MBSCHR.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSCHR.obj ..\CORE\MBSCHR.C



$(OUTPUTDIR)\MBSCMP.OBJ:	..\CORE\MBSCMP.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSCMP.obj ..\CORE\MBSCMP.C



$(OUTPUTDIR)\MBSCOLL.OBJ:	..\CORE\MBSCOLL.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSCOLL.obj ..\CORE\MBSCOLL.C



$(OUTPUTDIR)\MBSCSPN.OBJ:	..\CORE\MBSCSPN.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSCSPN.obj ..\CORE\MBSCSPN.C



$(OUTPUTDIR)\MBSDEC.OBJ:	..\CORE\MBSDEC.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSDEC.obj ..\CORE\MBSDEC.C



$(OUTPUTDIR)\MBSICMP.OBJ:	..\CORE\MBSICMP.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSICMP.obj ..\CORE\MBSICMP.C



$(OUTPUTDIR)\MBSICOLL.OBJ:	..\CORE\MBSICOLL.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSICOLL.obj ..\CORE\MBSICOLL.C



$(OUTPUTDIR)\MBSINC.OBJ:	..\CORE\MBSINC.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSINC.obj ..\CORE\MBSINC.C



$(OUTPUTDIR)\MBSLEN.OBJ:	..\CORE\MBSLEN.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSLEN.obj ..\CORE\MBSLEN.C



$(OUTPUTDIR)\MBSLWR.OBJ:	..\CORE\MBSLWR.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSLWR.obj ..\CORE\MBSLWR.C



$(OUTPUTDIR)\MBSNBCAT.OBJ:	..\CORE\MBSNBCAT.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSNBCAT.obj ..\CORE\MBSNBCAT.C



$(OUTPUTDIR)\MBSNBCMP.OBJ:	..\CORE\MBSNBCMP.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSNBCMP.obj ..\CORE\MBSNBCMP.C



$(OUTPUTDIR)\MBSNBCNT.OBJ:	..\CORE\MBSNBCNT.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSNBCNT.obj ..\CORE\MBSNBCNT.C



$(OUTPUTDIR)\MBSNBCOL.OBJ:	..\CORE\MBSNBCOL.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSNBCOL.obj ..\CORE\MBSNBCOL.C



$(OUTPUTDIR)\MBSNBCPY.OBJ:	..\CORE\MBSNBCPY.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSNBCPY.obj ..\CORE\MBSNBCPY.C



$(OUTPUTDIR)\MBSNBICM.OBJ:	..\CORE\MBSNBICM.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSNBICM.obj ..\CORE\MBSNBICM.C



$(OUTPUTDIR)\MBSNBICO.OBJ:	..\CORE\MBSNBICO.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSNBICO.obj ..\CORE\MBSNBICO.C



$(OUTPUTDIR)\MBSNBSET.OBJ:	..\CORE\MBSNBSET.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSNBSET.obj ..\CORE\MBSNBSET.C



$(OUTPUTDIR)\MBSNCAT.OBJ:	..\CORE\MBSNCAT.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSNCAT.obj ..\CORE\MBSNCAT.C



$(OUTPUTDIR)\MBSNCCNT.OBJ:	..\CORE\MBSNCCNT.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSNCCNT.obj ..\CORE\MBSNCCNT.C



$(OUTPUTDIR)\MBSNCMP.OBJ:	..\CORE\MBSNCMP.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSNCMP.obj ..\CORE\MBSNCMP.C



$(OUTPUTDIR)\MBSNCOLL.OBJ:	..\CORE\MBSNCOLL.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSNCOLL.obj ..\CORE\MBSNCOLL.C



$(OUTPUTDIR)\MBSNCPY.OBJ:	..\CORE\MBSNCPY.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSNCPY.obj ..\CORE\MBSNCPY.C



$(OUTPUTDIR)\MBSNEXTC.OBJ:	..\CORE\MBSNEXTC.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSNEXTC.obj ..\CORE\MBSNEXTC.C



$(OUTPUTDIR)\MBSNICMP.OBJ:	..\CORE\MBSNICMP.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSNICMP.obj ..\CORE\MBSNICMP.C



$(OUTPUTDIR)\MBSNICOL.OBJ:	..\CORE\MBSNICOL.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSNICOL.obj ..\CORE\MBSNICOL.C



$(OUTPUTDIR)\MBSNINC.OBJ:	..\CORE\MBSNINC.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSNINC.obj ..\CORE\MBSNINC.C



$(OUTPUTDIR)\MBSNSET.OBJ:	..\CORE\MBSNSET.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSNSET.obj ..\CORE\MBSNSET.C



$(OUTPUTDIR)\MBSPBRK.OBJ:	..\CORE\MBSPBRK.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSPBRK.obj ..\CORE\MBSPBRK.C



$(OUTPUTDIR)\MBSRCHR.OBJ:	..\CORE\MBSRCHR.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSRCHR.obj ..\CORE\MBSRCHR.C



$(OUTPUTDIR)\MBSREV.OBJ:	..\CORE\MBSREV.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSREV.obj ..\CORE\MBSREV.C



$(OUTPUTDIR)\MBSSET.OBJ:	..\CORE\MBSSET.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSSET.obj ..\CORE\MBSSET.C



$(OUTPUTDIR)\MBSSPN.OBJ:	..\CORE\MBSSPN.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSSPN.obj ..\CORE\MBSSPN.C



$(OUTPUTDIR)\MBSSPNP.OBJ:	..\CORE\MBSSPNP.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSSPNP.obj ..\CORE\MBSSPNP.C



$(OUTPUTDIR)\MBSSTR.OBJ:	..\CORE\MBSSTR.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSSTR.obj ..\CORE\MBSSTR.C



$(OUTPUTDIR)\MBSTOK.OBJ:	..\CORE\MBSTOK.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSTOK.obj ..\CORE\MBSTOK.C



$(OUTPUTDIR)\MBSTRLEN.OBJ:	..\CORE\MBSTRLEN.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSTRLEN.obj ..\CORE\MBSTRLEN.C



$(OUTPUTDIR)\MBSUPR.OBJ:	..\CORE\MBSUPR.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBSUPR.obj ..\CORE\MBSUPR.C



$(OUTPUTDIR)\MBTOHIRA.OBJ:	..\CORE\MBTOHIRA.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBTOHIRA.obj ..\CORE\MBTOHIRA.C



$(OUTPUTDIR)\MBTOKATA.OBJ:	..\CORE\MBTOKATA.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBTOKATA.obj ..\CORE\MBTOKATA.C



$(OUTPUTDIR)\MBTOLOWE.OBJ:	..\CORE\MBTOLOWE.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBTOLOWE.obj ..\CORE\MBTOLOWE.C



$(OUTPUTDIR)\MBTOUPPE.OBJ:	..\CORE\MBTOUPPE.C
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\MBTOUPPE.obj ..\CORE\MBTOUPPE.C



$(OUTPUTDIR)\initmbcp.OBJ:	..\core\initmbcp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\initmbcp.obj ..\core\initmbcp.c



$(OUTPUTDIR)\setmbcp.OBJ:	..\core\setmbcp.c
		$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$(OUTPUTDIR)\setmbcp.obj ..\core\setmbcp.c




