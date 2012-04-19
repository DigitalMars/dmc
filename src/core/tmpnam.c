/*_ tmpnam.c   Sat Sep	1 1990	 Modified by: Walter Bright */
/* Copyright (C) 1985-1995 by Digital Mars */
/* All Rights Reserved				*/
/* $Revision: 1.1.1.1 $ */

#include	<tchar.h>
#include	<sctchar.h>
#include	<stdlib.h>
#include	<stddef.h>
#include	<io.h>
#include	<fcntl.h>
#include	<string.h>
#include	<process.h>
#include	<errno.h>
#if M_UNIX || M_XENIX
#include	<unistd.h>
#include	<sys/stat.h>
#else
#include	<sys\stat.h>
#endif
#if MSDOS || __OS2__ || _WINDOWS ||__NT__
#include	<dos.h>
#endif

#ifdef __OS2__
#include "os2lib.h"
#endif
#ifdef __NT__
#include <windows.h>
#endif
#include <scio.h>

#ifndef _UNICODE
#define TP_TMPDIR	P_tmpdir
#else
#define TP_TMPDIR	wP_tmpdir
#endif

/*******************************
 * Generate temporary filename.
 * At least TMP_MAX different names are tried.
 * Input:
 *	string	Pointer to buffer to store filename into that
 *		is at least L_tmpnam bytes long. If NULL, then
 *		tmpnam() will use an internal static buffer which
 *		is overwritten by each call to tmpnam().
 * Returns:
 *	pointer to filename generated
 *	NULL if cannot generate a filename
 */

#ifndef __NT__
static unsigned __tmpnum;
#else

/* used tmpnam.c, wtmpnam.c, see to tnodata.c
*/
extern char	__seed;
extern unsigned	__tmpnum;
#define seed	__seed
#endif

_TCHAR *_ttmpnam(_TCHAR *string)
{   int save, doserrnosave;
    static _TCHAR buffer[L_tmpnam];
    _TCHAR *fname;
#ifndef __NT__
    static char seed;
#endif
    unsigned u;

    if (!string)
	string = buffer;
    _tcscpy (string, TP_TMPDIR);
    fname = string + _tcslen (string);
    if (!seed)			/* if first time through	*/
    {
	seed++;
#if MSDOS
	__tmpnum = _psp;
#else
	__tmpnum = getpid();
#endif
    }

    save = errno;
    doserrnosave = _doserrno;

    for (u = 0; u < TMP_MAX; u++)
    {
	/* Select a new filename to try	*/
	__tmpnum = (__tmpnum + 1) & 0x7FFF;
	if (__tmpnum == 0)
	    __tmpnum++;
	_itot (__tmpnum, fname, 10);
	errno = 0;
	if (_taccess(string,F_OK) && errno != EACCES)
	    goto L1;
    }
    string = NULL;			/* failed miserably	*/
L1:
    errno = save;
    _doserrno = doserrnosave;
    return string;
}

#ifndef _UNICODE

/***********************************
 * Generate temporary file.
 */

FILE *tmpfile(void)
{
#if MSDOS
    char name[L_tmpnam];

    FILE *fp = fopen(tmpnam(name),"wb+");
    if (fp)
    {
	_tmpnum(fp) = __tmpnum;
    }
    return fp;
#endif
#if __NT__
    char *dir = getenv("TEMP");
    char *name = tempnam(dir, "tmp");
    if (!name)
	return NULL;
    FILE *fp = fopen(name,"wb+");
    if (fp)
    {
	_tmpnum(fp) = __tmpnum;
    }
    free(name);
    return fp;
#endif
}

#endif

/*******************************
 * Generate temporary filename.
 * Use the directory 'dir', use the prefix 'pfx' for the filename.
 * Input:
 *	dir	The subdirectory to put the file in.
 *	pfx	The prefix (up to five characters) for the file prefix.
 *	
 * Returns:
 *	pointer to the malloc'ed filename generated
 *	NULL if filename could not be generated
 */

_TCHAR *_ttempnam (_TCHAR *dir, _TCHAR *pfx)
{   size_t i;
    _TCHAR *ret_val;
#if M_UNIX || M_XENIX
    struct stat buf;
    _TCHAR *ep = _tgetenv(_T("TMPDIR"));
#endif

#if M_UNIX || M_XENIX
    if (ep != NULL && stat(ep, &buf) == 0)
	{
	if (S_ISDIR(buf.st_mode) && (buf.st_mode & S_IWRITE))
		dir = ep;
	}
     else
	if (dir != NULL && stat(dir, &buf) == 0)
		{
		if (!(S_ISDIR(buf.st_mode) && (buf.st_mode & S_IWRITE)))
			dir = TP_TMPDIR;
		}
	else
		dir = TP_TMPDIR;
#else
    if (dir == NULL)
	dir = TP_TMPDIR;
#endif

    i = _tcslen(dir);
    ret_val = malloc((i+2 /* '\0' & '\\' */ + 8 /*root*/ + 4 /*.ext*/) * sizeof (_TCHAR));
    if (ret_val)
    {
	int save, doserrnosave;
	unsigned u;
	unsigned n;

	_tcscpy(ret_val,dir);

	/* Make sure directory ends with a separator	*/
#if MSDOS || __OS2__ || _WINDOWS || __NT__
# ifdef _MBCS
	if (i > 0) {
	  char * bp;
	    bp = _tcsdec (ret_val, ret_val + i);
	    if (*bp != '\\' && *bp != '/' && *bp != ':') {
	        ret_val[i++] = '\\';
	    }
	}
# else
	if(i>0 && ret_val[i-1] != _T('\\') && ret_val[i-1] != _T('/') &&
	   ret_val[i-1] != _T(':'))
	{
	    ret_val[i++] = _T('\\');
	}
# endif
#else
	if(i>0 && ret_val[i-1] != _T('/'))
	    ret_val[i++] = _T('/');
#endif

	_tcsncpy(ret_val + i, pfx, 5);
	ret_val[i + 5] = _T('\0');	/* strncpy doesn't put a 0 if more  */
	i = _tcslen(ret_val);		/* than 'n' chars.		    */
	save = errno;
	doserrnosave = _doserrno;

	/* Prefix can have 5 chars, leaving 3 for numbers.
	   26 ** 3 == 17576
	 */

#if MSDOS
	n = _psp;
#else
	n = getpid();	 	/* starting seed */
#endif 
	for (u = 1; u < 26*26*26; u++)
	{
       n = (n + 1) % (26*26*26);
	    errno = 0;
	    _itot(n,ret_val + i,26);
	    _tcscat(ret_val,_T(".tmp"));
	    if (_taccess(ret_val,F_OK) && errno != EACCES)
		goto L1;
	}
	free(ret_val);
	ret_val = NULL;			/* failed			*/
    L1:
	errno = save;
	_doserrno = doserrnosave;
    }

    return ret_val;
}
