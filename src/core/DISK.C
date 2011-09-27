/* Copyright (C) 1985-1993 by Digital Mars  */
/* All Rights Reserved    */
/* January 5, 1993  */
/* $Revision: 1.1.1.1 $ */

#include <tchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#include <direct.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <scrtl.h>
#include <sctchar.h>

#ifndef _UNICODE

#if !(M_UNIX || M_XENIX)

int __CLIB setdisk(int drive)
{
    unsigned ndrives;

    dos_setdrive (drive+1, &ndrives);
    return ((int)ndrives);
}

int __CLIB getdisk(void)
{
    unsigned drive;

    dos_getdrive (&drive);
    return((int)drive-1);
}

#endif

char *searchpath(const char *file)
{
    static char path [_MAX_PATH];

    path[0] = '\0';
    _searchenv (file, "PATH", path);
    if (path[0] == 0)
        return NULL;
    else
        return path;
}

#endif

#ifndef _NOSEARCHENV

void _tsearchenv(const _TCHAR *filename, const _TCHAR *varname, _TCHAR *pathname)
{
    int counter;
    int done  = 0;
    _TCHAR eachpath[_MAX_PATH+1];
    _TCHAR ch;
    int ci;
    int found  = 0;
    struct FIND *myfind;
    _TCHAR *path;
    char instring;  /* used for " processing */
    /* first check current directory */
    if (pathname == NULL || filename == NULL)
        return;

    myfind = __tfindfirst(filename,0);
    if (myfind)
    {
        _tcscpy(pathname,filename);
        return;
    }

    if (varname == NULL)
    {
        *pathname = _T('\0');
        return;
    }

    path = _tgetenv(varname);
    if (path == NULL)
    {
        *pathname = _T('\0');
        return;
    }

    while (1)
    {
        instring = 0;
        for (counter = 0; counter < _MAX_PATH; counter++)
        {
            ch = *path++;
            if (ch == _T('\0') || (ch == _T(';') && !instring))
                break;
            if (ch == _T('\"'))
            {
                instring ^=1;
                counter--;  /* " should not go into eachpath */
            }
            else
                eachpath[counter] = ch;
#ifdef _MBCS
            if (_istlead ((unsigned char) ch)) {
                if (counter + 1 >= _MAX_PATH) {
                    break;
                }
                if (*path == '\0') {
                    break;
                }
                counter++;
                eachpath[counter] = *path++;
            }
# endif
        }
        eachpath[counter] = _T('\0');

        if (ch == _T('\0') || *path == _T('\0'))
            done++;

#if 0
        /* remove all trailing spaces then all backslashes from
           the path component (this provides very close emulation
           of DOS's command shell behavior) */
        for (ci = 0; ci < 2; ci++)
            for (counter = _tcslen (eachpath) - 1; counter >= 0; counter --)
            {
#if M_UNIX || M_XENIX
                if (eachpath[counter] == _T(" /")[ci])
#else
                if (eachpath[counter] == _T(" \\")[ci])
#endif
                    eachpath[counter] = _T('\0');
                else
                    break;
            }
#endif  // if 0

        /* append the file name to the path component */
#if M_UNIX || M_XENIX
        if (counter && eachpath[counter-1] != _T('/'))
            _tcscat(eachpath,_T("/"));
#else
# ifdef _MBCS
        if (counter) {
          char * bp;
            bp = _tcsdec (eachpath, eachpath + counter);
            if (bp != NULL) {
                if (*bp != _T('\\')) {
                    _tcscat (eachpath, _T("\\"));
                }
            }
        }
# else
        if (counter && eachpath[counter-1] != _T('\\'))
            _tcscat(eachpath,_T("\\"));
# endif
#endif
        _tcscat(eachpath,filename);

        /* look for the path component plus filename */
        myfind = __tfindfirst(eachpath,0);
        if (myfind)
            found++;

        /* break the first time the file is found or when we've exhausted the path */
        if (done || found)
            break;
    }

    if (found)
        _tcscpy (pathname, eachpath);
    else
        *pathname = _T('\0');
    return;
}

#endif

#ifndef _NOMAKEPATH

