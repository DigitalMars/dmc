#include <scrtl.h>
#include <stdlib.h>
#include <errno.h>
#include <mt.h>

static int trans_table[] =
{
    EZERO,  /* 00 no error */
    EINVAL, /* 01 function number invalid */
    ENOENT, /* 02 file not found */
    ENOENT, /* 03 path not found */
    EMFILE, /* 04 too many open files */
    EACCES, /* 05 access denied */
    EBADF,  /* 06 invalid handle */
    ENOMEM, /* 07 memory control block destroyed */
    ENOMEM, /* 08 insufficient memory */
    ENOMEM, /* 09 memory block address invalid */
    E2BIG,  /* 0a environment invalid */
    ENOEXEC,/* 0b format invalid */
    EINVAL, /* 0c access code invalid */
    EINVAL, /* 0d data invalid */
    -1,     /* 0e reserved */
    ENOENT, /* 0f invalid drive */
    EACCES, /* 10 attempt to remove current dir */
    EXDEV,  /* 11 not same device */
    ENOENT, /* 12 no more files */
    -1      /* 13 disk write-protected */
};

#define ACCESS_DENIED     0x05
#define LAST_DOS2_CODE    0x12
#define SHARING_VIOLATION 0x20
#define LOCK_VIOLATION    0x21

unsigned __pascal __set_dos_errno (unsigned dos_code)
{
#if defined(_MT)
    struct THREAD *my_thread =(struct THREAD*) getthreaddata ();

#define ERRNO (my_thread->t_errno)
#define DOSERRNO (my_thread->t_doserrno)
#else
#define ERRNO errno
#define DOSERRNO _doserrno
#endif

#if defined(_MT)
    /* If getthreaddata fails, then nothing can be set, return EINVAL... */
    if (!my_thread)
    {
        return EINVAL;
    }
#endif

    DOSERRNO = dos_code;
    if (_osmajor >= 3 && (dos_code == SHARING_VIOLATION || dos_code == LOCK_VIOLATION))
        dos_code = ACCESS_DENIED;
    else if (dos_code > LAST_DOS2_CODE)
        dos_code = LAST_DOS2_CODE + 1;
    ERRNO = trans_table[dos_code];
    return DOSERRNO;
}

