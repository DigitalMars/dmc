/* Copyright (C) 1986-2001 by Digital Mars. $Revision: 1.1.1.2 $ */
#if __SC__ || __RCC__
#pragma once
#endif

#ifndef __DOS_H
#define __DOS_H 1

#if __cplusplus
extern "C" {
#endif

/* Define _CRTAPI1 (for compatibility with the NT SDK) */
#ifndef _CRTAPI1
#define _CRTAPI1 __cdecl
#endif

/* Define _CRTAPI2 (for compatibility with the NT SDK) */
#ifndef _CRTAPI2
#define _CRTAPI2 __cdecl
#endif

/* Define CRTIMP */
#ifndef _CRTIMP
#if defined(_WIN32) && defined(_DLL)
#define _CRTIMP  __declspec(dllimport)
#else
#define _CRTIMP
#endif
#endif

#if __OS2__ && __INTSIZE == 4
#define __CLIB  __stdcall
#else
#define __CLIB  __cdecl
#endif

#if !(M_XENIX || M_UNIX)
#pragma pack(1)
#endif

#if !defined(_WCHAR_T_DEFINED)
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED 1
#endif

#if _M_AMD64
typedef unsigned long long size_t;
#else
typedef unsigned size_t;
#endif

extern unsigned __cdecl _stack;

#if defined(DOS386)
extern unsigned short _x386_zero_base_selector;
extern void *_x386_zero_base_ptr;  
extern void __far *(*_x386_mk_protected_ptr)(unsigned long abs_address);
extern int (*_x386_free_protected_ptr)(void __far *);
extern unsigned long(*_x386_get_abs_address)(void __far *ptr);
extern unsigned int (_x386_coreleft)(void);
extern void* (_x386_map_physical_address)(void* abs_addr, unsigned size);
extern int (_x386_memlock)(void __far *pointer, unsigned int length);
extern int (_x386_memunlock)(void __far *pointer, unsigned int length);
#endif

#ifndef __REGS_DEFINED
#define __REGS_DEFINED

#if __INTSIZE == 4

#pragma pack(1)
struct _DWORDREGS {unsigned eax,ebx,ecx,edx,esi,edi,cflag,flags;};
#define DWORDREGS _DWORDREGS

struct _WORDREGS
{       unsigned short  ax,eaxmsw,bx,ebxmsw,cx,ecxmsw,dx,edxmsw,
			si,esimsw,di,edimsw;
	unsigned cflag,flags;
};
#define WORDREGS _WORDREGS

struct _BYTEREGS
{       unsigned char al,ah,ax2,ax3,bl,bh,bx2,bx3,
			  cl,ch,cx2,cx3,dl,dh,dx2,dx3;
};
#define BYTEREGS _BYTEREGS

union _REGS { struct _DWORDREGS e; struct _WORDREGS x; struct _BYTEREGS h; };
#define REGS _REGS

struct _SREGS { unsigned short es,cs,ss,ds,fs,gs; };
#define SREGS _SREGS

#pragma pack()
#else /* __INTSIZE == 4 */

#pragma pack(1)
struct WORDREGS { unsigned ax,bx,cx,dx,si,di,cflag,flags; };
struct BYTEREGS {unsigned char al,ah,bl,bh,cl,ch,dl,dh; };
union REGS { struct WORDREGS x; struct BYTEREGS h; };
struct SREGS { unsigned es,cs,ss,ds; };
#pragma pack()
#define _REGS REGS
#define _SREGS SREGS
#define _WORDREGS WORDREGS
#define _BYTEREGS BYTEREGS

#endif /* __INTSIZE==4 */
#endif /* __REGS_DEFINED */

#ifndef __NT__
#define disable() __emit__((char)(0xFA))
#define _disable() __emit__((char)(0xFA))
#define enable() __emit__((char)(0xFB))
#define _enable() __emit__((char)(0xFB))
#define geninterrupt(intr) __emit__(0xCD,(intr))

int __cdecl int86(int,union REGS *,union REGS *);
int __cdecl int86x(int,union REGS *,union REGS *,struct SREGS *);
int __cdecl intdos(union REGS *,union REGS *);
int __cdecl intdosx(union REGS *,union REGS *,struct SREGS *);
void __CLIB segread(struct SREGS *);
#define _int86 int86
#define _int86x int86x
#define _intdos intdos
#define _intdosx intdosx
#define _segread segread

#if DOS386
int __cdecl int86_real(int,union REGS *,union REGS *);
int __cdecl int86x_real(int,union REGS *,union REGS *,struct SREGS *);
#endif
#endif

#if (M_UNIX || M_XENIX)

#define VTIOC           ('v' << 8)
#define VT_OPENQRY      (VTIOC | 1)
#define VT_SETMODE      (VTIOC | 2)
#define VT_GETMODE      (VTIOC | 3)
#define VT_RELDISP      (VTIOC | 4)
#define VT_ACTIVATE (VTIOC | 5)
#define VT_FALSE        0
#define VT_TRUE         1
#define VT_ACKACQ       2
#define VT_AUTO         0
#define VT_PROCESS      1
struct vt_mode {char mode, waitv; short relsig, acqsig, frsig;};
typedef int (*_vt_sig_func_t)(int);
void _vt_get_mode(struct vt_mode *p);
int _vt_set_mode(struct vt_mode *p);
int _vt_get_num(void);
int _vt_activate(int vt_num);
int _vt_add(_vt_sig_func_t func);
int _vt_remove(_vt_sig_func_t func);

#include <sys/stat.h>

#ifdef __NT__
struct FINDW
{
	struct stat stat;
	unsigned short attribute;
	unsigned short time,date;
	unsigned long size;
	wchar_t name[16];
};
struct FINDA
#else
struct FIND
#endif
{
	struct stat stat;
	unsigned short attribute;
	unsigned short time,date;
	unsigned long size;
	char name[16];
};

#define FA_NORMAL       0x00
#define FA_DIREC        S_IFDIR

#else           /* !M_UNIX */

#pragma pack(1)

#ifdef __NT__
struct FINDW          /* struct used by wfindfirst() and wfindnext() */
{       char reserved[21];        /* reserved by DOS                 */
	char attribute;           /* attribute found (FA_XXXX)       */
	unsigned short time,date; /* file's time and date            */
	unsigned long size;       /* file's size                     */
#if !defined(_WIN32)
	wchar_t name[13];         /* filename followed by 0 byte     */
#else
	wchar_t name[260];
#endif
};
struct FINDA            /* struct used by findfirst() and findnext()    */
#else
struct FIND             /* struct used by findfirst() and findnext()    */
#endif
{       char reserved[21];      /* reserved by DOS                      */
	char attribute;         /* attribute found (FA_XXXX)            */
	unsigned short time,date; /* file's time and date       */
	unsigned long size; /* file's size              */
#if !defined(_WIN32)
	char name[13];          /* filename followed by 0 byte          */
#else
	char name[260];
#endif
};

struct find_t           /* used by _dos_findfirst() and _dos_findnext() */
{       char reserved[21];      /* reserved by DOS                      */
	char attrib;            /* attribute found (FA_XXXX)            */
	unsigned short wr_time,wr_date; /* file's last write        */
	unsigned long size; /* file's size              */
#if !defined(_WIN32)
	char name[13];          /* filename followed by 0 byte          */
#else
	char name[260];
#endif
};

#define _find_t find_t
#pragma pack()

#define FA_NORMAL       0x00
#define FA_RDONLY       0x01
#define FA_HIDDEN       0x02
#define FA_SYSTEM       0x04
#define FA_LABEL        0x08
#define FA_DIREC        0x10
#define FA_ARCH         0x20

#define _A_NORMAL       0x00
#define _A_RDONLY       0x01
#define _A_HIDDEN       0x02
#define _A_SYSTEM       0x04
#define _A_VOLID        0x08
#define _A_SUBDIR       0x10
#define _A_ARCH         0x20

#endif

#ifdef __NT__
#ifdef _UNICODE
#define FIND		FINDW
#else
#define FIND		FINDA
#endif
struct FINDA * __CLIB findfirst(const char *,int);
struct FINDA * __CLIB findnext(void);
struct FINDW * __CLIB _wfindfirst (const wchar_t *, int);
struct FINDW * __CLIB _wfindnext (void);
#else
struct FIND * __CLIB findfirst(const char *,int),* __CLIB findnext(void);
#endif

#if defined(MSDOS) || (defined(_WINDOWS) && !defined(__NT__))
unsigned __CLIB _dos_findfirst(const char *, unsigned, struct find_t *);
unsigned __CLIB _dos_findnext(struct find_t *);
#endif

#if defined(MSDOS)
void __CLIB _dos_keep(unsigned, unsigned);
#endif

#ifndef __NT__
#define _FP_OFF(fp) (*((unsigned __far *)&(fp)))
#define FP_OFF _FP_OFF

#if __INTSIZE == 4
extern unsigned __CLIB FP_SEG(void __far *);
#define _FP_SEG FP_SEG
#else
#define _FP_SEG(fp) (*((unsigned __far *)&(fp)+1))
#define FP_SEG _FP_SEG
#endif
#if __INTSIZE == 4
extern void __far * __CLIB MK_FP(unsigned short,unsigned);
#define MK_FP(seg,offset) MK_FP((seg),(unsigned)(offset))
#else
#define MK_FP(seg,offset) \
	((void __far *)(((unsigned long)(seg)<<16) | (unsigned)(offset)))
#endif
#endif

#define _MK_FP MK_FP

#if __INTSIZE == 2
void __far * __cdecl farmalloc(unsigned long size);
void __far * __cdecl farcalloc(unsigned long numelems,unsigned long elemsize);
void __far * __cdecl farrealloc(void __far *oldptr,unsigned long newsize);
int __cdecl farfree(void __far *ptr);
unsigned long __cdecl farcoreleft(void);
#endif

#pragma pack(1)
struct DOSERROR
{
	int exterror;   /* extended error code */
	char eclass;            /* error class */
	char action;    /* recommended action */
	char locus;             /* error locus */
};
#define _DOSERROR DOSERROR
#define errclass eclass

struct  COUNTRY {
	short   co_date;
	char    co_curr[5];
	char    co_thsep[2];
	char    co_desep[2];
	char    co_dtsep[2];
	char    co_tmsep[2];
	char    co_currstyle;
	char    co_digits;
	char    co_time;
	long    co_case;
	char    co_dasep[2];
	char    co_fill[10];
};

struct diskfree_t {
	unsigned total_clusters;
	unsigned avail_clusters;
	unsigned sectors_per_cluster;
	unsigned bytes_per_sector;
};
#define _diskfree_t diskfree_t

struct dos_date_t
{
	unsigned char day;
	unsigned char month;
	unsigned short  year;
	unsigned char dayofweek;
};
#define _dosdate_t dos_date_t

struct  _fatinfo
{
	char            fi_sclus;
	char            fi_fatid;
	unsigned        fi_nclus;
	int             fi_bysec;
};
#define fatinfo _fatinfo

struct dos_time_t
{
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
	unsigned char hsecond;
};
#define _dostime_t dos_time_t

struct time
{
	unsigned char   ti_min;
	unsigned char   ti_hour;
	unsigned char   ti_hund;
	unsigned char   ti_sec;
};

struct date
{
	int da_year;
	char    da_day;
	char    da_mon;
};

struct fcb
{
	char    fcb_drive;
	char    fcb_name[8];
	char    fcb_ext[3];
	short   fcb_curblk;
	short   fcb_filesize;
	short   fcb_date;
	char    fcb_resv[10];
	char    fcb_crrec;
	long    fcb_random;
};

#pragma pack()

#ifndef __NT__
struct COUNTRY *__CLIB  country( int , struct COUNTRY *);
extern unsigned int __CLIB _osversion;
#ifndef DOS386
int __CLIB              getcbrk(void);
void __CLIB             ctrlbrk(int (*FarPtr)());
int __CLIB              setcbrk(int value);
void __CLIB             getfat(unsigned char disk, struct fatinfo *fat);
void __CLIB             getfatd(struct fatinfo *fat);
int __CLIB              getverify(void);
void __CLIB             setverify(int value);
char __far * __CLIB     getdta(void);
void __CLIB             setdta(char __far *dta);
unsigned __CLIB         getpsp(void);
int __CLIB              setblock(unsigned segment, unsigned size);
char * __CLIB           parsfnm(const char *cmdline, struct fcb *fcb, int opt);
#endif /* DOS386 */
#endif /* __NT__ */

#ifndef DOS386
void __CLIB             getdate(struct date *datePtr);
void __CLIB             gettime(struct time *timePtr);
#endif

unsigned __CLIB         _dos_lock(int, int, unsigned long, unsigned long);
unsigned long __CLIB    _dos_seek(int, unsigned long, int);
int __CLIB              dos_creat(char *,int);
int __CLIB              _dos_creat(const char *,unsigned, int *);
int __CLIB              _dos_creatnew(const char *,unsigned , int *);
unsigned __CLIB         _dos_open(const char *, unsigned,int *);
int __CLIB              dos_close(int);
unsigned __CLIB         _dos_read(int, void __far *,unsigned,unsigned *);
unsigned __CLIB         _dos_write(int, const void __far *,unsigned,unsigned *);
int __CLIB              dos_get_ctrl_break(void);
#ifndef DOS386
int __CLIB              dos_abs_disk_read(int,int,long,char *);
int __CLIB              dos_abs_disk_write(int,int,long,const char *);
#endif
void __CLIB             dos_set_verify(int);
void __CLIB             dos_set_ctrl_break(int);
int __CLIB              dos_get_verify(void);
int __CLIB              getcurdir(int drive,char *dir);
char * __CLIB           _getdcwd(int drive,char *Pbuf,int PbufLen);
unsigned __CLIB         _dos_commit(int);
int __CLIB              dos_exterr(struct DOSERROR *);
int __CLIB              dosexterr(struct DOSERROR *);
int __CLIB              setdisk(int);
int __CLIB              getdisk(void);
long __CLIB             dos_getdiskfreespace(int);
unsigned __CLIB         _dos_getdiskfree(unsigned, struct diskfree_t *);
void __CLIB             dos_setdrive(unsigned, unsigned *);
void __CLIB             dos_getdrive(unsigned *);
unsigned __CLIB         dos_setftime(int, unsigned, unsigned);
unsigned __CLIB         dos_getftime(int, unsigned *, unsigned *);
unsigned __CLIB         dos_setfileattr(const char *, unsigned);
unsigned __CLIB         dos_getfileattr(const char *, unsigned *);
unsigned __CLIB         dos_setdate(struct dos_date_t *);
void __CLIB             dos_getdate(struct dos_date_t *);
int __CLIB              _dos_sethandlecount(unsigned);
unsigned __CLIB         dos_settime(struct dos_time_t *);
void __CLIB             dos_gettime(struct dos_time_t *);

#define absread(drive,nsects,lsects,buffer) dos_abs_disk_read(drive,nsects,lsects,buffer)
#define abswrite(drive,nsects,lsects,buffer) dos_abs_disk_write(drive,nsects,lsects,buffer)
#define _dosexterr(dosstruct) dos_exterr(dosstruct)
#define _dos_close dos_close

#if __INTSIZE == 2
unsigned                _dos_allocmem(unsigned int,unsigned int *);
int                     allocmem(unsigned int,unsigned int *);
unsigned                _dos_freemem(unsigned);
int                     freemem(unsigned);
#endif

#define _dos_getdate dos_getdate
#define _dos_getdrive dos_getdrive
#define _dos_getfileattr dos_getfileattr
#define _dos_getftime dos_getftime
#define _dos_gettime dos_gettime
#define _dos_setdate dos_setdate
#define _dos_setdrive dos_setdrive
#define _dos_setfileattr dos_setfileattr
#define _dos_setftime dos_setftime
#define _dos_settime dos_settime

unsigned char   __CLIB _inp(unsigned);
unsigned short  __CLIB _inpw(unsigned);
unsigned long   __CLIB _inpl(unsigned);

unsigned char   __CLIB _outp(unsigned,unsigned char);
unsigned short  __CLIB _outpw(unsigned,unsigned short);
unsigned long   __CLIB _outpl(unsigned,unsigned long);

unsigned char   __CLIB _inline_inp(unsigned);
unsigned short  __CLIB _inline_inpw(unsigned);
unsigned long   __CLIB _inline_inpl(unsigned);

unsigned char   __CLIB _inline_outp(unsigned,unsigned char);
unsigned short  __CLIB _inline_outpw(unsigned,unsigned short);
unsigned long   __CLIB _inline_outpl(unsigned,unsigned long);

#define _inp(x)		_inline_inp(x)
#define _inpw(x)        _inline_inpw(x)
#define _outp(x,y)      _inline_outp(x,y)
#define _outpw(x,y)	_inline_outpw(x,y)

#if __INTSIZE == 4
#define _inpl(x)        _inline_inpl(x)
#define _outpl(x,y)	_inline_outpl(x,y)
#endif

// Obsolete, use the _ prefixed versions
#define inp(x)          _inline_inp(x)
#define inpw(x)         _inline_inpw(x)
#define outp(x,y)       _inline_outp(x,y)
#define outpw(x,y)      _inline_outpw(x,y)

#if !(defined (_WINDOWS) || defined (__NT__))

#define _HARDERR_IGNORE 0
#define _HARDERR_RETRY  1
#define _HARDERR_ABORT  2
#define _HARDERR_FAIL   3

void __cdecl _harderr (void (__far __cdecl *) (unsigned, unsigned, unsigned __far *));
void __cdecl _hardresume (int);
void __cdecl _hardretn (int);

#endif /*!(defined (_WINDOWS) || defined (__NT__))*/

size_t __CLIB _chkstack(void);

#ifndef __NT__

#define interrupt __interrupt

void __cdecl peekbytes(unsigned,unsigned,void *,size_t);
void __cdecl pokebytes(unsigned,unsigned,const void *,size_t);
#if __cplusplus
void inline poke(unsigned segm, unsigned offs, int value)
{
	*((int __far *) MK_FP(segm, offs)) = value;
}

int inline peek(unsigned segm, unsigned offs)
{
	return *((int __far *) MK_FP(segm, offs));
}
void inline pokeb(unsigned segm, unsigned offs, char value)
{
	*((char __far *) MK_FP(segm, offs)) = value;
}

char inline peekb(unsigned segm, unsigned offs)
{
	return *((char __far *) MK_FP(segm, offs));
}
#else
#define peek(segm, offs) (*((int __far *) MK_FP((segm), (offs))))
#define peekb(segm, offs) (*((char __far *) MK_FP((segm), (offs))))
#define poke(segm, offs, value) (*((int __far *) MK_FP((segm), (offs))) = (int)(value))
#define pokeb(segm, offs, value) (*((char __far *) MK_FP((segm), (offs))) = (char)(value))
#endif

#ifndef _WINDOWS
#ifndef DOS386
void __CLIB _chain_intr (void (__interrupt __far * funcptr)());
void (__interrupt __far * __CLIB _dos_getvect(unsigned vector))();
void __CLIB _dos_setvect (unsigned vector, 
			  void (__interrupt __far * funcptr)());
#if __cplusplus
void inline (__interrupt __far * getvect(int vector))()
{
   return _dos_getvect (vector);
}
void inline setvect(int vector, void (__interrupt __far * funcptr)())
{
   _dos_setvect(vector, funcptr);
}
#else
#define getvect _dos_getvect
#define setvect _dos_setvect
#endif
#endif /* DOS386 */
#endif

int __cdecl bdos(int dosfunc, unsigned int dosdx, unsigned int dosal);
#define _bdos bdos
int __cdecl bdosptr(int, void *, unsigned);
int __cdecl bdosx(char,void *,char);

#ifndef DOS386
unsigned __CLIB dos_alloc(unsigned);
unsigned __CLIB dos_calloc(unsigned);
int __CLIB dos_free(unsigned);
#endif /* DOS386 */
void __CLIB _copy(unsigned,unsigned,unsigned);
unsigned long __CLIB dos_avail(void);
#ifndef DOS386
unsigned __CLIB dos_setblock(unsigned,unsigned);
unsigned __CLIB _dos_setblock(unsigned,unsigned,unsigned *);
#endif /* DOS386 */

#endif

int __pascal response_expand(int *,char ***);

int __CLIB cputype(void);

#ifndef _WIN32
# if defined(ZTC_COMPAT)
#  if !(M_UNIX || M_XENIX)
#   if !__cplusplus
#define WILDCARDS  extern int __cdecl __wildcard; int *__wild = &__wildcard;  
#   else
#define WILDCARDS  extern "C" { extern int __cdecl __wildcard; int *__wild = &__wildcard; }
#   endif
#  else
#define WILDCARDS
#  endif
# else
#  if !(M_UNIX || M_XENIX)
#   if !__cplusplus
#define EXPAND_WILDCARDS  extern int __cdecl __wildcard; int *__wild = &__wildcard;  
#   else
#define EXPAND_WILDCARDS  extern "C" { extern int __cdecl __wildcard; int *__wild = &__wildcard; }
#   endif
#  else
#define EXPAND_WILDCARDS
#  endif
# endif
#else
# if !(M_UNIX || M_XENIX)
#  if !__cplusplus
#define WILDCARDS		int __cdecl __wildcard = 1;
#define EXPAND_WILDCARDS	int __cdecl __wildcard = 1;
#  else 
#define WILDCARDS extern "C" { int __cdecl __wildcard = 1; }
#define EXPAND_WILDCARDS extern "C" { int __cdecl __wildcard=1; }
#  endif
# else
#define WILDCARDS
#define EXPAND_WILDCARDS
# endif
#endif

#define EXE_DOS         1
#define EXE_DOS16RM     2
#define EXE_ZPM         4
#define EXE_PHAR386     8
#define EXE_DOSX        0x10
#define EXE_WINDOWS     0x20
#define EXE_OS2         0x40
#define EXE_SCOUNIX     0x80
#define EXE_OS2_2       0x100
#define EXE_WINDOWSNT   0x200

extern unsigned short __cdecl _exe_type;

#if !(M_UNIX || M_XENIX)
#pragma pack()
#endif

#ifdef _WINDOWS
extern void __far __pascal DOS3CALL(void);
#define int21h  __asm   call    DOS3CALL
#else
#define int21h  __asm   int 21h
#endif

#if __cplusplus
inline unsigned _getdiskfree(unsigned driveno, struct _diskfree_t *diskspace)
{
	return _dos_getdiskfree(driveno, diskspace); /*needed for MSVCNT compatibility*/
}
#else
#define _getdiskfree _dos_getdiskfree   /*needed for MSVCNT compatibility*/
#endif

#if __cplusplus
}
#endif

#endif
