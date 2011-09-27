/* Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 */

#ifdef __NT__

#include <windows.h>
#include <scrtl.h>
#include <errno.h>
#include <stdlib.h>
#include <mt.h>

static struct association {
  unsigned char win32_code, errno_code;
} lookup_table[] =
{
  {ERROR_INVALID_FUNCTION,      EINVAL},
  {ERROR_FILE_NOT_FOUND,        ENOENT},
  {ERROR_PATH_NOT_FOUND,        ENOENT},
  {ERROR_TOO_MANY_OPEN_FILES,   EMFILE},
  {ERROR_ACCESS_DENIED,         EACCES},
  {ERROR_INVALID_HANDLE,        EBADF},
  {ERROR_ARENA_TRASHED,         ENOMEM},
  {ERROR_NOT_ENOUGH_MEMORY,     ENOMEM},
  {ERROR_INVALID_BLOCK,         ENOMEM},
  {ERROR_BAD_ENVIRONMENT,       E2BIG},
  {ERROR_BAD_FORMAT,            ENOEXEC},
  {ERROR_INVALID_ACCESS,        EINVAL},
  {ERROR_INVALID_DATA,          EINVAL},
  {ERROR_INVALID_DRIVE,         ENOENT},
  {ERROR_CURRENT_DIRECTORY,     EACCES},
  {ERROR_NOT_SAME_DEVICE,       EXDEV},
  {ERROR_NO_MORE_FILES,         ENOENT},
  {ERROR_LOCK_VIOLATION,        EACCES},
  {ERROR_BAD_NETPATH,           ENOENT},
  {ERROR_NETWORK_ACCESS_DENIED, EACCES},
  {ERROR_BAD_NET_NAME,          ENOENT},
  {ERROR_FILE_EXISTS,           EEXIST},
  {ERROR_CANNOT_MAKE,           EACCES},
  {ERROR_FAIL_I24,              EACCES},
  {ERROR_INVALID_PARAMETER,     EINVAL},
  {ERROR_NO_PROC_SLOTS,         EAGAIN},
  {ERROR_DRIVE_LOCKED,          EACCES},
  {ERROR_BROKEN_PIPE,           EPIPE},
  {ERROR_DISK_FULL,             ENOSPC},
  {ERROR_INVALID_TARGET_HANDLE, EBADF},
  {ERROR_INVALID_VERIFY_SWITCH, EINVAL},
  {ERROR_WAIT_NO_CHILDREN,      ECHILD},
  {ERROR_CHILD_NOT_COMPLETE,    ECHILD},
  {ERROR_DIRECT_ACCESS_HANDLE,  EBADF},
  {ERROR_NEGATIVE_SEEK,         EINVAL},
  {ERROR_SEEK_ON_DEVICE,        EACCES},
  {ERROR_DIR_NOT_EMPTY,         ENOTEMPTY},
  {ERROR_NOT_LOCKED,            EACCES},
  {ERROR_BAD_PATHNAME,          ENOENT},
  {ERROR_MAX_THRDS_REACHED,     EAGAIN},
  {ERROR_LOCK_FAILED,           EACCES},
  {ERROR_ALREADY_EXISTS,        EEXIST},
  {ERROR_FILENAME_EXCED_RANGE,  ENOENT},
  {ERROR_NESTING_NOT_ALLOWED,   EAGAIN}
};

#define TABLE_SIZE sizeof lookup_table / sizeof (struct association)

unsigned __pascal __set_nt_errno ()
{
  struct association *assoc;
# ifdef _MT
    struct THREAD *my_thread = _getthreaddata ();
  
#   define ERRNO (my_thread->t_errno)
#   define DOSERRNO (my_thread->t_doserrno)
# else
#   define ERRNO errno
#   define DOSERRNO _doserrno
# endif
  
#if defined(_MT)
    /* if getthreaddata failes, can't do anything, so bail.. */
    if (!my_thread)
    {
         return EINVAL;
    }
#endif

  DOSERRNO = GetLastError ();
  for (assoc = lookup_table; assoc < lookup_table + TABLE_SIZE; assoc += 1)
    if (assoc->win32_code == DOSERRNO)
    {
      ERRNO = assoc->errno_code;
      return DOSERRNO;
    }
  if (DOSERRNO >= ERROR_WRITE_PROTECT
    && DOSERRNO <= ERROR_SHARING_BUFFER_EXCEEDED)
      ERRNO = EACCES;
  else if (DOSERRNO >= ERROR_INVALID_STARTING_CODESEG
    && DOSERRNO <= ERROR_INFLOOP_IN_RELOC_CHAIN)
      ERRNO = ENOEXEC;
  else
    ERRNO = EINVAL;
  return DOSERRNO;
}

#endif
