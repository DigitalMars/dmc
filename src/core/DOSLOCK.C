/* Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 */
 
#include <dos.h>
#include <errno.h>
#include <scrtl.h>
#ifdef __NT__
# include <scio.h>
#endif

unsigned _dos_lock (int fh, int function, unsigned long rgn_off, unsigned long rgn_len)
{
# ifdef __NT__
    switch (function)
    {
    case 0:
      if (LockFile (_osfhnd[fh], rgn_off, 0, rgn_len, 0))
        return 0;
      break;
    case 1:
      if (UnlockFile (_osfhnd[fh], rgn_off, 0, rgn_len, 0))
        return 0;
      break;
    default:
      __set_errno (EINVAL);
      return -1;
    }
    return __set_nt_errno ();
# else
    asm
    {
      mov  bx,word ptr fh
      mov  dx,word ptr rgn_off
      mov  cx,word ptr rgn_off+2
      mov  di,word ptr rgn_len
      mov  si,word ptr rgn_len+2
      mov  al,byte ptr function
      mov  ah,5ch
      int  21h
      jc   error
    }
    return 0;
  error:
    return __set_dos_errno (_AX);
# endif
}