void __tfnmerge(_TCHAR *path,const _TCHAR *drive,const _TCHAR *dirname,
                const _TCHAR *filename,const _TCHAR *ext)
{
    int length;
    int backslashNeeded = 0;
    int periodNeeded = 0;
    int first_component = 1;  /* determines strcat or strcpy */


    if (!path)
        return;

    /* check for needed backslash */
    length = (dirname) ? _tcslen(dirname) : 0;
# ifdef _MBCS
    if (length > 0) {
      char * bp;
        bp = _tcsdec (dirname, dirname + length);
        if (bp != NULL) {
            if (*bp != '/' && *bp != '\\') {
                backslashNeeded = 1;
            }
        }
    }
# else
    if ((length > 0) && ((dirname[length-1] != _T('/')) && (dirname[length-1] != _T('\\'))))
        backslashNeeded = 1;
# endif

    /* Check for needed period */
    length = (ext) ? _tcslen(ext) : 0;
    if ((length > 0) && (ext[0] != _T('.')))
        periodNeeded = 1;

    /* If we have a drive spec then extract it.  Only use 1 character because
       _MAX_DRIVE == 3 */
    if (drive && (_tcslen(drive) > 0))
    {
        path[0] = drive[0];
        path[1] = _T(':');
        path[2] = 0;
        first_component = 0;
    }

    /* if we have a directory, append it */
    if (dirname && _tcslen(dirname) > 0)
    {
        if (!first_component)
            _tcscat(path, dirname);
        else
        {
            _tcscpy(path,dirname);
            first_component = 0;
        }
    }

    /* if we need a slash, add it */
#if M_UNIX || M_XENIX
    if (backslashNeeded)
        _tcscat(path, _T("/"));
#else
    if (backslashNeeded)
        _tcscat(path, _T("\\"));
#endif

    /* If we have a filename, append it */
    if (filename && _tcslen(filename) > 0)
    {
        if (!first_component)
            _tcscat(path, filename);
        else
        {
            _tcscpy(path,filename);
            first_component = 0;
        }
    }

    /* If we need a period, add it */
    if (periodNeeded)
    {
        if (!first_component)
            _tcscat(path, _T("."));
        else
        {
            _tcscpy(path, _T("."));
            first_component = 0;
        }
    }

    /* If we have an extension, append it */
    if (ext && _tcslen(ext) > 0)
    {
        if (!first_component)
            _tcscat(path, ext);
        else
        {
            _tcscpy(path,ext);
           first_component = 0;
        }
    }

    /* If no components found, then return empty string */
    if (first_component)
        _tcscpy(path,_T(""));
}

void _tmakepath(_TCHAR *path,const _TCHAR *drive,const _TCHAR *dirname,
                const _TCHAR *filename,const _TCHAR *ext)
{
    __tfnmerge(path, drive, dirname, filename, ext);
}

#endif

#if !(M_UNIX || M_XENIX)

#ifndef _UNICODE

/*********************************************************************/
int __CLIB getcurdir (int drive, _TCHAR *dir)
{
    _TCHAR buf[_MAX_PATH];
    int start = 0;

    if (_tgetdcwd (drive, buf, _MAX_PATH) == NULL)
        return -1;
    if (buf[1] == _T(':'))
        start = 3;
    _tcscpy (dir, buf+start);
    return 0;
}

#endif

#ifndef _NOGETDCWD

/*********************************************************************/
_TCHAR * __CLIB _tgetdcwd (int drive, _TCHAR *Pbuf, int PbufLen)
{
    int prevDrive;

    if (drive == 0)
        return _tgetcwd (Pbuf, PbufLen);

    prevDrive = _getdrive();
    if (_chdrive (drive))
        return NULL;
    Pbuf = _tgetcwd (Pbuf, PbufLen);
    _chdrive (prevDrive);
    return (Pbuf);
}

#endif

#endif

#if __INTSIZE == 2

/*********************************************************************/
char __far * __CLIB getdta(void)
{
    char __far *dta;

    asm
    {
        mov ah,2Fh
        int 21h
        mov WORD PTR dta,bx
        mov WORD PTR (dta+2),es
    }
    return dta;
}

