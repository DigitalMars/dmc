/*
 * Copyright (C) 1986-2004 by Digital Mars
 * tchar.h.  Controls mapping of string routines to either
 * unicode or MBCS or sbcs variants.
 * $Revision: 1.2 $
 */
#if __DMC__ || __RCC__
#pragma once
#endif

#ifndef __TCHAR_H
#define __TCHAR_H 1


# if __cplusplus
extern "C" {
# endif

/* Far Functions */
# if __INTSIZE == 2
#define _ftcscat    _fstrcat
#define _ftcschr    _fstrchr
#define _ftcscmp    _fstrcmp
#define _ftcscpy    _fstrcpy
#define _ftcscspn   _fstrcspn
#define _ftcslen    _fstrlen
#define _ftcsncat   _fstrncat
#define _ftcsncmp   _fstrncmp
#define _ftcsncpy   _fstrncpy
#define _ftcspbrk   _fstrpbrk
#define _ftcsrchr   _fstrrchr
#define _ftcsspn    _fstrspn
#define _ftcsstr    _fstrstr
#define _ftcstok    _fstrtok
#define _ftcsdup    _fstrdup
#define _ftcsicmp   _fstricmp
#define _ftcsnicmp  _fstrnicmp
#define _ftcsnset   _fstrnset
#define _ftcsrev    _fstrrev
#define _ftcsset    _fstrset
#define _ftcslwr    _fstrlwr
#define _ftcsupr    _fstrupr
# else
/* For 32-bit models, no far functions */
#define _ftcscat	_tcscat
#define _ftcschr	_tcschr
#define _ftcscmp	_tcscmp
#define _ftcscpy	_tcscpy
#define _ftcscspn	_tcscspn
#define _ftcsncat	_tcsncat
#define _ftcsncmp	_tcsncmp
#define _ftcsncpy	_tcsncpy
#define _ftcspbrk	_tcspbrk
#define _ftcsrchr	_tcsrchr
#define _ftcsspn	_tcsspn
#define _ftcsstr	_tcsstr
#define _ftcstok	_tcstok
#define _ftcsdup	_tcsdup
#define _ftcsicmp	_tcsicmp
#define _ftcsnicmp	_tcsnicmp
#define _ftcsnset	_tcsnset
#define _ftcsrev	_tcsrev
#define _ftcsset	_tcsset
#define _ftcsclen	_tcsclen
#define _ftcsnccat	_tcsnccat
#define _ftcsnccpy	_tcsnccpy
#define _ftcsnccmp	_tcsnccmp
#define _ftcsncicmp	_tcsncicmp
#define _ftcsncset	_tcsncset
#define	_ftcsdec	_tcsdec
#define	_ftcsinc	_tcsinc
#define	_ftcsnbcnt	_tcsnccnt
#define	_ftcsnccnt	_tcsnccnt
#define	_ftcsnextc	_tcsnextc
#define	_ftcsninc	_tcsninc
#define	_ftcsspnp	_tcsspnp
#define _ftcslwr	_tcslwr
#define _ftcsupr	_tcsupr
#define _ftcslen	_tcslen
#define _ftclen		_tclen
#define	_ftccpy		_tccpy
#define _ftccmp		_tccmp
# endif /* __INTSIZE == 2 */

# ifdef	_UNICODE
/* Current UNICODE Support */

#  ifndef UNICODE
#define UNICODE
#  endif

#include <wchar.h>

typedef wchar_t wint_t;
typedef wchar_t wctype_t;
typedef wchar_t _TCHAR;
typedef wchar_t _TSCHAR;
typedef wchar_t _TUCHAR;
typedef wchar_t _TXCHAR;
typedef wint_t  _TINT;

#if !defined(_TCHAR_DEFINED)
#ifndef __STDC__
typedef wchar_t		TCHAR;
#endif
#define _TCHAR_DEFINED 1
#endif

#define _TEOF		WEOF
#define __T(x)		L##x

/* ProgramEntry/Environment */
#define _tmain		wmain
#define _tWinMain	wWinMain
#define __targv		__wargv
#define _tenviron	_wenviron
#define _tcmdln		_wcmdln
#define _tpgmptr	_wpgmptr
#define __tparsecmdline __wparsecmdline

/* Formatted I/O */
#define _tprintf	wprintf
#define _ftprintf	fwprintf
#define _stprintf	_swprintf
#define _sntprintf	_snwprintf
#define _vtprintf	vwprintf
#define _vftprintf	vfwprintf
#define _vstprintf	vswprintf
#define _vsntprintf	_vsnwprintf
#define _tscanf		wscanf
#define _ftscanf	fwscanf
#define _stscanf	swscanf

/* Unformatted I/O */
#define _fgettc		fgetwc
#define _fgettchar	_fgetwchar
#define _fgetts		fgetws
#define _fputtc		fputwc
#define _fputtchar	_fputwchar
#define _fputts		fputws
#define _gettc		getwc
#define _gettchar	getwchar
#define _getts		_getws
#define _puttc		putwc
#define _puttchar	putwchar
#define _putts		_putws
#define _ungettc	ungetwc

/* String conversion functions */
#define _tcstold	wcstold
#define _tcstod		wcstod
#define _tcstol		wcstol
#define _tcstoul	wcstoul
#define _itot		_itow
#define _ltot		_ltow
#define _ultot		_ultow
#define _ttoi		_wtoi
#define _ttol		_wtol

/* String functions */
#define _tcscat		wcscat
#define _tcschr		wcschr
#define _tcscpy		wcscpy
#define _tcscspn	wcscspn
#define _tcslen		wcslen
#define _tcsncat	wcsncat
#define _tcsncpy	wcsncpy
#define _tcspbrk	wcspbrk
#define _tcsrchr	wcsrchr
#define _tcsspn		wcsspn
#define _tcsstr		wcsstr
#define _tcstok(a,b)	wcstok(a,b,NULL)
#define _tcsdup		_wcsdup
#define _tcsnset	_wcsnset
#define _tcsrev		_wcsrev
#define _tcsset		_wcsset
#define _tcscmp		wcscmp
#define _tcsicmp	_wcsicmp
#define _tcsnccmp	wcsncmp
#define _tcsncmp	wcsncmp
#define _tcsncicmp	_wcsnicmp
#define _tcsnicmp	_wcsnicmp
#define _tcscoll	wcscoll
#define _tcsicoll	_wcsicoll
#define _tcsnccoll	_wcsncoll
#define _tcsncoll	_wcsncoll
#define _tcsncicoll	_wcsnicoll
#define _tcsnicoll	_wcsnicoll

/* Execute functions */
#define _texecl		_wexecl
#define _texecle	_wexecle
#define _texeclp	_wexeclp
#define _texeclpe	_wexeclpe
#define _texecv		_wexecv
#define _texecve	_wexecve
#define _texecvp	_wexecvp
#define _texecvpe	_wexecvpe
#define _tspawnl	_wspawnl
#define _tspawnle	_wspawnle
#define _tspawnlp	_wspawnlp
#define _tspawnlpe	_wspawnlpe
#define _tspawnv	_wspawnv
#define _tspawnve	_wspawnve
#define _tspawnvp	_wspawnvp
#define _tspawnvp	_wspawnvp
#define _tspawnvpe	_wspawnvpe
#define _tsystem	_wsystem

/* Time functions */
#define _tasctime	_wasctime
#define _tctime		_wctime
#define _tstrdate	_wstrdate
#define _tstrtime	_wstrtime
#define _tutime		_wutime
#define _tcsftime	wcsftime

/* Directory functions */
#define _tchdir		_wchdir
#define _tgetcwd	_wgetcwd
#define _tgetdcwd	_wgetdcwd
#define _tmkdir		_wmkdir
#define _trmdir		_wrmdir

/* Environment/Path functions */
#define _tfullpath	_wfullpath
#define _tgetenv	_wgetenv
#define _tmakepath	_wmakepath
#define _tputenv	_wputenv
#define _tsearchenv	_wsearchenv
#define _tsplitpath	_wsplitpath

/* Stdio functions */
#define _tfdopen	_wfdopen
#define _tfsopen	_wfsopen
#define _tfopen		_wfopen
#define _tfreopen	_wfreopen
#define _tperror	_wperror
#define _tpopen		_wpopen
#define _ttempnam	_wtempnam
#define _ttmpnam	_wtmpnam

/* Io functions */
#define _taccess	_waccess
#define _tchmod		_wchmod
#define _tcreat		_wcreat
#define _tfindfirst	_wfindfirst
#define _tfindnext	_wfindnext
#define _tmktemp	_wmktemp
#define _topen		_wopen
#define _tremove	_wremove
#define _trename	_wrename
#define _tsopen		_wsopen
#define _tunlink	_wunlink
#define _tfinddata_t	_wfinddata_t
#define _tFIND		FINDW

/* Stat functions */
#define _tstat		_wstat

/* Setlocale functions */
#define _tsetlocale	_wsetlocale

/* Redundant "logical-character" mappings */
#define _tcsclen	wcslen
#define _tcsnccat	wcsncat
#define _tcsnccpy	wcsncpy
#define _tcsncset	_wcsnset
#define	_tcsdec		_wcsdec
#define	_tcsinc		_wcsinc
#define	_tcsnbcnt	_wcsncnt
#define	_tcsnccnt	_wcsncnt
#define	_tcsnextc	_wcsnextc
#define	_tcsninc	_wcsninc
#define	_tcsspnp	_wcsspnp
#define _tcslwr		_wcslwr
#define _tcsupr		_wcsupr
#define _tcsxfrm	wcsxfrm

#   if __STDC__ || defined(_NO_INLINING)
#define _tccpy(_pc1,_cpc2) (*(_pc1) = *(_cpc2))
#define _tccmp(_cpc1,_cpc2) (((wchar_t)*(_cpc1))-((wchar_t)*(_cpc2)))
#   else /* __STDC__ */
__inline void __cdecl _tccpy(wchar_t *_pc1, const wchar_t *_cpc2) { *_pc1 = *_cpc2; }
__inline int __cdecl _tccmp(const wchar_t *_cpc1, const wchar_t *_cpc2) { return (int) (((wchar_t)*_cpc1)-((wchar_t)*_cpc2)); }
#   endif /* __STDC__ */

/* Ctype functions */
#define _istalnum	iswalnum
#define _istalpha	iswalpha
#define _istascii	iswascii
#define _istcntrl	iswcntrl
#define _istdigit	iswdigit
#define _istgraph	iswgraph
#define _istlower	iswlower
#define _istprint	iswprint
#define _istpunct	iswpunct
#define _istspace	iswspace
#define _istupper	iswupper
#define _istxdigit	iswxdigit
#define _totupper	towupper
#define _totlower	towlower

/* No Lead bytes etc. for UNICODE */
#define _istlegal(_c)	(1)
#define _istlead(_c)	(0)
#define _istleadbyte(_c)	(0)

/* Inlined Routines: For -A, these are Macros, _NO_INLINING is for MS Compatability */
#  if __STDC__ || defined(_NO_INLINING)
#define _wcsdec(_ws1, _ws2) ((_ws2)-1)
#define _wcsinc(_ws) ((_ws)+1)
#define _wcsnextc(_cws)	((unsigned int) *(_cws))
#define _tclen(_ws)	(1)
#  else
__inline wchar_t * __cdecl _wcsinc(const wchar_t *_ws) { return (wchar_t *)(_ws + 1); }
__inline wchar_t * __cdecl _wcsdec(const wchar_t * _ws1, const wchar_t * _ws2) { return (wchar_t *)(_ws2-1); }
__inline unsigned int __cdecl _wcsnextc(const wchar_t * _cws) { return (unsigned int)*_cws; }
__inline size_t __cdecl _tclen(const wchar_t *_ws) { return 1; }
#  endif

# else /* ndef _UNICODE */
/* Single Byte and Multi-Byte Character Support */
#include <string.h>

#define _TEOF		EOF
#define __T(x)		x

/* ProgramEntry/Environment */
#define _tmain		main
#define _tWinMain	WinMain
#define __targv		__argv
#define __tparsecmdline __parsecmdline
#  ifdef _POSIX_
#define _tenviron	environ
#  else
#define _tenviron	_environ
#  endif
#define _tcmdln		_acmdln
#define _tpgmptr	_pgmptr

/* Formatted I/O */
#define _tprintf	printf
#define _ftprintf	fprintf
#define _stprintf	sprintf
#define _sntprintf	_snprintf
#define _vtprintf	vprintf
#define _vftprintf	vfprintf
#define _vstprintf	vsprintf
#define _vsntprintf	_vsnprintf
#define _tscanf		scanf
#define _ftscanf	fscanf
#define _stscanf	sscanf

/* Unformatted I/O */
#define _fgettc		fgetc
#define _fgettchar	_fgetchar
#define _fgetts		fgets
#define _fputtc		fputc
#define _fputtchar	_fputchar
#define _fputts		fputs
#define _gettc		getc
#define _gettchar	getchar
#define _getts		gets
#define _puttc		putc
#define _puttchar	putchar
#define _putts		puts
#define _ungettc	ungetc

/* String conversion functions */
#define _tcstold	strtold
#define _tcstod		strtod
#define _tcstol		strtol
#define _tcstoul	strtoul
#define _itot		_itoa
#define _ltot		_ltoa
#define _ultot		_ultoa
#define _ttoi		atoi
#define _ttol		atol

/* String functions */
#define _tcscat		strcat
#define _tcscpy		strcpy
#define _tcslen		strlen
#define _tcsxfrm	strxfrm
#define _tcscoll	strcoll
#define _tcsdup		_strdup
#define _tcsicoll	_stricoll
#define _tcsnicoll	_strnicoll

/* Execute functions */
#define _texecl		_execl
#define _texecle	_execle
#define _texeclp	_execlp
#define _texeclpe	_execlpe
#define _texecv		_execv
#define _texecve	_execve
#define _texecvp	_execvp
#define _texecvpe	_execvpe
#define _tspawnl	_spawnl
#define _tspawnle	_spawnle
#define _tspawnlp	_spawnlp
#define _tspawnlpe	_spawnlpe
#define _tspawnv	_spawnv
#define _tspawnve	_spawnve
#define _tspawnvp	_spawnvp
#define _tspawnvpe	_spawnvpe
#define _tsystem	system

/* Time functions */
#define _tasctime	asctime
#define _tctime		ctime
#define _tstrdate	_strdate
#define _tstrtime	_strtime
#define _tutime		utime
#define _tcsftime	strftime

/* Directory functions */
#define _tchdir		_chdir
#define _tgetcwd	_getcwd
#define _tgetdcwd	_getdcwd
#define _tmkdir		_mkdir
#define _trmdir		_rmdir

/* Environment/Path functions */
#define _tfullpath	_fullpath
#define _tgetenv	getenv
#define _tmakepath	_makepath
#define _tputenv	_putenv
#define _tsearchenv	_searchenv
#define _tsplitpath	_splitpath

/* Stdio functions */
#  ifdef _POSIX_
#define _tfdopen	fdopen
#  else
#define _tfdopen	_fdopen
#  endif
#define _tfsopen	_fsopen
#define _tfopen		fopen
#define _tfreopen	freopen
#define _tperror	perror
#define _tpopen		_popen
#define _ttempnam	_tempnam
#define _ttmpnam	tmpnam

/* Io functions */
#define _taccess	_access
#define _tchmod		_chmod
#define _tcreat		_creat
#define _tfindfirst	_findfirst
#define _tfindnext	_findnext
#define _tmktemp	_mktemp
#define _topen		_open
#define _tremove	remove
#define _trename	rename
#define _tsopen		_sopen
#define _tunlink	_unlink
#define _tfinddata_t	_finddata_t
#define _tFIND		FINDA

/* Ctype functions */
#define _istascii	isascii
#define _istcntrl	iscntrl
#define _istxdigit	isxdigit

/* Stat functions */
#define _tstat		_stat

/* Setlocale functions */
#define _tsetlocale	setlocale

#  ifdef _MBCS
/* Multi-byte character support */
#include <mbstring.h>

typedef char            _TCHAR;
typedef signed char     _TSCHAR;
typedef unsigned char   _TUCHAR;
typedef unsigned char   _TXCHAR;
typedef unsigned int    _TINT;

#if !defined(_TCHAR_DEFINED)
#   ifndef __STDC__
typedef char            TCHAR;
#   endif
#define _TCHAR_DEFINED 1
#endif

/* Since the types for the MBCS routines do not
   match those of the single byte routines, there
   has to be some type-safe mappings. The easiest
   way to acomplish is through routines that cast
   appropriately.  By default, these routines are inlined.
*/
#   ifdef _MB_MAP_DIRECT
/* use mb functions directly - types must match */

/* String functions */
#define _tcschr		_mbschr
#define _tcscspn	_mbscspn
#define _tcsncat	_mbsnbcat
#define _tcsncpy	_mbsnbcpy
#define _tcspbrk	_mbspbrk
#define _tcsrchr	_mbsrchr
#define _tcsspn		_mbsspn
#define _tcsstr		_mbsstr
#define _tcstok		_mbstok
#define _tcsnset	_mbsnbset
#define _tcsrev		_mbsrev
#define _tcsset		_mbsset
#define _tcscmp		_mbscmp
#define _tcsicmp	_mbsicmp
#define _tcsnccmp	_mbsncmp
#define _tcsncmp	_mbsnbcmp
#define _tcsncicmp	_mbsnicmp
#define _tcsnicmp	_mbsnbicmp
#define _tcscoll	_mbscoll
#define _tcsicoll	_mbsicoll
#define _tcsnccoll	_mbsncoll
#define _tcsncoll	_mbsnbcoll
#define _tcsncicoll	_mbsnicoll
#define _tcsnicoll	_mbsnbicoll

/* "logical-character" mappings */
#define _tcsclen	_mbslen
#define _tcsnccat	_mbsncat
#define _tcsnccpy	_mbsncpy
#define _tcsncset	_mbsnset

/* MBCS-specific mappings */
#define	_tcsdec		_mbsdec
#define	_tcsinc		_mbsinc
#define	_tcsnbcnt	_mbsnbcnt
#define	_tcsnccnt	_mbsnccnt
#define	_tcsnextc	_mbsnextc
#define	_tcsninc	_mbsninc
#define	_tcsspnp	_mbsspnp
#define _tcslwr		_mbslwr
#define _tcsupr		_mbsupr
#define _tclen		_mbclen
#define _tccpy		_mbccpy

#   elif __STDC__ || defined(_NO_INLINING)

/* String functions */
char * __cdecl _tcschr(const char *, unsigned int);
int __cdecl _tcscmp(const char *, const char *);
size_t __cdecl _tcscspn(const char *, const char *);
int __cdecl _tcsncmp(const char *, const char *, size_t);
char * __cdecl _tcsncpy(char *, const char *, size_t);
char * __cdecl _tcspbrk(const char *, const char *);
char * __cdecl _tcsrchr(const char *, int);
size_t __cdecl _tcsspn(const char *, const char *);
char * __cdecl _tcsstr(const char *, const char *);
char * __cdecl _tcstok(char *, const char *);
int __cdecl _tcsicmp(const char *, const char *);
int __cdecl _tcsnicmp(const char *, const char *, size_t);
char * __cdecl _tcsrev(char *);

/* "logical-character" mappings */
size_t __cdecl _tcsclen(const char *);
int __cdecl _tcsnccmp(const char *,const char *,size_t);

/* MBCS-specific mappings */
char * __cdecl _tcsdec(const char *, const char *);
char * __cdecl _tcsinc(const char *);
char * __cdecl _tcsninc(const char *_s1,size_t _n);
size_t __cdecl _tclen(const char *);

/* Helper */
unsigned int __cdecl _tcsnextc(const char *);

#   else

/* String functions */
__inline char * _tcschr(const char *_s1,unsigned int _c) {return (char *)_mbschr((const unsigned char *)_s1,_c);}
__inline int  _tcscmp(const char * _s1,const char * _s2) {return _mbscmp((const unsigned char *)_s1,(const unsigned char *)_s2);}
__inline size_t _tcscspn(const char * _s1,const char * _s2) {return _mbscspn((const unsigned char *)_s1,(const unsigned char *)_s2);}
__inline int _tcsncmp(const char * _s1,const char * _s2,size_t _n) {return _mbsnbcmp((const unsigned char *)_s1,(const unsigned char *)_s2,_n);}
__inline char * _tcsncpy(char * _s1,const char * _s2,size_t _n) {return (char *)_mbsnbcpy((unsigned char *)_s1,(const unsigned char *)_s2,_n);}
__inline char * _tcspbrk(const char * _s1,const char * _s2) {return (char *)_mbspbrk((const unsigned char *)_s1,(const unsigned char *)_s2);}
__inline char * _tcsrchr(const char * _s1,unsigned int _c) {return (char *)_mbsrchr((const unsigned char *)_s1,_c);}
__inline size_t _tcsspn(const char * _s1,const char * _s2) {return _mbsspn((const unsigned char *)_s1,(const unsigned char *)_s2);}
__inline char * _tcsstr(const char * _s1,const char * _s2) {return (char *)_mbsstr((const unsigned char *)_s1,(const unsigned char *)_s2);}
__inline char * _tcstok(char * _s1,const char * _s2) {return (char *)_mbstok((unsigned char *)_s1,(const unsigned char *)_s2);}
__inline int _tcsicmp(const char * _s1,const char * _s2) {return _mbsicmp((const unsigned char *)_s1,(const unsigned char *)_s2);}
__inline int _tcsnicmp(const char * _s1,const char * _s2,size_t _n) {return _mbsnbicmp((const unsigned char *)_s1,(const unsigned char *)_s2,_n);}
__inline char * _tcsrev(char * _s1) {return (char *)_mbsrev((unsigned char *)_s1);}

/* "logical-character" mappings */
__inline size_t _tcsclen(const char * _s1) {return _mbslen((const unsigned char *)_s1);}
__inline int _tcsnccmp(const char * _s1,const char * _s2,size_t _n) {return _mbsncmp((const unsigned char *)_s1,(const unsigned char *)_s2,_n);}

/* MBCS-specific mappings */
__inline char * _tcsdec(const char * _s1,const char * _s2) {return (char *)_mbsdec((const unsigned char *)_s1,(const unsigned char *)_s2);}
__inline char * _tcsinc(const char * _s1) {return (char *)_mbsinc((const unsigned char *)_s1);}
__inline char * _tcsninc(const char *_s1,size_t _n) {return (char *)_mbsninc((const unsigned char *)_s1,_n);}
__inline size_t _tclen(const char * _s1) {return _mbclen((const unsigned char *)_s1);}

/* Helper */
__inline unsigned int _tcsnextc(const char * _s1) {unsigned int _n=0; if (_ismbblead((unsigned int)(unsigned char *)*_s1)) _n=((unsigned int)*_s1++)<<8; _n+=(unsigned int)*_s1; return(_n);}

#   endif

/* MBCS-specific mappings */
#define	_tccmp(_c1,_c2)	_tcsnccmp(_c1,_c2,1)

/* Ctype functions */
#define _istalnum	_ismbcalnum
#define _istalpha	_ismbcalpha
#define _istdigit	_ismbcdigit
#define _istgraph	_ismbcgraph
#define _istlegal	_ismbclegal
#define _istlower	_ismbclower
#define _istprint	_ismbcprint
#define _istpunct	_ismbcpunct
#define _istspace	_ismbcspace
#define _istupper	_ismbcupper
#define _totupper	_mbctoupper
#define _totlower	_mbctolower
#define _istlead	_ismbblead
#define _istleadbyte	isleadbyte

#  else /* !_MBCS */

/* Single-Byte Character routines */
typedef char            _TCHAR;
typedef signed char     _TSCHAR;
typedef unsigned char   _TUCHAR;
typedef char            _TXCHAR;
typedef int             _TINT;

#if !defined(_TCHAR_DEFINED)
#   ifndef __STDC__
typedef char            TCHAR;
#   endif
#define _TCHAR_DEFINED 1
#endif

/* String functions */
#define _tcschr		strchr
#define _tcscmp		strcmp
#define _tcscspn	strcspn
#define _tcsncat	strncat
#define _tcsncmp	strncmp
#define _tcsncpy	strncpy
#define _tcspbrk	strpbrk
#define _tcsrchr	strrchr
#define _tcsspn		strspn
#define _tcsstr		strstr
#define _tcstok		strtok
#define _tcsicmp	_stricmp
#define _tcsnicmp	_strnicmp
#define _tcsnset	_strnset
#define _tcsrev		_strrev
#define _tcsset		_strset

/* "logical-character" mappings */
#define _tcsclen	strlen
#define _tcsnccat	strncat
#define _tcsnccpy	strncpy
#define _tcsnccmp	strncmp
#define _tcsncicmp	_strnicmp
#define _tcsncset	_strnset

/* MBCS-specific functions */
#define	_tcsdec		_strdec
#define	_tcsinc		_strinc
#define	_tcsnbcnt	_strncnt
#define	_tcsnccnt	_strncnt
#define	_tcsnextc	_strnextc

#define	_tcsninc	_strninc
#define	_tcsspnp	_strspnp
#define _tcslwr		_strlwr
#define _tcsupr		_strupr
#define _tcsxfrm	strxfrm
#define _istlead(_c)    (0)
#define _istleadbyte(_c)    (0)

#   if __STDC__ || defined(_NO_INLINING)
#define _tclen(_pc)	(1)
#define _tccpy(_pc1,_cpc2) (*(_pc1) = *(_cpc2))
#define _tccmp(_cpc1,_cpc2) (((unsigned char)*(_cpc1))-((unsigned char)*(_cpc2)))
#   else /* __STDC__ */
__inline size_t __cdecl _tclen(const char *_cpc) { return (_cpc,1); }
__inline void __cdecl _tccpy(char *_pc1, const char *_cpc2) { *_pc1 = *_cpc2; }
__inline int __cdecl _tccmp(const char *_cpc1, const char *_cpc2) { return (int) (((unsigned char)*_cpc1)-((unsigned char)*_cpc2)); }
#   endif /* __STDC__ */

/* Ctype-functions */
#define _istalnum	isalnum
#define _istalpha	isalpha
#define _istdigit	isdigit
#define _istgraph	isgraph
#define _istlower	islower
#define _istprint	isprint
#define _istpunct	ispunct
#define _istspace	isspace
#define _istupper	isupper
#define _totupper	toupper
#define _totlower	tolower
#define _istlegal(_c)   (1)

/* define NULL pointer value */
#   ifndef NULL
#    ifdef __cplusplus
#define NULL	0
#    else
#define NULL	((void *)0)
#    endif
#   endif

#   if __STDC__ || defined(_NO_INLINING)
#define _strdec(_cpc1, _cpc2) ((_cpc2)-1)
#define _strinc(_pc)	((_pc)+1)
#define _strnextc(_cpc)	((unsigned int) *(_cpc))
#define _strninc(_pc, _sz) (((_pc)+(_sz)))
#define _strncnt(_cpc, _sz) ((strlen(_cpc)>_sz) ? _sz : strlen(_cpc))
#define _strspnp(_cpc1, _cpc2) ((*((_cpc1)+strspn(_cpc1,_cpc2))) ? ((_cpc1)+strspn(_cpc1,_cpc2)) : NULL)
#   else /* __STDC__ */
__inline char * __cdecl _strdec(const char * _cpc1, const char * _cpc2) { return (char *)(_cpc1,(_cpc2-1)); }
__inline char * __cdecl _strinc(const char * _pc) { return (char *)(_pc+1); }
__inline unsigned int __cdecl _strnextc(const char * _cpc) { return (unsigned int)*_cpc; }
__inline char * __cdecl _strninc(const char * _pc, size_t _sz) { return (char *)(_pc+_sz); }
__inline size_t __cdecl _strncnt( const char * _cpc, size_t _sz) { size_t len; len = strlen(_cpc); return (len>_sz) ? _sz : len; }
__inline char * __cdecl _strspnp( const char * _cpc1, const char * _cpc2) { return (*(_cpc1 += strspn(_cpc1,_cpc2))!='\0') ? (char*)_cpc1 : NULL; }
#   endif /* __STDC__ */

#  endif /* _MBCS */

# endif /* _UNICODE */

#define _T(x)		__T(x)
#define _TEXT(x)	__T(x)

# if __cplusplus
}
# endif

#endif
