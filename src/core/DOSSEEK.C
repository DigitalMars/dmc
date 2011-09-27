/* Copyright (C) 1994 by Digital Mars
 * All Rights Reserved
 */
 
#include <dos.h>
#include <errno.h>
#include <scrtl.h>
#ifdef __NT__
# include <scio.h>
#endif

unsigned long _dos_seek (int fh, unsigned long foffset, int method)
{
# ifdef __NT__
    DWORD new_pointer;
    
    new_pointer = SetFilePointer (_osfhnd[fh], (long)foffset, NULL, method);
    if (new_pointer != -1)
      return new_pointer;
    __set_nt_errno ();
    return -1;
# else
    asm
    {
      mov  bx,word ptr fh
      mov  dx,word ptr foffset
      mov  cx,word ptr foffset+2
      mov  al,byte ptr method
      mov  ah,42h
      int  21h
      jc   error
    }
    return; /* DX:AX */
  error:
    __set_dos_errno (_AX);
    return -1;
# endif
}