/*********************************************************************/
void __CLIB getfat(unsigned char disk, struct fatinfo *fat)
{
    char sclus;
    char fatid;
    int nclus;
    int bysec;

    asm
    {
        push ds
        mov ah,1Ch
        mov dl,disk
        int 21h
        mov ah,ds:[bx]
        pop ds
        mov sclus,al
        mov fatid,ah
        mov nclus,dx
        mov bysec,cx
    }
    fat->fi_sclus = sclus;
    fat->fi_fatid = fatid;
    fat->fi_nclus = nclus;
    fat->fi_bysec = bysec;
}

/*********************************************************************/
void __CLIB getfatd(struct fatinfo *fat)
{
    getfat(0,fat);
}

/*********************************************************************/
#if (__SMALL__ || __MEDIUM__)
int __CLIB getftime(int FHdl, struct ftime *FtPtr)
{

    asm
    {
        mov ax, 5700h
        mov bx, FHdl
        INT 21h
        jc Failed
        push ds
        pop es
        mov di,FtPtr
        mov es:[di],cx
        mov es:[di+2],dx
        mov ax,0
        jmp Done
    Failed:
        push ax
        call __set_dos_errno
        mov ax,-1
    Done:
    }
    return(_AX);
}
#else
int __CLIB getftime(int FHdl, struct ftime *FtPtr)
{
    asm
    {
        mov ax, 5700h
        mov bx, FHdl
        INT 21h
        jc  Failed
        LES di,FtPtr
        mov es:[di],cx
        mov es:[di+2],dx
        mov ax,0
        jmp Done
    Failed:
        push    ax
        call __set_dos_errno
        mov    ax,-1
    Done:
    }
    return(_AX);
}
#endif

/*********************************************************************/
#if (__SMALL__ || __MEDIUM__)
int __CLIB setftime(int handle, struct ftime *ftptr)
{
    asm
    {
        mov ax,5701h
        mov bx,handle
        push ds
        pop es
        mov di,(unsigned *)ftptr
        mov cx,es:[di]
        mov dx,es:[di+2]
        INT 21h
        jc Failed
        mov ax, 0
        jmp Done
    Failed:
        push ax
        call __set_dos_errno
        mov AX,-1
    Done:
    }
    return(_AX);
}
#else
int __CLIB setftime(int handle, struct ftime *ftptr)
{
    asm
    {
        mov ax, 5701h
        mov bx, handle
        LES di, ftptr
        mov cx,es:[di]
        mov dx,es:[di+2]
        INT 21h
        jc Failed
        mov ax, 0
        jmp Done
    Failed:
        push ax
        call __set_dos_errno
        mov AX,-1
    Done:
    }
    return(_AX);
}
#endif

/*********************************************************************/
int __CLIB getverify(void)
{
    asm
    {
        mov ax,5400h
        INT 21h
        cbw
    }
    return(_AX);
}

/*********************************************************************/
void __CLIB setverify(int value)
{

    asm
    {
        mov ah,2Eh
        mov al,(char)value
        INT 21h
    }
}

#endif

#ifndef _UNICODE

/*********************************************************************/
int __CLIB getw(FILE *FHdl)
{
   unsigned char byte1;
   unsigned char byte2;

   if((byte1 = getc(FHdl)) != EOF &&
      (byte2 = getc(FHdl)) != EOF)
      return (byte2 << 8) | byte1;
   else
      return EOF;
}

/*********************************************************************/
int __CLIB putw(int Word, FILE *FilePtr)
{
   if(putc((unsigned char)Word, FilePtr) != EOF &&
      putc((unsigned char)(Word>>8), FilePtr) != EOF)
      return Word;
   else
      return EOF;
}

#if __INTSIZE == 2
/*********************************************************************/
void __CLIB setdta(char __far *dta)
{
    asm
    {
        push ds
        mov ah,1Ah
        lds dx,dta
        int 21h
        pop ds
    }
}

/*********************************************************************/
/* LOCK */
int __CLIB lock(int FHdl, long Offset, long Size)
{
    unsigned looff;
    unsigned hioff;
    unsigned losize;
    unsigned hisize;

    looff=(int)Offset;
    hioff=(int)(Offset>>16);
    losize=(int)Size;
    hisize=(int)(Size>>16);

    asm
    {
        mov ax,5C00h
        mov bx,FHdl
        mov cx,hioff
        mov dx,looff
        mov si,hisize
        mov di,losize
        INT 21h
        jnc Success
        push ax
        call __set_dos_errno
        mov ax,-1
        jmp Done
    Success:
        mov ax,0
    Done:
    }
    return (_AX);
}

