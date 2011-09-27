/* Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 * March 10, 1994
 *
 * The following definitions are used internally to Digital Mars RTL routines.
 * They are not part of the published interface.
 * Names in this file are defined in one source file and referenced in one or more 
 *   other source files.
 * This file is included in both the defining and referencing files, insuring that
 *   the definition and references will remain in sync.
 */

/* _32BIT is used in some internal declarations.
 * It allows you to declare 4-byte pointers in any model. It means:
 *   WIN32         = __near (that's the only kind of pointer it has).
 *   DOS386        = __near (NOT __far, which is a 6-byte pointer).
 *   DOS16 & WIN16 = __far  (NOT __near, which is a 2-byte pointer).
 */
#if __INTSIZE == 4
#  define _32BIT __near
#else
#  define _32BIT __far
#endif

typedef unsigned short wchar_t;

/* spawnXXX/execXXX */
int __spawn (int mode, const char *cmd, const char *const *argv, const char *const *envp, int use_path);
const char *const *__seek_envp (const char *const *arg);
#ifdef __NT__
int __wspawn (int mode, const wchar_t *cmd, const wchar_t *const *argv, const wchar_t *const *envp, int use_path);
const wchar_t *const *__wseek_envp (const wchar_t *const *arg);
#endif

/* atexit/onexit/exit/exitstat */
extern void (_32BIT *_atexit_tbl[]) (void);
extern short _atexit_max;
extern void (_32BIT **_atexitp) (void);

/* cinit/putenv/buildenv */
extern int __build_environ (void);

/* exit/exitstat */
void (* _hookexitp)(int errstatus);
extern void (*_fcloseallp)(void);

/* exec2 related stuff  */
#if __INTSIZE == 4
  extern int _exec (char *command, char *commandline, char *envptr, int chain);
#ifdef __NT__
  extern int _wexec (wchar_t *command, wchar_t *commandline, wchar_t *envptr, int chain);
#endif
#else
  extern unsigned aenvseg;
  extern int _exec (char *command, char *commandline, int envseg, int chain);
#endif

/* hidden value returned by time function */
extern unsigned short __time_milliseconds;

/* internal routines and macros used by time functions */
void __internal_tzset();

#include <time.h>
time_t __localstructtmtotime_t (struct tm *);
int __adjustfordst(struct tm *);

#define SECSPERHOUR (60L*60L)
#define SECSPERDAY	(SECSPERHOUR*24L)

#if 0
#define ISLEAP(year) ((year)%4 == 0 && ( (year)%100 != 0 || (year)%400 == 0) )
#else
#define ISLEAP(year) ((year)%4 == 0)
#endif

#define DAYS_PER_LEAP_PERIOD 1461L  /* Number of days in every 4 year period */
/* These defines control the appearance of the Leap Periods */
#define DAYS_IN_YEAR1 365
#define DAYS_IN_YEAR2 730
#define DAYS_IN_YEAR3 1096
#define NLEAP_YEARS(years) ((years + 1) >> 2)

#if M_UNIX || M_XENIX
#       define YEAR0 1980
#else
#       define YEAR0 1970   /* used for MS compatible time calcs. */
#endif

#if M_UNIX || M_XENIX
#define WDAY0 2        /*  Tuesday  : Jan. 1st 1980 */
#else
#define WDAY0 4        /*  Thursday : Jan. 1st 1970 */
#endif

/**************************
 * # of days in year at start of month
 */
extern const unsigned short _mdays[13];


/* umask/open/creat */
extern int __umask_pmode;

/* sets _doserrno to NT or DOS error code, then converts it to errno */
extern unsigned __pascal __set_dos_errno (unsigned code);
extern unsigned __pascal __set_nt_errno (void);

/* for setting your own errno, not from either OS. Sets _doserrno to zero as there was
   no OS error. */
void __pascal __set_errno (int an_errno);

/* fatal error routine */
void __faterr (char *);

/* malloc/free/etc. */
#pragma pack(1)
  struct heapfree
  {
    struct heapfree *next;
    int size;
  };
#pragma pack()

extern struct heapfree _baslnk;
extern struct heapfree *_allocp;

/*
 * int-sized registers. Ex. INTAX is the AX register that is the size of an int
 * in any model: AX in 16-bit models, EAX in 32-bit models. INTAX is for use in
 * __asm blocks, _INTAX is for use outside of them.
 */
#if __INTSIZE == 4
# define INTAX EAX
# define INTBX EBX
# define INTCX ECX
# define INTDX EDX
# define INTSI ESI
# define INTDI EDI
# define INTBP EBP
# define INTSP ESP
# define _INTAX _EAX
# define _INTBX _EBX
# define _INTCX _ECX
# define _INTDX _EDX
# define _INTSI _ESI
# define _INTDI _EDI
# define _INTBP _EBP
# define _INTSP _ESP
#else
# define INTAX AX
# define INTBX BX
# define INTCX CX
# define INTDX DX
# define INTSI SI
# define INTDI DI
# define INTBP BP
# define INTSP SP
# define _INTAX _AX
# define _INTBX _BX
# define _INTCX _CX
# define _INTDX _DX
# define _INTSI _SI
# define _INTDI _DI
# define _INTBP _BP
# define _INTSP _SP
#endif

/* contains floating point exception function  */
extern void (* __fp_sigfunc)(int,int);