/*********************************************************************/
/* UNLOCK */
int __CLIB unlock(int handle, long offset, long length)
{
    unsigned looff;
    unsigned hioff;
    unsigned losize;
    unsigned hisize;

    looff=(unsigned)offset;
    hioff=(unsigned)(offset>>16);
    losize=(unsigned)length;
    hisize=(unsigned)(length>>16);
    asm
    {
        mov ax,5C01h
        mov bx,handle
        mov cx,hioff
        mov dx,looff
        mov si,hisize
        mov di,losize
        INT 21h
        jnc Success
        push ax
        call __set_dos_errno
        mov ax,-1
        jmp Done
    Success:
        mov ax,0
    Done:
    }
    return(_AX);
}

/*********************************************************************/
unsigned __CLIB getpsp(void)
{
    asm
    {
        mov ah,62h;
        INT 21h;
    }
    return((unsigned)(_BX));
}
/*********************************************************************/
int __CLIB setblock(unsigned segment, unsigned size)
{
   unsigned rc;
   unsigned olderrno,olddoserrno;

   olderrno = errno;
   olddoserrno = _doserrno;
   errno = 0;
   rc = dos_setblock (size, segment);
   if (errno)
       return rc;
   if (rc != segment)
   {
       __set_errno (ENOMEM);
       return 0;
   }
   errno = olderrno;
   _doserrno = olddoserrno;
   return -1;
}

/*********************************************************************/
#if (__SMALL__ || __MEDIUM__)
char * __CLIB parsfnm(const char *cmdline, struct fcb *fcb, int opt)
{
    asm
    {
         mov si,cmdline
         push ds
         pop es
         mov di,fcb
         mov al,(char)opt
         mov ah, 29h
         int 21h
     }
     if (_AX == 255)
         return NULL;
     return ((char *)_SI);
}
#else
char * __CLIB parsfnm(const char *cmdline, struct fcb *fcb, int opt)
{
    asm
    {
        push ds
        lds si,cmdline
        les di,fcb
        mov al,(char)opt
        mov ah,29h
        int 21h
        pop ds
    }
    if (_AX == 255)
        return NULL;
    FP_OFF (cmdline) = _SI;
    return (char *)cmdline;
}
#endif
#endif

#endif

#ifndef _NOFULLPATH

/*********************************************************************/

#define PUT_CHAR(chr) \
   { \
   if (dst >= end) goto err; \
   *dst++ = (chr); \
   }

#define PUT_STRING(str,len) \
   { \
   if (dst + (len) >= end) goto err; \
   memcpy (dst, (str), (len) * sizeof (_TCHAR)); \
   dst += (len); \
   }

#define IS_SLASH(chr) ((chr) == _T('/') || (chr) == _T('\\'))

_TCHAR * __CLIB _tfullpath (_TCHAR *buf, const _TCHAR *path, size_t buflen)
{
    _TCHAR *beg;
    _TCHAR *end;
    _TCHAR *dst;
    const _TCHAR *src;
    int len;
    _TCHAR drive;
    /* Used to parse \\machine_name\share_name\ strings */
    int slash_count;
    _TCHAR *temp_buff = NULL;

    if (buf)
    {
        beg = buf;
        end = beg + buflen - 1;
    }
    else
    {
        beg = malloc ((_MAX_PATH + 1) * sizeof (_TCHAR));
        if (beg == NULL)
            goto err;
        end = beg + _MAX_PATH;
    }
    dst = beg;
    *end = _T('\0');

    src = path;
    if (*src == _T('\0'))
        goto err;

    if (src[1] == _T(':'))
    {
        if (!_istalpha (*src))
            goto err;
        drive = _totupper (*src);
        PUT_STRING(src,2);
        src += 2;
    }
#if !(M_UNIX || M_XENIX)
    else if (_tcsncmp (src,_T("\\\\"),2) != 0)
    {
        drive = _getdrive();
        if (IS_SLASH (*src))
        {
            if (drive)
            {
                PUT_CHAR (drive + _T('A') - 1);
                PUT_CHAR (_T(':'));
                src ++;
            }
            else
            {
                /* _getdrive will return 0 on UNC mappings only??? */
                dst = _tgetdcwd(0,dst, end - dst);
                if (dst == NULL)
                    goto err;
                /* UNC Format is \\machine_name\share_name\... */
                /* parse out machine and share names */
                dst = _tcschr((dst+2),_T('\\'));
                if (!dst)
                    goto err;
                dst = _tcschr((dst+1),_T('\\'));
                if (!dst)
                    goto err;
            }
        }
        else
        {
            dst = _tgetdcwd (drive, dst, end - dst);
            if (dst == NULL)
                goto err;
            dst = dst + _tcslen (dst);
# ifdef _MBCS
            {
              char * bp;
                bp = _tcsdec (beg, dst);
                if (bp != NULL) {
                    if (IS_SLASH (*bp)) {
                        dst--;
                    }
                }
            }
# else
            if (IS_SLASH (dst[-1]))
                dst--;
# endif
        }
    }
    else
    {
        /* Must be a UNC name, parse out machine and share names */
        PUT_STRING (src, 2);
        src += 2;

        for (len = 0,slash_count=0; len < _MAX_PATH; len ++)
        {
            if (IS_SLASH(src[len]))
                slash_count++;
            if (src[len] == _T('\0') || slash_count == 2)
                break;
# ifdef _MBCS
            if (_istlead (src[len])) {
                len++;
                if (src[len] == '\0') {
                    break;
                }
            }
# endif
        }

        PUT_STRING (src,len);
        src += len;
        if (IS_SLASH (*src))
            src++;
    }

#endif

    while (*src != _T('\0'))
    {
        /* LOOK FOR A SLASH IN THE SRC PATH */
        for (len = 0; len < _MAX_FNAME; len ++)
        {
            if (IS_SLASH (src[len]) || src[len] == _T('\0'))
                break;
# ifdef _MBCS
            if (_istlead (src[len])) {
                len++;
                if (src[len] == '\0') {
                    break;
                }
            }
# endif
        }
        if (len >= _MAX_FNAME)
            goto err;

        /* CHECK FOR "." AND ".." RELATIVE DIRECTORY NAMES */
        if (_tcsncmp (src, _T("."), len) == 0)
             ;
        else if (_tcsncmp (src, _T(".."), len) == 0)
        {
            /* REMOVE LAST FILENAME COMPONENT TO GET PARENT DIRECTORY NAME */
# ifdef _MBCS
          char * bp = dst;
            for (; dst > beg; dst = bp) {
                bp = _tcsdec (beg, dst);
                if (IS_SLASH (*bp) || *bp == ':') {
                    break;
                }
            }
            if (dst <= beg) {
                goto err;
            }
            bp = _tcsdec (beg, dst);
            if (IS_SLASH (*bp)) {
                dst = bp;
            }
# else
            for (/* dst = dst */; dst > beg; dst--)
                if (IS_SLASH(dst[-1]) || dst[-1] == _T(':'))
                    break;
            if (dst <= beg)
                goto err;
            if (IS_SLASH (dst[-1]))
                dst--;
# endif
        }
        else
        {
            /* ADD THE NEW FILENAME COMPONENT */
#if M_UNIX || M_XENIX
            PUT_CHAR (_T('/'));
#else
            PUT_CHAR (_T('\\'));
#endif
            PUT_STRING (src, len);
        }

        src += len;
        if (IS_SLASH (*src))
            src++;
   }
   /* FOR ROOT DIRECTORY, ADD A FINAL BACKSLASH */
#if !(M_UNIX || M_XENIX)
# ifdef _MBCS
   {
     char * bp;
       bp = _tcsdec (beg, dst);
       if (*bp == ':') {
           PUT_CHAR (_T('\\'));
       }
   }
# else
   if (dst[-1] == _T(':'))
       PUT_CHAR (_T('\\'));
# endif
#endif
   PUT_CHAR (_T('\0'));
   return beg;
err:
   if (buf == NULL && dst != NULL)
       free(dst);
   return NULL;
}

#endif

